#include"bTree.h"

/*
================================================
Arquivo da arvore-B
================================================
*/

//Constroe uma strcut trio, usada nas funcoes da arvore-B,
// que leva um id e offset do registro, e o filho direito do registro
trio makeTrio(int id, int64_t off, int filho){
    trio a; a.id = id, a.offset = off; a.filho = filho;
    return a;
}

//Inicializa um header da bt
BT_HEADER *btHeaderInit(){
    BT_HEADER *bth = (BT_HEADER*)malloc(sizeof(BT_HEADER));
    bth->status = '1';
    bth->noRaiz = -1;
    bth->proxRRN = -1;
    bth->nroChaves = 0;
    return bth;
}


//Incializa o arquivo e o header da arvore b
BT_HEADER *bTreeInit(char *filename){
    BT_HEADER *bth = btHeaderInit();
    FILE *btFile = fopen(filename, "wb");
    BT_writeHeader(bth, btFile, false);
    fclose(btFile);
    return bth;
}
//Escreve o header no arquivo
void BT_writeHeader(BT_HEADER *bth, FILE* fd, bool seek){
    if(seek)
        fseek(fd, 0, SEEK_SET);
    char cifrao = '$';
    fwrite(&(bth->status), 1, 1, fd);
    fwrite(&(bth->noRaiz), 4, 1, fd);
    fwrite(&(bth->proxRRN), 4, 1, fd);
    fwrite(&(bth->nroChaves), 4, 1, fd);
    for(int i = 0; i < 47; i++)
        fwrite(&cifrao, 1, 1,fd);
}
//Le o header de um arquivo
BT_HEADER* bt_headerRead(FILE *btFile){
    if(ftell(btFile) != 0)
        fseek(btFile, 0, SEEK_SET);
    BT_HEADER *bth = btHeaderInit();
    fread(&(bth->status), 1, 1, btFile);
    fread(&(bth->noRaiz), 4, 1, btFile);
    fread(&(bth->proxRRN), 4, 1, btFile);
    fread(&(bth->nroChaves), 4, 1, btFile);
    return bth;
}
//Inicializa uma pagina de arvore b
BT_NODE *BT_nodeInit(int height){
    BT_NODE *btn = (BT_NODE*)malloc(sizeof(BT_NODE));
    btn->alturaNo = height;
    btn->nroChaves = 0;
    for(int i = 0; i < 3; i++){
        btn->chaves[i] = -1;
        btn->filhos[i] = -1;
        btn->offsets[i] = -1;
    }
    btn->filhos[3] = -1;
    return btn;
}

//Libera memoria de uma pagina de arvore b
void BT_nodeFree(BT_NODE **btn){
    free(*btn);
    *btn = NULL;
}
//Le uma pagina de arvore b
BT_NODE *BT_nodeRead(int RRN, FILE *fd){
    fseek(fd, 60 + (60 * RRN), SEEK_SET);
    BT_NODE *btn = BT_nodeInit(0);
    fread(&(btn->alturaNo), 4, 1, fd);
    fread(&(btn->nroChaves), 4, 1, fd);
    for(int i = 0; i < 3; i++){
        fread(&(btn->chaves[i]), 4, 1, fd);
        fread(&(btn->offsets[i]), 8, 1, fd);
    }
    fread(&(btn->filhos), 4, 4, fd);
    return btn;
}

//Escreve uma pagina de arvore b
void BT_nodeWrite(int RRN, BT_NODE *btn, FILE *fd){
    fseek(fd, 60 + (60 * RRN), SEEK_SET);
    fwrite(&(btn->alturaNo), 4, 1, fd);
    fwrite(&(btn->nroChaves), 4, 1, fd);
    for(int i = 0; i < 3; i++){
        fwrite(&(btn->chaves[i]), 4, 1, fd);
        fwrite(&(btn->offsets[i]), 8, 1, fd);
    }
    fwrite(&(btn->filhos), 4, 4, fd);
}

//Funcao recursiva de busca na arvore b
int64_t BT_searchAux(int nodeRRN, int id, FILE *btfile){
    //Caso base, chegou no fim e nao achou
    if(nodeRRN == -1)
        return -1;
    int64_t offset;
    int nextRRN;
    BT_NODE *current = BT_nodeRead(nodeRRN, btfile);
    //Busca dentro da pagina
    for(int i = 0; i < current->nroChaves; i++){
        if(current->chaves[i] == id){
            offset = current->offsets[i];
            BT_nodeFree(&current);
            return offset;
        }
        //Descida na recursao
        if(current->chaves[i] > id){
            nextRRN = current->filhos[i];
            BT_nodeFree(&current);
            return BT_searchAux(nextRRN, id, btfile);
        } 
    }
    //Descida na recursao
    nextRRN = current->filhos[current->nroChaves];
    BT_nodeFree(&current);
    return BT_searchAux(nextRRN, id, btfile);
}

//Funcao de busca por um id na arvore b, retorna o offset, ou -1 caso nao encontre
int64_t BT_search(BT_HEADER *bth, int id, FILE *btfile){
    return BT_searchAux(bth->noRaiz, id, btfile);
}

//Remove o item mais a direita de uma pagina
//Retorna o trio com id, offset e filho direito
//Usado para o split
trio BT_nodeRemoveRight(BT_NODE *btn){
    trio promoted;
    promoted.id = btn->chaves[btn->nroChaves - 1];
    promoted.offset = btn->offsets[btn->nroChaves - 1];
    promoted.filho = btn->filhos[btn->nroChaves];
    btn->chaves[btn->nroChaves - 1] = -1;
    btn->offsets[btn->nroChaves - 1] = -1;
    btn->filhos[btn->nroChaves] = -1;
    btn->nroChaves--;
    return promoted;
}

//Funcoes de print para debug

void printNormal(int RRN, FILE *fd){
    if(RRN == -1)
        return;
    BT_NODE *cur = BT_nodeRead(RRN, fd);
    printf("%d, h = %d : ", RRN, cur->alturaNo);
    for (int i = 0; i < cur->nroChaves; i++){
        printf("{%d} (id : %d | off : %ld) ", cur->filhos[i], cur->chaves[i], cur->offsets[i]);
    }
    printf("{%d}\n", cur->filhos[cur->nroChaves]);
    for(int i = 0; i < cur->nroChaves; i++)
        printNormal(cur->filhos[i], fd);
    printNormal(cur->filhos[cur->nroChaves], fd);
    BT_nodeFree(&cur);
}

bool BT_printWrap(BT_HEADER *bth, FILE *fd){
    bool arr[100] = {0}, flag = false;
    BT_print(bth->noRaiz, fd, arr);
    for(int i = 0; i < bth->proxRRN; i++){
        if(!arr[i]){
            printf("Pagina perdida : %d\n", i);
            flag = true;
        }
    }
    return flag;

}

//Insere um novo registro dentro de um no
//Caso haja overflow, insere e retorna o maior
trio BT_nodeInsert(BT_NODE *btn, trio new){
    trio promoted;
    //Caso de overflow
    if(btn->nroChaves == 3){
        for(int i = 0; i < btn->nroChaves; i++){
            if(new.id < btn->chaves[i]){
                promoted = makeTrio(btn->chaves[btn->nroChaves - 1], btn->offsets[btn->nroChaves - 1],btn->filhos[btn->nroChaves]);
                for(int j = btn->nroChaves - 2; j >= i; j--){
                    btn->chaves[j + 1] = btn->chaves[j];
                    btn->offsets[j + 1] =  btn->offsets[j];
                }
                for(int j = btn->nroChaves - 1; j > i; j--)
                    btn->filhos[j+1] = btn->filhos[j];
                btn->chaves[i] = new.id;
                btn->offsets[i] = new.offset;
                btn->filhos[i + 1] = new.filho;
                return promoted;
            }
        }   
        return new; 
    }
    //Caso vazio
    if(btn->nroChaves == 0){
        btn->chaves[0] = new.id;
        btn->offsets[0] = new.offset;
        btn->filhos[1] = new.filho;
        btn->nroChaves++;
        return makeTrio(-1, -1, -1);
    }   
    //Caso normal
    for(int i = 0; i <= btn->nroChaves; i++){
        if(i == btn->nroChaves){
            btn->chaves[i] = new.id;
            btn->offsets[i] = new.offset;
            btn->filhos[i + 1] = new.filho;
            break;
        }
        if(btn->chaves[i] > new.id){
            for(int j = btn->nroChaves - 1; j >= i; j--)
                btn->chaves[j + 1] = btn->chaves[j];
            for(int j = btn->nroChaves - 1; j >= i; j--)
                btn->offsets[j + 1] = btn->offsets[j];
            for(int j = btn->nroChaves; j > i; j--)
                btn->filhos[j + 1] = btn->filhos[j];
            btn->chaves[i] = new.id;
            btn->offsets[i] = new.offset;
            btn->filhos[i + 1] = new.filho;
            break;
        }
    }
    btn->nroChaves++;
    return makeTrio(-1, -1, -1);
}


//Funcao recursiva da insercao
//Retorna um trio cujo id sera -1, caso nao haja split, e um id valido se haver
trio BT_insertAux(FILE* btfile, BT_HEADER *bth, int currentRRN, trio reg){
    //Leitura da pagina
    BT_NODE *current = BT_nodeRead(currentRRN, btfile);
    trio promoted;
    //Caso seja uma pagina folha
    if(current->filhos[0] ==  -1){
        //Caso nao haja overflow
        if((promoted = BT_nodeInsert(current, reg)).id == -1){
            BT_nodeWrite(currentRRN, current, btfile);
            fflush(btfile);
            BT_nodeFree(&current);
            return makeTrio(-1 ,-1, -1);
        }
        //Caso haja
        BT_nodeWrite(currentRRN, current, btfile);
        fflush(btfile);
        return promoted;
    }
    //Descida na recursao
    int nextPage =  current->nroChaves;
    for(int i = 0; i < current->nroChaves; i++){
        if(reg.id < current->chaves[i]){
            nextPage = i;
            break;
        }
    }   
    promoted = BT_insertAux(btfile, bth, current->filhos[nextPage], reg);
    //Sem split
    if(promoted.id == -1)
        return promoted;
    //Com split na pagina inferior e na atual
    if(current->nroChaves == 3){
        trio middle, second;
        BT_NODE *newRight, *left;
        left = BT_nodeRead(current->filhos[nextPage], btfile); 
        newRight = BT_nodeInit(current->alturaNo - 1);
        second = BT_nodeRemoveRight(left);
        middle = BT_nodeRemoveRight(left);
        if(middle.filho == -1)
            middle.filho = bth->proxRRN;
        else{
            newRight->filhos[0] = middle.filho;
            middle.filho = bth->proxRRN;
        }
        BT_nodeInsert(newRight, promoted);
        BT_nodeInsert(newRight, second);
        promoted = BT_nodeInsert(current, middle);
        BT_nodeWrite(bth->proxRRN++, newRight, btfile);
        BT_nodeWrite(current->filhos[nextPage], left,btfile);
        BT_nodeWrite(currentRRN, current, btfile);
        return promoted;
    }
    //Com split apenas na pagina inferior
    for(int i = current->nroChaves - 1; i >= nextPage; i--){
        current->chaves[i + 1] = current->chaves[i];
        current->offsets[i + 1] = current->offsets[i]; 
    }
    for(int i = current->nroChaves; i > nextPage; i--)
        current->filhos[i + 1] = current->filhos[i];
    BT_NODE *newRight = BT_nodeInit(current->alturaNo - 1);
    BT_NODE *left = BT_nodeRead(current->filhos[nextPage], btfile);
    trio second = BT_nodeRemoveRight(left);
    trio middle = BT_nodeRemoveRight(left);
    current->chaves[nextPage]= middle.id;
    current->offsets[nextPage] = middle.offset;
    current->filhos[nextPage + 1] = bth->proxRRN;
    current->nroChaves++;
    BT_nodeInsert(newRight, second);
    BT_nodeInsert(newRight, promoted);
    newRight->filhos[0] = middle.filho;
    BT_nodeWrite(current->filhos[nextPage], left, btfile);
    BT_nodeWrite(currentRRN, current, btfile);
    BT_nodeWrite(bth->proxRRN++, newRight ,btfile);
    //bth->nroChaves++;
    return makeTrio(-1, -1 ,-1);
}

//Funcao da insercao
void BT_insert(FILE *btFile, BT_HEADER *bth, trio reg){
    //caso vazio
    if(bth->noRaiz == -1){ 
        bth->noRaiz = 0;
        bth->proxRRN = 1;
        bth->nroChaves++;
        BT_NODE *btn = BT_nodeInit(0);
        BT_nodeInsert(btn, reg);
        BT_nodeWrite(0, btn, btFile);
        BT_writeHeader(bth, btFile, true);
        fflush(btFile);
        BT_nodeFree(&btn);
        return;
    }
    //Insere recursivamentes
    trio promoted = BT_insertAux(btFile, bth, bth->noRaiz, reg);
    if(promoted.id == -1){
        bth->nroChaves++;
        return;
    }
    //Caso do split da raiz
    trio middle, second;
    BT_NODE *newRoot, *newRight, *oldRoot;
    oldRoot = BT_nodeRead(bth->noRaiz, btFile);
    newRoot = BT_nodeInit(oldRoot->alturaNo + 1);
    newRight = BT_nodeInit(oldRoot->alturaNo);
    second = BT_nodeRemoveRight(oldRoot); //Entra na nova direita
    middle = BT_nodeRemoveRight(oldRoot); //Sera a nova chave separadora
    //Insercao nas respectivas paginas
    BT_nodeInsert(newRoot, middle);
    BT_nodeInsert(newRight, second);
    BT_nodeInsert(newRight, promoted);
    //Ajuste dos ponteiros
    newRoot->filhos[0] = bth->noRaiz;
    newRoot->filhos[1] = bth->proxRRN;
    newRight->filhos[0] = middle.filho;
    newRight->filhos[1] = second.filho;
    //Escrita
    BT_nodeWrite(bth->noRaiz, oldRoot, btFile);
    BT_nodeWrite(bth->proxRRN++, newRight, btFile);
    //Ajuste do header
    bth->noRaiz = bth->proxRRN;
    bth->nroChaves++;
    BT_nodeWrite(bth->proxRRN++, newRoot, btFile);
    fflush(btFile);
}
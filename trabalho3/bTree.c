#include"bTree.h"



trio makeTrio(int id, int64_t off, int filho){
    trio a; a.id = id, a.offset = off; a.filho = filho;
    return a;
}
BT_HEADER *btHeaderInit(){
    BT_HEADER *bth = (BT_HEADER*)malloc(sizeof(BT_HEADER));
    bth->status = '1';
    bth->noRaiz = -1;
    bth->proxRRN = -1;
    bth->nroChaves = 0;
    return bth;
}


BT_HEADER *bTreeInit(char *filename){
    BT_HEADER *bth = btHeaderInit();
    FILE *btFile = fopen(filename, "wb");
    BT_writeHeader(bth, btFile, false);
    fclose(btFile);
    return bth;
}




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


void BT_nodeFree(BT_NODE **btn){
    free(*btn);
    *btn = NULL;
}

BT_NODE *BT_nodeRead(int RRN, FILE *fd){
    if(RRN != -1)
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




int64_t BT_search_Node(BT_NODE *btn, int id){
    for(int i = 0; i < btn->nroChaves; i++)
        if(btn->chaves[i] == id)
            return btn->offsets[i];
    return -1;
}


int64_t BT_searchAux(int nodeRRN, int id, FILE *btfile){
    if(nodeRRN == -1)
        return -1;
    int64_t offset;
    int nextRRN;
    BT_NODE *current = BT_nodeRead(nodeRRN, btfile);
    for(int i = 0; i < current->nroChaves; i++){
        if(current->chaves[i] == id){
            offset = current->offsets[i];
            BT_nodeFree(&current);
            return offset;
        }
        if(current->chaves[i] > id){
            nextRRN = current->filhos[i];
            BT_nodeFree(&current);
            return BT_searchAux(nextRRN, id, btfile);
        } 
    }
    nextRRN = current->filhos[current->nroChaves];
    BT_nodeFree(&current);
    return BT_searchAux(nextRRN, id, btfile);
}


int64_t BT_search(BT_HEADER *bth, int id, FILE *btfile){
    return BT_searchAux(bth->noRaiz, id, btfile);
}


trio BT_nodeRemoveRight(BT_NODE *btn){
    trio promoted;
    promoted.id = btn->chaves[2];
    promoted.offset = btn->offsets[2];
    promoted.filho = btn->filhos[3];
    btn->chaves[2] = -1;
    btn->offsets[2] = -1;
    btn->filhos[3] = -1;
    btn->nroChaves--;
    return promoted;
}


void logNode(BT_NODE* btn){
    FILE *fd =  fopen("node.log", "a");
        for (int i = 0; i < 3; i++){
            fprintf(fd,"{%d} (id : %d | off : %ld) ", btn->filhos[i], btn->chaves[i], btn->offsets[i]);
        }
        fprintf(fd,"{%d}\n", btn->filhos[3]);
    fclose(fd);
}
void BT_print(int RRN, FILE *fd, bool arr[]){
    if(RRN == -1)
        return;
    arr[RRN] = true;
    BT_NODE *cur = BT_nodeRead(RRN, fd);
    printf("%d, h = %d : ", RRN, cur->alturaNo);
    for (int i = 0; i < cur->nroChaves; i++){
        printf("{%d} (id : %d | off : %ld) ", cur->filhos[i], cur->chaves[i], cur->offsets[i]);
    }
    printf("{%d}\n", cur->filhos[cur->nroChaves]);
    for(int i = 0; i < cur->nroChaves; i++)
        BT_print(cur->filhos[i], fd, arr);
    BT_print(cur->filhos[cur->nroChaves], fd, arr);
    BT_nodeFree(&cur);
}

#include<limits.h>
bool validRec(int RRN,FILE *fd, int lower, int higher){
    if(RRN == -1)
        return true;
    BT_NODE *cur = BT_nodeRead(RRN, fd);
    bool flag = true, arr[4];
    if(lower > cur->chaves[0] || higher < cur->chaves[cur->nroChaves]){
        BT_nodeFree(&cur);
        return false;
    }
    for(int i = 0; i < cur->nroChaves - 1; i++){
        if(cur->chaves[i] > cur->chaves[i + 1]){
            BT_nodeFree(&cur);
            return false;
        }
    }
    arr[0] = validRec(cur->filhos[0], fd, INT_MIN, cur->chaves[0]);
    arr[cur->nroChaves] = validRec(cur->filhos[cur->nroChaves],fd, cur->chaves[cur->nroChaves-1], INT_MAX );
    for(int i = 1; i < cur->nroChaves; i++){
        arr[i] = validRec(cur->filhos[i], fd, cur->chaves[i-1], cur->chaves[i]);
    }
    for(int i = 0; i <= cur->nroChaves; i++){
        if(!arr[i]){
            BT_nodeFree(&cur);
            return false;
        }
    }
    BT_nodeFree(&cur);
    return true;

}


bool valid(BT_HEADER *bth, FILE *fd){
    return validRec(bth->noRaiz, fd, INT_MIN, INT_MAX);
}


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


trio BT_nodeInsert(BT_NODE *btn, trio new){
    trio promoted;
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
    if(btn->nroChaves == 0){
        btn->chaves[0] = new.id;
        btn->offsets[0] = new.offset;
        btn->filhos[1] = new.filho;
        btn->nroChaves++;
        return makeTrio(-1, -1, -1);
    }   
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



trio BT_insertAux(FILE* btfile, BT_HEADER *bth, int currentRRN, trio reg){
    BT_NODE *current = BT_nodeRead(currentRRN, btfile);
    trio promoted;
    //logNode(current);
    if(current->filhos[0] ==  -1){
        if((promoted = BT_nodeInsert(current, reg)).id == -1){
            BT_nodeWrite(currentRRN, current, btfile);
            fflush(btfile);
            BT_nodeFree(&current);
            return makeTrio(-1 ,-1, -1);
        }
        BT_nodeWrite(currentRRN, current, btfile);
        fflush(btfile);
        return promoted;
    }
    int nextPage =  current->nroChaves;
    for(int i = 0; i < current->nroChaves; i++){
        if(reg.id < current->chaves[i]){
            nextPage = i;
            break;
        }
    }   
    promoted = BT_insertAux(btfile, bth, current->filhos[nextPage], reg);
    if(promoted.id == -1)
        return promoted;
    if(current->nroChaves == 3){
        trio middle;
        BT_NODE *newRight, *left;
        left = BT_nodeRead(current->filhos[nextPage], btfile); 
        newRight = BT_nodeInit(current->alturaNo - 1);
        middle = BT_nodeRemoveRight(left);
        if(middle.filho == -1)
            middle.filho = bth->proxRRN;
        else{
            newRight->filhos[0] = middle.filho;
            middle.filho = bth->proxRRN;
        }
        BT_nodeInsert(newRight, promoted);
        promoted = BT_nodeInsert(current, middle);
        BT_nodeWrite(bth->proxRRN++, newRight, btfile);
        BT_nodeWrite(current->filhos[nextPage], left,btfile);
        BT_nodeWrite(currentRRN, current, btfile);
        return promoted;
    }
    for(int i = current->nroChaves - 1; i >= nextPage; i--){
        current->chaves[i + 1] = current->chaves[i];
        current->offsets[i + 1] = current->offsets[i]; 
    }
    for(int i = current->nroChaves; i > nextPage; i--)
        current->filhos[i + 1] = current->filhos[i];
    BT_NODE *newRight = BT_nodeInit(current->alturaNo - 1);
    BT_NODE *left = BT_nodeRead(current->filhos[nextPage], btfile);
    trio middle = BT_nodeRemoveRight(left);
    current->chaves[nextPage]= middle.id;
    current->offsets[nextPage] = middle.offset;
    current->filhos[nextPage + 1] = bth->proxRRN;
    current->nroChaves++;
    BT_nodeInsert(newRight, promoted);
    newRight->filhos[0] = middle.filho;
    BT_nodeWrite(current->filhos[nextPage], left, btfile);
    BT_nodeWrite(currentRRN, current, btfile);
    BT_nodeWrite(bth->proxRRN++, newRight ,btfile);
    //bth->nroChaves++;
    return makeTrio(-1, -1 ,-1);
}

void BT_insert(FILE *btFile, BT_HEADER *bth, trio reg){
    if(bth->noRaiz == -1){ //caso vazio
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
    trio promoted = BT_insertAux(btFile, bth, bth->noRaiz, reg);
    if(promoted.id == -1){
        bth->nroChaves++;
        return;
    }
    trio middle;
    BT_NODE *newRoot, *newRight, *oldRoot;
    oldRoot = BT_nodeRead(bth->noRaiz, btFile);
    newRoot = BT_nodeInit(oldRoot->alturaNo + 1);
    newRight = BT_nodeInit(oldRoot->alturaNo);
    middle = BT_nodeRemoveRight(oldRoot);
    BT_nodeInsert(newRoot, middle);
    BT_nodeInsert(newRight, promoted);
    newRoot->filhos[0] = bth->noRaiz;
    newRoot->filhos[1] = bth->proxRRN;
    newRight->filhos[0] = middle.filho;
    newRight->filhos[1] = promoted.filho;;
    BT_nodeWrite(bth->noRaiz, oldRoot, btFile);
    BT_nodeWrite(bth->proxRRN++, newRight, btFile);
    bth->noRaiz = bth->proxRRN;
    bth->nroChaves++;
    BT_nodeWrite(bth->proxRRN++, newRoot, btFile);
    fflush(btFile);
}
#include"bTree.h"



BT_HEADER *bTreeInit(char *filename){
    BT_HEADER *bth = btHeaderInit();
    FILE *btFile = fopen(filename, "wb");
    BT_writeHeader(bth, btFile, false);
    fclose(btFile);
    return bth;
}

BT_HEADER *btHeaderInit(){
    BT_HEADER *bth = (BT_HEADER*)malloc(sizeof(BT_HEADER));
    bth->status = '1';
    bth->noRaiz = -1;
    bth->proxRRN = -1;
    bth->nroChaves = 0;
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
    for(int i = 0; i < 57; i++)
        fwrite(&cifrao, 1, 1,fd);
}

BT_HEADER* bt_headerRead(FILE *btFile){
    if(ftell(btFile) != 0)
        fseek(btFile, 0, SEEK_SET);
    BT_HEADER *bth = btHeaderInit();
    fread(&(bth->status), 1, 1, btFile);
    fread(&(bth->noRaiz), 1, 1, btFile);
    fread(&(bth->proxRRN), 1, 1, btFile);
    fread(&(bth->nroChaves), 1, 1, btFile);
    return bth;
}




BT_NODE *BT_nodeInit(int height){
    BT_NODE *btn = (BT_NODE*)malloc(sizeof(BT_NODE));
    btn->alturaNo = height;
    btn->nroChaves = 0;
    btn->chaves = (int*)malloc(3 *sizeof(int));
    btn->filhos = (int*)malloc(4 *sizeof(int));
    btn->offsets = (int*)malloc(3 *sizeof(int));
    for(int i = 0; i < 3; i++){
        btn->chaves = -1;
        btn->filhos = -1;
        btn->offsets = -1;
    }
    btn->filhos[3] = -1;
    btn->pai = -1;
    btn->RRN = -1;
    return btn;
}


void BT_nodeFree(BT_NODE **btn){
    free((*btn)->chaves);
    free((*btn)->filhos);
    free((*btn)->offsets);
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
    btn->RRN = RRN;
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

bool BT_nodeInsert(BT_NODE *btn, int id, int64_t offset){
    if(btn->nroChaves == 3)
        return false;
    if(btn->nroChaves == 0){
        btn->chaves[0] = id;
        btn->offsets[0] = offset;
        return true;
    }   
    for(int i = 0; i < btn->nroChaves; i++){
        if(btn->chaves[i] > id){
            for(int j = btn->nroChaves - 1; j >= i; j--)
                btn->chaves[j + 1] = btn->chaves[j];
            for(int j = btn->nroChaves - 1; j >= i; j--)
                btn->offsets[j + 1] = btn->offsets[j];
            btn->chaves[i] = id;
            btn->offsets[i] = offset;
        }
    }
    return true;
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
        if(current->chaves[i] < id){
            nextRRN = current->filhos[i];
            BT_nodeFree(&current);
            return BT_searchAux(nextRRN, id, btfile);
        } 
    }
    nextRRN = current->nroChaves + 1;
    BT_nodeFree(&current);
    return BT_searchAux(nextRRN, id, btfile);
}


int64_t BT_search(BT_HEADER *bth, int id, FILE *btfile){
    return BT_searchAux(bth->noRaiz, id, btfile);
}


pair BT_nodeRemoveRight(BT_NODE *btn){
    pair promoted;
    promoted.id = btn->chaves[2];
    promoted.offset = btn->offsets[2];
    btn->chaves[2] = -1;
    btn->offsets[2] = -1;
    return promoted;
}

pair BT_insertAux(FILE* btfile, BT_HEADER *bth, int currentRRN, pair reg){
    BT_NODE *current = BT_nodeRead(currentRRN, btfile);
    if(current->filhos[0] ==  -1){
        if(BT_nodeInsert(current, reg.id, reg.offset)){
            BT_nodeWrite(currentRRN, current, btfile);
            BT_nodeFree(&current);
            pair ret; ret.id = -1;
            return ret;
        }
        pair promoted = BT_nodeRemoveRight(current);
        BT_nodeWrite(currentRRN, current, btfile);
        return promoted;
    }
    for(int i = 0; i < current->nroChaves; i++){

    }
}

void BT_insert(FILE *btFile, BT_HEADER *bth, pair reg){
    if(bth->noRaiz == -1){ //caso vazio
        bth->noRaiz = 0;
        bth->proxRRN = 1;
        bth->nroChaves++;
        BT_NODE *btn = BT_nodeInit(0);
        BT_nodeInsert(btn, reg.id, reg.offset);
        BT_nodeWrite(0, btn, btFile);
        BT_nodeFree(&btn);
        return;
    }
    pair promoted = BT_insertAux(btFile, bth, bth->noRaiz, reg);


}
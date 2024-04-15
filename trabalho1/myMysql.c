#include"myMysql.h"



void selectWhere(char* filename, int numOfParameters, char** fields, char** values){
    FILE *fd = fopen(filename, "rb");
    if(fd == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    //tratar cabecalho
    int64_t offset = 25;
    bool flag = false;
    while(!feof(fd)){
        PLAYER *p = playerFromBin(fd, offset);
        if(checkPlayer(p, numOfParameters, fields, values)){
            playerPrint(p);
            flag = true;
        }
        offset += playerTamanho(p);
        playerFree(&p);
    }
    if(!flag)
        printf("Registro inexistente.\n\n");
}



void selectSQL(char* filename){
    FILE* fd =  fopen(filename, "rb");
    if(fd == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    bool flag =  false;
    //tratar cabecalho
    HEADER *h = extraiHeader(fd);
    int64_t offset = 25;
    //fseek(fd, 25, SEEK_SET);
    while(!feof(fd) && h->offset > offset){
        PLAYER *p = playerFromBin(fd, offset);
        if(strcmp(p->nome,  "M. RISTIC") == 0)
            printf("parou");
        if(p->status == '0'){
            playerPrint(p);
            flag = true;
        }
        offset += playerTamanho(p);
        playerFree(&p);
    }
    if(!flag)
        printf("Registro inexistente.\n\n");
}

void createTable(char* srcName, char* destName){
    if(fopen(srcName, "rb") == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    csvToBin(srcName, destName);
}
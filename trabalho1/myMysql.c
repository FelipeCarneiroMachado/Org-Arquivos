#include"myMysql.h"



void selectWhere(char* filename, int numOfParameters, char** fields, char** values){
    FILE *fd = fopen(filename, "rb");
    if(fd == NULL){
        printf("Falha no processamento do arquivo\n");
        return;
    }
    //tratar cabecalho
    int64_t offset = 25;
    while(!feof(fd)){
        PLAYER *p = playerFromBin(fd, offset);
        if(checkPlayer(p, numOfParameters, fields, values)){
            playerPrint(p);
        }
        offset += playerTamanho(p);
        playerFree(&p);
    }

}

void selectSQL(char* filename){
    FILE* fd =  fopen(filename, "rb");
    if(fd == NULL){
        printf("Falha no processamento do arquivo\n");
        return;
    }
    //tratar cabecalho
    int64_t offset = 25;
    //fseek(fd, 25, SEEK_SET);
    while(!feof(fd)){
        PLAYER *p = playerFromBin(fd, offset);
        if(p->status == '0')
            playerPrint(p);
        offset += playerTamanho(p);
        playerFree(&p);
    }
}

void createTable(char* srcName, char* destName){
    if(fopen(srcName, "rb") == NULL){
        printf("Falha no processamento do arquivo\n");
        return;
    }
    csvToBin(srcName, destName);
}
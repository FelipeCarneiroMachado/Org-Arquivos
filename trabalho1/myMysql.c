#include"myMysql.h"




void selectWhere(char* filename, int numOfParameters, char** fields, char** values){
    FILE *fd = fopen(filename, "rb");
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
    //tratar cabecalho
    int64_t offset = 25;
    //fseek(fd, 25, SEEK_SET);
    while(!feof(fd)){
        PLAYER *p = playerFromBin(fd, offset);
        playerPrint(p);
        offset += playerTamanho(p);
        playerFree(&p);
    }
}

void createTable(char* srcName, char* destName){
    csvToBin(srcName, destName);
}
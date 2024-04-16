#include"myMysql.h"



void selectWhere(char* filename, int numOfParameters, char** fields, char** values){
    FILE *fd = fopen(filename, "rb");
    if(fd == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    //tratar cabecalho
    HEADER *h = extraiHeader(fd);
    if(h->status == '0'){
        printf("\nArquivo de dados inconsistente\n");
        return;
    }
    int64_t offset = 25;
    bool flagFound = false, searchForId = false;
    for(int i = 0; i < numOfParameters; i++)
        if(strcmp("id", fields[i]) == 0)
            searchForId = true;
    while(h->offset > offset){
        PLAYER *p = playerFromBin(fd, offset);
        if(checkPlayer(p, numOfParameters, fields, values)){
            playerPrint(p);
            flagFound = true;
            if(searchForId)
                break;
        }
        offset += playerTamanho(p);
        playerFree(&p);
    }
    if(!flagFound)
        printf("Registro inexistente.\n\n");
    fclose(fd);
    free(h);
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
    if(h->status == '0'){
        printf("\nArquivo de dados inconsistente\n");
        return;
    }
    int64_t offset = 25;
    //fseek(fd, 25, SEEK_SET);
    while(!feof(fd) && h->offset > offset){
        PLAYER *p = playerFromBin(fd, offset);
        // if(strcmp(p->nome,  "M. RISTIC") == 0)
        //     printf("parou");
        if(p->status == '0'){
            playerPrint(p);
            flag = true;
        }
        offset += playerTamanho(p);
        playerFree(&p);
    }
    if(!flag)
        printf("Registro inexistente.\n\n");
    fclose(fd);
    free(h);
    
}

void createTable(char* srcName, char* destName){
    if(fopen(srcName, "rb") == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    csvToBin(srcName, destName);
}
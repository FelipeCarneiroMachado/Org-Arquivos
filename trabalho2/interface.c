#include"interface.h"
/*
================================================
Arquivo fonte da interface
================================================
*/

//Printa os registros que batem com as condicoes
//Recebe 2 arrays de strings, 1 com os campos a serem comparados e outro com os valores
//Os arrays devem ser pareados (campo[i] corresponde a valor[i])
void selectWhere(char* filename, int numOfParameters, char** fields, char** values){
    FILE *fd = fopen(filename, "rb");
    if(fd == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    HEADER *h = extraiHeader(fd);//Extrai informacoes do header
    if(h->status == '0'){
        printf("\nArquivo de dados inconsistente\n");
        return;
    }
    int id;
    int64_t offset = 25; //Primeiro offset do arquivo
    bool flagFound = false, searchForId = false;
    for(int i = 0; i < numOfParameters; i++) //Id eh unico (chave primaria, se ele esta envolvido na busca eh parar mais cedo, economizando tempo)
        if(strcmp("id", fields[i]) == 0){
            searchForId = true;
            id = atoi(values[i]);
        }
    // if(searchForId){
    //     PLAYER *p = playerFromBin(fd, indexSearch(globalIndex, id));
    //     playerPrint(p);
    //     playerFree(&p);
    //     return;
    // }
    while(h->offset > offset){ //Itera sobre o arquivo
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
    if(!flagFound) //Verifica se algum registro foi encontrado
        printf("Registro inexistente.\n\n");
    fclose(fd);
    free(h);
}

INDEX* createIndex(FILE* bin, char* indexName){
    INDEX* index = makeIndex(bin);
    writeIndex(index, indexName);
    return index;
}


void delete(FILE* data, INDEX* index, int numOfParameters, char** fields, char** values){
    if(data == NULL || index == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    HEADER* h =  extraiHeader(data);
    int id;
    uint64_t offset = 25;
    bool flagFound = false, searchForId = false;
    for(int i = 0; i < numOfParameters; i++) //Id eh unico (chave primaria, se ele esta envolvido na busca eh parar mais cedo, economizando tempo)
        if(strcmp("id", fields[i]) == 0){
            searchForId = true;
            id = atoi(values[i]);
        }
    if(searchForId){
        offset = indexSearch(index, id);
        PLAYER *p = playerFromBin(data, offset);
        indexRemove(index, id); 
        removeInDisk(data, h, offset);
        return;
    }
    while(h->offset > offset){ //Itera sobre o arquivo
        PLAYER *p = playerFromBin(data, offset);
        if(checkPlayer(p, numOfParameters, fields, values)){
            indexRemove(index, p->id);
            removeInDisk(data, h, offset);
            flagFound = true;
        }
        offset += playerTamanho(p);
        playerFree(&p);
    }
    
}







//Printa todo o arquivo de dados
void selectAll(char* filename){
    FILE* fd =  fopen(filename, "rb");
    if(fd == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    bool flag =  false;
    HEADER *h = extraiHeader(fd); //Extrai informacoes do header
    if(h->status == '0'){
        printf("\nArquivo de dados inconsistente\n");
        return;
    }
    int64_t offset = 25;
    while(h->offset > offset){ //Itera sobre o arquivo
        PLAYER *p = playerFromBin(fd, offset);
        if(p->status == '0'){
            playerPrint(p);
            flag = true;
        }
        offset += playerTamanho(p);
        playerFree(&p);
    }
    if(!flag) //Verifica se algum registro foi encontrado
        printf("Registro inexistente.\n\n");
    fclose(fd);
    free(h);
    
}

// Função para criar uma tabela a partir de um arquivo CSV
void createTable(char* srcName, char* destName){
    if(fopen(srcName, "rb") == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    csvToBin(srcName, destName);
    // if(globalIndex == NULL){
    //     FILE *fd = fopen(destName, "rb+");
    //     globalIndex = createIndex(fd);
    //     fclose(fd);
    // }
}
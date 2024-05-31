#include"interface.h"
/*
================================================
Arquivo fonte da interface
================================================
*/

//Printa os registros que batem com as condicoes
//Recebe 2 arrays de strings, 1 com os campos a serem comparados e outro com os valores
//Os arrays devem ser pareados (campo[i] corresponde a valor[i])
void selectWhere(FILE* fd, HEADER* h, int numOfParameters, char** fields, char** values){
//Extrai informacoes do header
    if(h->status == '0'){
        printf("Falha no processamento do arquivo.\n");
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
        PLAYER *p = playerFromBin(fd, NO_SEEK);
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

INDEX* createIndex(FILE* bin, HEADER* h, char* indexName){
    INDEX* index = makeIndex(bin, h);
    writeIndex(index, indexName);
    return index;
}


void delete(FILE* data, HEADER* h, INDEX* index, int numOfParameters, char** fields, char** values){
    FILE *log = fopen("log.log", "w");
    int id;
    int64_t offset = 25;
    bool flagFound = false, searchForId = false;
    for(int i = 0; i < numOfParameters; i++) //Id eh unico (chave primaria, se ele esta envolvido na busca eh parar mais cedo, economizando tempo)
        if(strcmp("id", fields[i]) == 0){
            searchForId = true;
            id = atoi(values[i]);
            if(id == 222537)
                offset = 3;
        }
    if(searchForId){
        offset = indexSearch(index, id);
        if(offset == -1){
            //printf("Registro inexistente.\n\n");
            return;
        }

        indexRemove(index, id); 
        removeInDisk(data, h, offset);
        return;
    }
    fseek(data, 25, SEEK_SET);
    while(h->offset > offset){ //Itera sobre o arquivo
        PLAYER *p = playerFromBin(data, NO_SEEK); 
        if(p->id == 222537)    
            fprintf(log, "player read : id = %d offset = %ld\n", p->id, offset);
        fprintf(log, "player read : id = %d offset = %ld\n", p->id, offset);
        fflush(log);
        if(checkPlayer(p, numOfParameters, fields, values)){
            indexRemove(index, p->id);
            removeInDisk(data, h, offset);
            flagFound = true;
            fseek(data, offset + playerTamanho(p), SEEK_SET);
        }
        offset += playerTamanho(p);
        playerFree(&p);
    }
    if(!flagFound) //Verifica se algum registro foi encontrado
        printf("Registro inexistente.\n\n");
    fflush(data);
}







//Printa todo o arquivo de dados
void selectAll(FILE* fd, HEADER* h){
    if(fd == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    if(h->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    bool flag =  false;
    int64_t offset = 25;
    while(h->offset > offset){ //Itera sobre o arquivo
        PLAYER *p = playerFromBin(fd, NO_SEEK);
        if(p->status == '0'){
            playerPrint(p);
            flag = true;
        }
        offset += playerTamanho(p);
        playerFree(&p);
    }
    if(!flag) //Verifica se algum registro foi encontrado
        printf("Registro inexistente.\n\n");
    
}

// Função para criar uma tabela a partir de um arquivo CSV
void createTable(FILE* csv, FILE* bin){
    if(csv == NULL || bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;       
    }
    csvToBin(csv, bin);
}



void logList(FILE* bin, HEADER *h, char *name){
    FILE *log = fopen(name, "w");
    int64_t offset = h->topo;
    int i = 0;
    PLAYER *current = playerFromBin(bin, offset);
    while(offset != -1){
        fprintf(log, "[%d] off = %lx / id = %d / nome = %s / tamanho = %d\n", i++,
         offset, current->id, current->nome, current->tamanho);
        offset = current->prox;
        playerFree(&current);
        current = playerFromBin(bin,offset);
    }
    fclose(log);
}
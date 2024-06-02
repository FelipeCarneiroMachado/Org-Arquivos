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
    while(h->offset > offset){ //Itera sobre o arquivo
        PLAYER *p = playerFromBin(fd, NO_SEEK);
        if(checkPlayer(p, numOfParameters, fields, values)){
            playerPrint(p);
            flagFound = true;
            if(searchForId)
                break;
        }
        offset += playerTamanho(p, true);
        playerFree(&p);
    }
    if(!flagFound) //Verifica se algum registro foi encontrado
        printf("Registro inexistente.\n\n");
}

INDEX* createIndex(FILE* bin, HEADER* h, char* indexName){
    INDEX* index = makeIndex(bin, h); //Cria o indice em ram
    writeIndex(index, indexName); //Escreve em disco o indice
    return index;
}

//Remove do arquivo de daos todos os registros que se encaixem nos parametros estipulados
//Recebe 2 arrays de strings, 1 com os campos a serem comparados e outro com os valores
//Os arrays devem ser pareados (campo[i] corresponde a valor[i])
void delete(FILE* data, HEADER* h, INDEX* index, int numOfParameters, char** fields, char** values){
    //Declaracao de variaveis
    FILE *log = fopen("log.log", "w");
    int id;
    int64_t offset = 25;
    bool flagFound = false, searchForId = false; //Flags


    for(int i = 0; i < numOfParameters; i++) //Id eh unico (chave primaria, se ele esta envolvido na busca eh parar mais cedo, economizando tempo)
        if(strcmp("id", fields[i]) == 0){
            searchForId = true;
            id = atoi(values[i]); //Determina se ha busca por id e determina o id
        }

    //Busca por ID, utiliza o indice
    if(searchForId){
        offset = indexSearch(index, id);
        if(offset == -1){//Caso nao ache
            //printf("Registro inexistente.\n\n");
            return;
        }
        indexRemove(index, id); 
        removeInDisk(data, h, offset);
        return;
    }

    //Busca sequencial no arquivo de dados
    fseek(data, 25, SEEK_SET);
    while(h->offset > offset){ //Itera sobre o arquivo
        PLAYER *p = playerFromBin(data, NO_SEEK);
        fprintf(log, "read id = %d / off = %ld / size = %d", p->id, offset, p->tamanho);
        fflush(log); //Extrai player do offset atual (sem fseek)
        //Verificacao de parametros e remocao
        if(checkPlayer(p, numOfParameters, fields, values)){
            indexRemove(index, p->id);
            removeInDisk(data, h, offset);
            flagFound = true;
            fseek(data, offset + playerTamanho(p, true), SEEK_SET);
        }
        offset += playerTamanho(p, true);
        playerFree(&p);
    }
    // if(!flagFound) //Verifica se algum registro foi encontrado
    //     printf("Registro inexistente.\n\n");
    //Salva as alteracoes feitas em disco
    //Necessario para nao interferir com outras remocoes sucessoras
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
        offset += playerTamanho(p, true);
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
    fclose(bin);
    fclose(log);
}
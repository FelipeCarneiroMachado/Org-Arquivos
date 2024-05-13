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
    int64_t offset = 25; //Primeiro offset do arquivo
    bool flagFound = false, searchForId = false;
    for(int i = 0; i < numOfParameters; i++) //Id eh unico (chave primaria, se ele esta envolvido na busca eh parar mais cedo, economizando tempo)
        if(strcmp("id", fields[i]) == 0)
            searchForId = true;
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
}
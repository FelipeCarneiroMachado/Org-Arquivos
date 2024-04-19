#include"player.h"
/*
================================================
Arquivo fonte da struct PLAYER
Aqui estao definidas todas as funcoes para manipulacao 
dessa struct, incluindo conversao entre formatos (texto, binario no arquivo, struct)
================================================
*/

//Funcoes para uso externo

PLAYER* playerInit(){
    //Aloca e inicializa a struct com valores propicios
    PLAYER* newPlayer = (PLAYER*)malloc(sizeof(PLAYER));
    newPlayer->status = '0';
    newPlayer->prox = -1;
    newPlayer->tamanho = 0;
    newPlayer->id = -1;
    newPlayer->idade = -1;
    newPlayer->clube = NULL;
    newPlayer->pais = NULL;
    newPlayer->nome = NULL;
    newPlayer->clubeLen = 0;
    newPlayer->nomeLen = 0;
    newPlayer->paisLen = 0;
    return newPlayer;
}

void playerSetIdade(PLAYER* p, int idade){
    if(idade == 0)  //Lida com o caso nulo
        idade =  -1;
    p->idade = idade;

}
void playerSetId(PLAYER* p, int id){
    p->id = id;
}
//Todas as funcoes que setam campos com strings criam copias das strings passadas como argumentos
//nao eh preocupacao do usuario deste header se preocupar com essa logistica
void playerSetNome(PLAYER* p, char* nome){
    int len = strlen(nome);
    if(len == 0) //caso nulo
        return;
    char* nomeCp = (char*)malloc(len + 1);
    strcpy(nomeCp, nome);
    p->nome = nomeCp;
    p->nomeLen = len;
}
void playerSetClube(PLAYER* p, char* clube){
    int len = strlen(clube); //caso nulo
    if(len == 0)
        return;
    char* clubeCp = (char*)malloc(len + 1);
    strcpy(clubeCp, clube);
    p->clube = clubeCp;
    p->clubeLen = len;
}
void playerSetPais(PLAYER* p, char* pais){
    int len = strlen(pais); //caso nulo
    if(len == 0)
        return;
    char* paisCp = (char*)malloc(len + 1);
    strcpy(paisCp, pais);
    p->pais = paisCp;
    p->paisLen = len;
}
int playerTamanho(PLAYER* p){
    int size = 33; //tamanho fixo = 1 + 8 + 6 * 4
    size += p->clubeLen + p->nomeLen + p->paisLen;
    p->tamanho = size; //Retorna e seta o tamanho
    return size;
}
void playerPrint(PLAYER *p){
    if(p->nome != NULL)
        printf("Nome do Jogador: %s\n", p->nome);
    else
        printf("Nome do Jogador: SEM DADO\n");
    if(p->pais != NULL)
        printf("Nacionalidade do Jogador: %s\n", p->pais);
    else
        printf("Nacionalidade do Jogador: SEM DADO\n");
    if(p->clube != NULL)
        printf("Clube do Jogador: %s\n\n", p->clube);
    else
        printf("Clube do Jogador: SEM DADO\n\n");
    }

void playerFree(PLAYER** p){
    //Liberacao de memoria, a struct deve ser passada por referencia
    if((*p)->nome != NULL)
        free((*p)->nome);
    if((*p)->clube != NULL)
        free((*p)->clube);
    if((*p)->pais != NULL)
        free((*p)->pais);
    free(*p);
    *p = NULL;
}


bool checkPlayer(PLAYER* p, int numOfParameters, char** fields, char** values){
    //Compara os campos de uma struct com uma lista de valores
    //Recebe 2 arrays de strings, 1 com os campos a serem comparados e outro com os valores
    //Os arrays devem ser pareados (campo[i] corresponde a valor[i])
    if(p->status == '1')//Se o registro esta logicamente removido, retorna falso
        return false;
    for(int i = 0; i < numOfParameters; i++){
        if(strcmp(fields[i], "id") == 0){
            if(atoi(values[i]) != p->id)
                return false; 
        }
        if(strcmp(fields[i], "idade") == 0)
            if(atoi(values[i]) != p->idade)
                return false;
        if(strcmp(fields[i], "nome") == 0){
            if(p->nome == NULL)
                return false;
            if(strcmp(values[i], p->nome) != 0)
                return false;
        }
        if(strcmp(fields[i], "nacionalidade") == 0){
            if(p->pais == NULL)
                return false;
            if(strcmp(values[i], p->pais) != 0)
                return false;
        }
        if(strcmp(fields[i], "nomeClube") == 0){
            if(p->clube == NULL)
                return false;
            if(strcmp(values[i], p->clube) != 0)
                return false;
        }
    }
    return true;
}
PLAYER* parseLine(char *line){
    //A partir de uma linha do .csv, gera uma struct com as informcoes
    PLAYER* newPlayer = playerInit();
    char iterChar, tempStr[64];
    int i = 0, j = 0;
    while((iterChar = line[i]) != ','){
        //loop para ler id
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetId(newPlayer, atoi(tempStr));
    j = 0; i++;
    memset(tempStr, 0, 64);
    while((iterChar = line[i]) != ','){
        //loop para ler idade
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetIdade(newPlayer, atoi(tempStr));
    j = 0; i++;
    memset(tempStr, 0, 64);
    while((iterChar = line[i]) != ','){
        //loop para ler nome
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetNome(newPlayer, tempStr);
    j = 0; i++;
    memset(tempStr, 0, 64);
    while((iterChar = line[i]) != ','){
        //loop para ler pais
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetPais(newPlayer, tempStr);
    j = 0; i++;
    memset(tempStr, 0, 64);
    while((iterChar = line[i]) != '\n'){
        //loop para ler clube
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetClube(newPlayer, tempStr);
    j = 0; i++;
    playerTamanho(newPlayer);
    return newPlayer;
}





PLAYER* playerFromBin(FILE*fd, uint64_t offset){
    //Extrai uma struct de um binario no offset parametro
    char regBuffer[128];
    char *tempPtr;
    char fieldBuffer[64]; //buffers teporarios
    PLAYER* p = playerInit();
    if(ftell(fd) != offset) //Se o arquivo ja esta no offset, evita um seek
        fseek(fd, offset, SEEK_SET);
    fread(regBuffer, 1, 5, fd); //Primeiro fread = status e tamanho
    p->status = regBuffer[0];
    tempPtr = regBuffer + 1;
    memcpy(&(p->tamanho), tempPtr, 4); 
    fread(&regBuffer, 1, p->tamanho - 5, fd); //fread no resto do registro
    tempPtr = regBuffer;
    memcpy(&p->prox, tempPtr, 8);
    tempPtr += 8;
    memcpy(&(p->id), tempPtr, 4);
    tempPtr += 4;
    memcpy(&(p->idade), tempPtr, 4);
    tempPtr += 4;
    memcpy(&(p->nomeLen), tempPtr, 4);
    int off = 20 + p->nomeLen;
    tempPtr += 4;
    memset(fieldBuffer, 0, 64);
    memcpy(fieldBuffer, tempPtr, p->nomeLen);
    tempPtr += p->nomeLen;
    playerSetNome(p, fieldBuffer);
    memcpy(&(p->paisLen), tempPtr, 4);
    tempPtr += 4;
    memset(fieldBuffer, 0, 64);
    memcpy(fieldBuffer, tempPtr, p->paisLen);
    tempPtr += p->paisLen;
    playerSetPais(p, fieldBuffer);
    memcpy(&(p->clubeLen), tempPtr, 4);
    tempPtr += 4;
    memset(fieldBuffer, 0, 64);
    memcpy(fieldBuffer, tempPtr, p->clubeLen);
    tempPtr += p->clubeLen;
    off += 4 + p->paisLen;
    playerSetClube(p, fieldBuffer);
    return p;
}
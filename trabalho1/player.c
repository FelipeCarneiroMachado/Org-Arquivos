#include"player.h"



//Funcoes internas utilitarias



//Funcoes para uso externo
PLAYER* playerInit(){
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
    if(idade == 0)  
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
    if(len == 0)
        return;
    char* nomeCp = (char*)malloc(len + 1);
    strcpy(nomeCp, nome);
    p->nome = nomeCp;
    p->nomeLen = len;
}
void playerSetClube(PLAYER* p, char* clube){
    int len = strlen(clube);
    if(len == 0)
        return;
    char* clubeCp = (char*)malloc(len + 1);
    strcpy(clubeCp, clube);
    p->clube = clubeCp;
    p->clubeLen = len;
}
void playerSetPais(PLAYER* p, char* pais){
    int len = strlen(pais);
    if(len == 0)
        return;
    char* paisCp = (char*)malloc(len + 1);
    strcpy(paisCp, pais);
    p->pais = paisCp;
    p->paisLen = len;
}
int playerTamanho(PLAYER* p){
    int size = 33; //6 campos= 4 * 6 + 8 = 33 bytes
    size += p->clubeLen + p->nomeLen + p->paisLen;
    p->tamanho = size;
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
    if(p->status == '1')
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
    PLAYER* newPlayer = playerInit();
    char iterChar, tempStr[50];
    int i = 0, j = 0;
    while((iterChar = line[i]) != ','){
        //loop para ler id
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetId(newPlayer, atoi(tempStr));
    j = 0; i++;
    memset(tempStr, 0, 50);
    while((iterChar = line[i]) != ','){
        //loop para ler idade
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetIdade(newPlayer, atoi(tempStr));
    j = 0; i++;
    memset(tempStr, 0, 50);
    while((iterChar = line[i]) != ','){
        //loop para ler nome
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetNome(newPlayer, tempStr);
    j = 0; i++;
    memset(tempStr, 0, 50);
    while((iterChar = line[i]) != ','){
        //loop para ler pais
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetPais(newPlayer, tempStr);
    j = 0; i++;
    memset(tempStr, 0, 50);
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
    char regBuffer[120];
    char fieldBuffer[50]; //buffers teporarios
    PLAYER* p = playerInit();
    if(ftell(fd) != offset)
        fseek(fd, offset, SEEK_SET);
    fread(regBuffer, 1, 5, fd);
    p->status = regBuffer[0];
    slice(fieldBuffer, regBuffer, 1, 5);
    memcpy(&(p->tamanho), fieldBuffer, 4);
    fread(&regBuffer, 1, p->tamanho - 5, fd);
    slice(fieldBuffer, regBuffer, 0, 8);
    memcpy(&p->prox, fieldBuffer, 8);
    slice(fieldBuffer, regBuffer, 8, 12);
    memcpy(&(p->id), fieldBuffer, 4);
    slice(fieldBuffer, regBuffer, 12, 16);
    memcpy(&(p->idade), fieldBuffer, 4);
    slice(fieldBuffer, regBuffer, 16, 20);
    memcpy(&(p->nomeLen), fieldBuffer, 4);
    int off = 20 + p->nomeLen;
    slice(fieldBuffer, regBuffer, 20, off);
    playerSetNome(p, fieldBuffer);
    //strcpy(p->nome, fieldBuffer);
    slice(fieldBuffer, regBuffer, off, off + 4);
    memcpy(&(p->paisLen), fieldBuffer, 4);
    slice(fieldBuffer, regBuffer, off + 4, off + 4 + p->paisLen);
    playerSetPais(p, fieldBuffer);
    //strcpy(p->pais, fieldBuffer);
    off += 4 + p->paisLen;
    slice(fieldBuffer, regBuffer, off, off + 4);
    memcpy(&(p->clubeLen), fieldBuffer, 4);
    slice(fieldBuffer, regBuffer, off + 4, off + 4 + p->clubeLen);
    playerSetClube(p, fieldBuffer);
    //strcpy(p->clube, fieldBuffer);
    return p;
}
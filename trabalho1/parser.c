#include"parser.h"



// PLAYER* parseLine(char* line){
//     char del[2] = ",";
//     char* token = strtok(line, del);
//     PLAYER* newPlayer = playerInit();
//     newPlayer->id = atoi(token);
//     if(strcmp(token = strtok(NULL, del), "") != 0)
//         newPlayer->idade = atoi(token);
//     if(strcmp(token = strtok(NULL, del), "") != 0)
//         playerSetNome(newPlayer, token);
//     if(strcmp(token = strtok(NULL, del), "") != 0)
//         playerSetPais(newPlayer, token);
//     if(strcmp(token = strtok(NULL, del), "") != 0)
//         playerSetClube(newPlayer, token);
//     return newPlayer;
// }

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

    return newPlayer;
}



void slice(char * dest, char* src, int start, int end){
    int k = 0;
    for(int i = start; i < end; i++){
        dest[k++] = src[i];
    }
    dest[k] = '\0';
}

PLAYER* playerFromBin(FILE*fd, uint64_t offset){
    char regBuffer[120];
    char fieldBuffer[50];
    PLAYER* p = playerInit();
    fseek(fd, offset, SEEK_SET);
    fread(&regBuffer, 1, 120, fd);
    int8_t status = regBuffer[0];
    if(status != '0')
        return NULL;
    slice(fieldBuffer, regBuffer, 1, 5);
    int32_t regSize;
    memcpy(&regSize, fieldBuffer, 4);
    slice(fieldBuffer, regBuffer, 5, 13);
    int64_t prox;
    memcpy(&prox, fieldBuffer, 8);
    slice(fieldBuffer, regBuffer, 13, 17);
    memcpy(&(p->id), fieldBuffer, 4);
    slice(fieldBuffer, regBuffer, 17, 21);
    memcpy(&(p->idade), fieldBuffer, 4);
    slice(fieldBuffer, regBuffer, 21, 25);
    memcpy(&(p->nomeLen), fieldBuffer, 4);
    int off = 25 + p->nomeLen;
    slice(fieldBuffer, regBuffer, 25, off);
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
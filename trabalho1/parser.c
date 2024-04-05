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

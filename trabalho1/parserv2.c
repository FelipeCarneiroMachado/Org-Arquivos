#include"parser.h"


PLAYER* parseLine(char* line){
    char del[1] = ",";
    char* token = strtok(line, del);
    PLAYER* newPlayer = playerInit();
    newPlayer->id = atoi(token);
    if(strcmp(token = strtok(NULL, del), "") != 0)
        newPlayer->idade = atoi(token);
    if(strcmp(token = strtok(NULL, del), "") != 0)
        playerSetNome(newPlayer, token);
    if(strcmp(token = strtok(NULL, del), "") != 0)
        playerSetPais(newPlayer, token);
    if(strcmp(token = strtok(NULL, del), "") != 0)
        playerSetClube(newPlayer, token);
    return newPlayer;
}
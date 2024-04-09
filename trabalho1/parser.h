#ifndef H_PARSE
    //Este arquivo contem as funcoes para transformar o texto do .csv em informacao binaria
    #define H_PARSE
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include"player.h"
    PLAYER *parseLine(char *line);
    PLAYER* playerFromBin(FILE*fd, uint64_t offset);




#endif
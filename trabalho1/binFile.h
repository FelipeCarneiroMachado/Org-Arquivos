#ifndef BIN_H
    //Este arquivo define as funcoes para manipular diretamente as informcoes do arquivo binario
    #define BIN_H
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include"player.h"
    #include"parser.h"
    void initFile(char* filename);
    void csvToBin(char* srcFile, char* destFile);
    void escreveRegistro(FILE* data, uint64_t offset, PLAYER* player);
    void setStatus(FILE *fd, uint8_t status);



#endif
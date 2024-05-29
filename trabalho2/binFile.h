/*
================================================
Arquivo interface para manipulacao de binario
================================================
*/

#ifndef BIN_H
    #define BIN_H
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include"player.h"
    #include"utils.h"
    //struct para o header do arquivo binario
    typedef struct _header{
        char status;
        int64_t topo;
        int64_t offset;
        int nReg;
        int nRem;
    }HEADER;
    void initFile(char* filename);
    HEADER* extraiHeader(FILE *fd);
    void csvToBin(char* srcFile, char* destFile);
    void escreveRegistro(FILE* data, uint64_t offset, PLAYER* player);
    void setStatus(FILE *fd, uint8_t status);
    void removeInDisk(FILE* bin, HEADER* h , uint64_t offset);
    void insertPlayer(FILE *bin, HEADER *h, PLAYER* p);


#endif
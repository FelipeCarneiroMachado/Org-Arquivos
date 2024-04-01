#ifndef BIN_H
    #define BIN_H
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include"player.h"
    #include"parser.h"
    void initFile();
    void escreveRegistro(FILE* data, uint64_t offset, PLAYER* player);



#endif
#ifndef INDEX_H
    #define INDEX_H
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include"player.h"
    #include"utils.h"
    #include"binFile.h"
    typedef struct index_{
        int8_t status;
        uint32_t nReg;
        uint32_t arrLen;
        struct data{
            uint32_t id;
            int64_t offset; 
        }*array;
    }INDEX;
    INDEX* makeIndex(FILE *fd, HEADER* h);
    int64_t indexSearch(INDEX* index, int id);
    void indexFree(INDEX** i);
    void writeIndex(INDEX* index, char* filename);
    void indexRemove(INDEX* index, int id);

#endif
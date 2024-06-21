#ifndef H_BTREE
    #define H_BTREE
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    typedef struct {
        char status;
        int noRaiz;
        int proxRRN;
        int nroChaves;
    }BT_HEADER;
    typedef struct{
        int alturaNo;
        int nroChaves;
        int *chaves;
        int *offsets;
        int *filhos;
        int RRN;
        int pai;
    }BT_NODE;
    typedef struct {
        int id;
        int64_t offset;
    }pair;



#endif
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
        int chaves[3];
        int64_t offsets[3];
        int filhos[4];
    }BT_NODE;
    typedef struct {
        int id;
        int64_t offset;
        int filho;
    }trio;

    BT_HEADER *bTreeInit(char *filename);
    void BT_writeHeader(BT_HEADER *bth, FILE* fd, bool seek);
    BT_HEADER* bt_headerRead(FILE *btFile);
    int64_t BT_search(BT_HEADER *bth, int id, FILE *btfile);
    bool valid(BT_HEADER *bth, FILE *fd);
    bool BT_printWrap(BT_HEADER *bth, FILE *fd);
    void printNormal(int RRN, FILE *fd);
    //void BT_print(int RRN, FILE *fd);
    void BT_insert(FILE *btFile, BT_HEADER *bth, trio reg);
    trio makeTrio(int id, int64_t off, int filho);
    void logNode(BT_NODE* btn);

#endif
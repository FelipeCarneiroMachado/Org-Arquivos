#ifndef H_PLAYER
    #define H_PLAYER
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    typedef struct _player PLAYER;
    PLAYER* playerInit();
    void playerSetIdade(PLAYER* p, int idade);
    void playerSetId(PLAYER* p, int id);
    void playerSetNome(PLAYER* p, char* nome);
    void playerSetClube(PLAYER* p, char* clube);
    void playerSetPais(PLAYER* p, char* pais);





#endif
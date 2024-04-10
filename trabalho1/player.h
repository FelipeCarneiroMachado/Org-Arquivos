#ifndef H_PLAYER
    //Este arquivo e tem como objetivo realizar o manejo da struct que possui as informcoes do jogador
    #define H_PLAYER
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    //definicao da struct que contem as informacoes do jogador
    struct _player{
        int id;
        int idade;
        char* nome;
        int nomeLen;
        char *pais;
        int paisLen;
        char* clube;
        int clubeLen;
    };
    typedef struct _player PLAYER;
    bool checkPlayer(PLAYER* p, int numOfParameters, char** fields, char** values);
    PLAYER* playerInit();
    void playerSetIdade(PLAYER* p, int idade);
    void playerSetId(PLAYER* p, int id);
    void playerSetNome(PLAYER* p, char* nome);
    void playerSetClube(PLAYER* p, char* clube);
    void playerSetPais(PLAYER* p, char* pais);
    size_t playerTamanho(PLAYER* p);
    void playerPrint(PLAYER *p);
    void playerFree(PLAYER** p);




#endif
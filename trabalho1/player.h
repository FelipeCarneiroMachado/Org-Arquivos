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
        char status;
        int64_t prox;
        int32_t tamanho;
        int id;
        int idade;
        int nomeLen;
        char* nome;
        int paisLen;
        char *pais;
        int clubeLen;
        char* clube;
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
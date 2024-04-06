#include"player.h"



//Funcoes internas utilitarias




//Funcoes para uso externo
PLAYER* playerInit(){
    PLAYER* newPlayer = (PLAYER*)malloc(sizeof(PLAYER));
    newPlayer->id = -1;
    newPlayer->idade = -1;
    newPlayer->clube = NULL;
    newPlayer->pais = NULL;
    newPlayer->nome = NULL;
    newPlayer->clubeLen = 0;
    newPlayer->nomeLen = 0;
    newPlayer->paisLen = 0;
    return newPlayer;
}

void playerSetIdade(PLAYER* p, int idade){
    p->idade = idade;
}
void playerSetId(PLAYER* p, int id){
    p->id = id;
}
//Todas as funcoes que setam campos com strings criam copias das strings passadas como argumentos
//nao eh preocupacao do usuario deste header se preocupar com essa logistica
void playerSetNome(PLAYER* p, char* nome){
    int len = strlen(nome);
    char* nomeCp = (char*)malloc(len + 1);
    strcpy(nomeCp, nome);
    p->nome = nomeCp;
    p->nomeLen = len;
}
void playerSetClube(PLAYER* p, char* clube){
    int len = strlen(clube);
    char* clubeCp = (char*)malloc(len + 1);
    strcpy(clubeCp, clube);
    p->clube = clubeCp;
    p->clubeLen = len;
}
void playerSetPais(PLAYER* p, char* pais){
    int len = strlen(pais);
    char* paisCp = (char*)malloc(len + 1);
    strcpy(paisCp, pais);
    p->pais = paisCp;
    p->paisLen = len;
}
size_t playerTamanho(PLAYER* p){
    int size = 28; //6 campos= 4 * 6 + 8 = 33 bytes
    size += p->clubeLen + p->nomeLen + p->paisLen;
    return size;
}
void playerPrint(PLAYER *p){
    printf("id %d\n idade %d\n nome e len %s %d\n pais e len %s %d\n clube e len %s %d", p->id, p->idade, p->nome, p->nomeLen, p->pais, p->paisLen, p->clube, p->clubeLen);
}
void playerFree(PLAYER** p){
    if((*p)->nome != NULL)
        free((*p)->nome);
    if((*p)->clube != NULL)
        free((*p)->clube);
    if((*p)->pais != NULL)
        free((*p)->pais);
    free(*p);
    *p = NULL;
}
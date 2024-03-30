#include"player.h"


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
//Funcoes internas utilitarias
char* stringInit(){
    //Inicializa uma string como "$"
    char *s = (char*)malloc(1);
    s[0] = '$';
    return s;
}



//Funcoes para uso externo
PLAYER* playerInit(){
    PLAYER* newPlayer = (PLAYER*)malloc(sizeof(PLAYER));
    newPlayer->id = -1;
    newPlayer->idade = -1;
    newPlayer->clube = stringInit();
    newPlayer->pais = stringInit();
    newPlayer->nome = stringInit();
    newPlayer->clubeLen = 1;
    newPlayer->nomeLen = 1;
    newPlayer->paisLen = 1;
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
    free(p->nome);
    p->nome = nomeCp;
    p->nomeLen = len;
}
void playerSetClube(PLAYER* p, char* clube){
    int len = strlen(clube);
    char* clubeCp = (char*)malloc(len + 1);
    strcpy(clubeCp, clube);
    free(p->clube);
    p->clube = clube;
    p->clubeLen = len;
}
void playerSetPais(PLAYER* p, char* pais){
    int len = strlen(pais);
    char* paisCp = (char*)malloc(len + 1);
    strcpy(paisCp, pais);
    free(p->pais);
    p->pais = pais;
    p->paisLen = len;
}

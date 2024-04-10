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
    if(idade == 0)  
        idade =  -1;
    p->idade = idade;

}
void playerSetId(PLAYER* p, int id){
    p->id = id;
}
//Todas as funcoes que setam campos com strings criam copias das strings passadas como argumentos
//nao eh preocupacao do usuario deste header se preocupar com essa logistica
void playerSetNome(PLAYER* p, char* nome){
    int len = strlen(nome);
    if(len == 0)
        return;
    char* nomeCp = (char*)malloc(len + 1);
    strcpy(nomeCp, nome);
    p->nome = nomeCp;
    p->nomeLen = len;
}
void playerSetClube(PLAYER* p, char* clube){
    int len = strlen(clube);
    if(len == 0)
        return;
    char* clubeCp = (char*)malloc(len + 1);
    strcpy(clubeCp, clube);
    p->clube = clubeCp;
    p->clubeLen = len;
}
void playerSetPais(PLAYER* p, char* pais){
    int len = strlen(pais);
    if(len == 0)
        return;
    char* paisCp = (char*)malloc(len + 1);
    strcpy(paisCp, pais);
    p->pais = paisCp;
    p->paisLen = len;
}
size_t playerTamanho(PLAYER* p){
    int size = 33; //6 campos= 4 * 6 + 8 = 33 bytes
    size += p->clubeLen + p->nomeLen + p->paisLen;
    return size;
}
void playerPrint(PLAYER *p){
    if(p->nome != NULL)
        printf("Nome do Jogador: %s\n", p->nome);
    else
        printf("Nome do Jogador: SEM DADO\n");
    if(p->pais != NULL)
        printf("Nacionalidade do Jogador: %s\n", p->pais);
    else
        printf("Nacionalidade do Jogador: SEM DADO\n");
    if(p->clube != NULL)
        printf("Clube do Jogador: %s\n\n", p->clube);
    else
        printf("Clube do Jogador: SEM DADO\n\n");
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
bool checkPlayer(PLAYER* p, int numOfParameters, char** fields, char** values){
    for(int i = 0; i < numOfParameters; i++){
        if(strcmp(fields[i], "id") == 0){
            if(atoi(values[i]) != p->id)
                return false; 
        }
        if(strcmp(fields[i], "idade") == 0)
            if(atoi(values[i]) != p->idade)
                return false;
        if(strcmp(fields[i], "nome") == 0)
            if(p->nome != NULL)
                if(strcmp(values[i], p->nome) != 0)
                    return false;
        if(strcmp(fields[i], "NACIONALIDADE") == 0)
            if(p->pais != NULL)
                if(strcmp(values[i], p->pais) != 0)
                    return false;
        if(strcmp(fields[i], "clube") == 0)
            if(p->clube != NULL)
                if(strcmp(values[i], p->clube) != 0)
                    return false;
    }
    return true;
}
#include "player.h" // Inclui o header player.h

// Funções internas utilitárias

// Função para extrair uma parte de uma string e armazenar em outra
void slice(char *dest, char *src, int start, int end) {
    int k = 0;

    for (int i = start; i < end; i++) {
        dest[k++] = src[i];
    }
    
    dest[k] = '\0';
}

// Funções para uso externo

// Inicializa um novo jogador
PLAYER* playerInit() {
    // Aloca memória para o jogador
    PLAYER* newPlayer = (PLAYER*)malloc(sizeof(PLAYER));
    // Inicializa os campos do jogador
    newPlayer->status = '0';
    newPlayer->prox = -1;
    newPlayer->tamanho = 0;
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

// Define a idade do jogador
void playerSetIdade(PLAYER* p, int idade) {
    if (idade == 0)
        idade = -1;
    p->idade = idade;
}

// Define o ID do jogador
void playerSetId(PLAYER* p, int id) {
    p->id = id;
}

// Define o nome do jogador
void playerSetNome(PLAYER* p, char* nome) {
    int len = strlen(nome);

    if (len == 0)
        return;
    
    char* nomeCp = (char*)malloc(len + 1);
    strcpy(nomeCp, nome);
    p->nome = nomeCp;
    p->nomeLen = len;
}

// Define o clube do jogador
void playerSetClube(PLAYER* p, char* clube) {
    int len = strlen(clube);

    if (len == 0)
        return;
    
    char* clubeCp = (char*)malloc(len + 1);
    strcpy(clubeCp, clube);
    p->clube = clubeCp;
    p->clubeLen = len;
}

// Define o país do jogador
void playerSetPais(PLAYER* p, char* pais) {
    int len = strlen(pais);

    if (len == 0)
        return;
    
    char* paisCp = (char*)malloc(len + 1);
    strcpy(paisCp, pais);
    p->pais = paisCp;
    p->paisLen = len;
}

// Retorna o tamanho do jogador
int playerTamanho(PLAYER* p) {
    int size = 33; // Tamanho dos campos fixos

    size += p->clubeLen + p->nomeLen + p->paisLen; // Tamanho dos campos variáveis
    p->tamanho = size;

    return size;
}

// Imprime os detalhes do jogador
void playerPrint(PLAYER *p) {
    if (p->nome != NULL)
        printf("Nome do Jogador: %s\n", p->nome);
    else
        printf("Nome do Jogador: SEM DADO\n");
    if (p->pais != NULL)
        printf("Nacionalidade do Jogador: %s\n", p->pais);
    else
        printf("Nacionalidade do Jogador: SEM DADO\n");
    if (p->clube != NULL)
        printf("Clube do Jogador: %s\n\n", p->clube);
    else
        printf("Clube do Jogador: SEM DADO\n\n");
}

// Libera a memória alocada para o jogador
void playerFree(PLAYER** p) {
    if ((*p)->nome != NULL)
        free((*p)->nome);
    if ((*p)->clube != NULL)
        free((*p)->clube);
    if ((*p)->pais != NULL)
        free((*p)->pais);
    
    free(*p);
    *p = NULL;
}

// Verifica se os campos do jogador correspondem aos valores fornecidos
bool checkPlayer(PLAYER* p, int numOfParameters, char** fields, char** values) {
    if (p->status == '1')
        return false;
    
    for (int i = 0; i < numOfParameters; i++) {
        if (strcmp(fields[i], "id") == 0) {
            if (atoi(values[i]) != p->id)
                return false;
        }

        if (strcmp(fields[i], "idade") == 0)
            if (atoi(values[i]) != p->idade)
                return false;
        
        if (strcmp(fields[i], "nome") == 0) {
            if (p->nome == NULL)
                return false;
        
            if (strcmp(values[i], p->nome) != 0)
                return false;
        }

        if (strcmp(fields[i], "nacionalidade") == 0) {
            if (p->pais == NULL)
                return false;
        
            if (strcmp(values[i], p->pais) != 0)
                return false;
        }

        if (strcmp(fields[i], "nomeClube") == 0) {
            if (p->clube == NULL)
                return false;
            if (strcmp(values[i], p->clube) != 0)
                return false;
        }

    }

    return true;
}

// Função para analisar uma linha de entrada e criar um jogador
PLAYER* parseLine(char *line) {
    PLAYER* newPlayer = playerInit(); // Inicializa um novo jogador
    char iterChar, tempStr[50];
    int i = 0, j = 0;

    // Loop para ler o ID
    while ((iterChar = line[i]) != ',') {
        tempStr[j++] = iterChar;
        i++;
    }
    
    tempStr[j] = '\0';
    playerSetId(newPlayer, atoi(tempStr)); // Define o ID do jogador
    j = 0; i++;
    memset(tempStr, 0, 50);

    // Loop para ler a idade
    while ((iterChar = line[i]) != ',') {
        tempStr[j++] = iterChar;
        i++;
    }

    tempStr[j] = '\0';
    playerSetIdade(newPlayer, atoi(tempStr)); // Define a idade do jogador
    j = 0; i++;
    memset(tempStr, 0, 50);

    // Loop para ler o nome
    while ((iterChar = line[i]) != ',') {
        tempStr[j++] = iterChar;
        i++;
    }

    tempStr[j] = '\0';
    playerSetNome(newPlayer, tempStr); // Define o nome do jogador
    j = 0; i++;
    memset(tempStr, 0, 50);

    // Loop para ler o país
    while ((iterChar = line[i]) != ',') {
        tempStr[j++] = iterChar;
        i++;
    }

    tempStr[j] = '\0';
    playerSetPais(newPlayer, tempStr); // Define o país do jogador
    j = 0; i++;
    memset(tempStr, 0, 50);

    // Loop para ler o clube
    while ((iterChar = line[i]) != '\n') {
        tempStr[j++] = iterChar;
        i++;
    }

    tempStr[j] = '\0';
    playerSetClube(newPlayer, tempStr); // Define o clube do jogador
    j = 0; i++;
    playerTamanho(newPlayer); // Calcula o tamanho do jogador

    return newPlayer;
}

// Cria um jogador a partir de um arquivo binário
PLAYER* playerFromBin(FILE* fd, uint64_t offset) {
    char regBuffer[120];
    char fieldBuffer[50]; // Buffers temporários

    PLAYER* p = playerInit(); // Inicializa um novo jogador

    if (ftell(fd) != offset)
        fseek(fd, offset, SEEK_SET);
    
    fread(regBuffer, 1, 5, fd); // Lê os primeiros 5 bytes do registro
    p->status = regBuffer[0]; // Define o status do jogador

    slice(fieldBuffer, regBuffer, 1, 5);
    memcpy(&(p->tamanho), fieldBuffer, 4); // Copia o tamanho do registro
    fread(&regBuffer, 1, p->tamanho - 5, fd); // Lê o restante do registro
    
    slice(fieldBuffer, regBuffer, 0, 8);
    memcpy(&p->prox, fieldBuffer, 8); // Copia o próximo offset

    slice(fieldBuffer, regBuffer, 8, 12);
    memcpy(&(p->id), fieldBuffer, 4); // Copia o ID do jogador

    slice(fieldBuffer, regBuffer, 12, 16);
    memcpy(&(p->idade), fieldBuffer, 4); // Copia a idade do jogador

    slice(fieldBuffer, regBuffer, 16, 20);
    memcpy(&(p->nomeLen), fieldBuffer, 4); // Copia o comprimento do nome

    int off = 20 + p->nomeLen;

    slice(fieldBuffer, regBuffer, 20, off);
    playerSetNome(p, fieldBuffer); // Define o nome do jogador

    slice(fieldBuffer, regBuffer, off, off + 4);
    memcpy(&(p->paisLen), fieldBuffer, 4); // Copia o comprimento do país
    slice(fieldBuffer, regBuffer, off + 4, off + 4 + p->paisLen);
    playerSetPais(p, fieldBuffer); // Define o país do jogador
    off += 4 + p->paisLen;

    slice(fieldBuffer, regBuffer, off, off + 4);
    memcpy(&(p->clubeLen), fieldBuffer, 4); // Copia o comprimento do clube
    slice(fieldBuffer, regBuffer, off + 4, off + 4 + p->clubeLen);
    playerSetClube(p, fieldBuffer); // Define o clube do jogador

    return p;
}

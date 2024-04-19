#include "parser.h"

// Função para analisar uma linha e criar um novo jogador com os dados fornecidos
PLAYER* parseLine(char *line) {
    // Inicializa um novo jogador
    PLAYER* newPlayer = playerInit();
    // Variáveis temporárias para armazenar caracteres e strings
    char iterChar, tempStr[50];
    int i = 0, j = 0;
    
    // Loop para ler o ID do jogador
    while ((iterChar = line[i]) != ',') {
        tempStr[j++] = iterChar;
        i++;
    }

    tempStr[j] = '\0';
    playerSetId(newPlayer, atoi(tempStr)); // Define o ID do jogador
    j = 0;
    i++;

    // Limpa o buffer temporário
    memset(tempStr, 0, 50);

    // Loop para ler a idade do jogador
    while ((iterChar = line[i]) != ',') {
        tempStr[j++] = iterChar;
        i++;
    }

    tempStr[j] = '\0';
    playerSetIdade(newPlayer, atoi(tempStr)); // Define a idade do jogador
    j = 0;
    i++;

    // Limpa o buffer temporário
    memset(tempStr, 0, 50);

    // Loop para ler o nome do jogador
    while ((iterChar = line[i]) != ',') {
        tempStr[j++] = iterChar;
        i++;
    }

    tempStr[j] = '\0';
    playerSetNome(newPlayer, tempStr); // Define o nome do jogador
    j = 0;
    i++;

    // Limpa o buffer temporário
    memset(tempStr, 0, 50);

    // Loop para ler o país do jogador
    while ((iterChar = line[i]) != ',') {
        tempStr[j++] = iterChar;
        i++;
    }

    tempStr[j] = '\0';
    playerSetPais(newPlayer, tempStr); // Define o país do jogador
    j = 0;
    i++;

    // Limpa o buffer temporário
    memset(tempStr, 0, 50);

    // Loop para ler o clube do jogador
    while ((iterChar = line[i]) != '\n') {
        tempStr[j++] = iterChar;
        i++;
    }

    tempStr[j] = '\0';
    playerSetClube(newPlayer, tempStr); // Define o clube do jogador
    j = 0;
    i++;

    // Calcula o tamanho do registro do jogador
    playerTamanho(newPlayer);

    // Retorna o novo jogador
    return newPlayer;
}

// Função para extrair uma substring de uma string
void slice(char *dest, char *src, int start, int end) {
    int k = 0;
    for (int i = start; i < end; i++) {
        dest[k++] = src[i];
    }
    
    dest[k] = '\0'; // Adiciona o terminador nulo
}

// Função para criar um jogador a partir de um arquivo binário
PLAYER* playerFromBin(FILE *fd, uint64_t offset) {
    // Buffer para armazenar o registro do jogador
    char regBuffer[120];
    // Buffer para armazenar campos individuais do registro
    char fieldBuffer[50];
    // Inicializa um novo jogador
    PLAYER *p = playerInit();
    // Move o cursor do arquivo para o deslocamento fornecido
    fseek(fd, offset, SEEK_SET);
    // Lê o registro do jogador do arquivo binário para o buffer
    fread(&regBuffer, 1, 120, fd);
    // Extrai o status do jogador do buffer
    int8_t status = regBuffer[0];
    p->status = regBuffer[0];

    // Extrai e define os campos do jogador a partir do buffer
    slice(fieldBuffer, regBuffer, 1, 5);
    memcpy(&(p->tamanho), fieldBuffer, 4);

    slice(fieldBuffer, regBuffer, 5, 13);
    memcpy(&p->prox, fieldBuffer, 8);

    slice(fieldBuffer, regBuffer, 13, 17);
    memcpy(&(p->id), fieldBuffer, 4);

    slice(fieldBuffer, regBuffer, 17, 21);
    memcpy(&(p->idade), fieldBuffer, 4);

    slice(fieldBuffer, regBuffer, 21, 25);
    memcpy(&(p->nomeLen), fieldBuffer, 4);
    int off = 25 + p->nomeLen;

    slice(fieldBuffer, regBuffer, 25, off);
    playerSetNome(p, fieldBuffer);

    slice(fieldBuffer, regBuffer, off, off + 4);
    memcpy(&(p->paisLen), fieldBuffer, 4);

    slice(fieldBuffer, regBuffer, off + 4, off + 4 + p->paisLen);
    playerSetPais(p, fieldBuffer);
    off += 4 + p->paisLen;

    slice(fieldBuffer, regBuffer, off, off + 4);
    memcpy(&(p->clubeLen), fieldBuffer, 4);

    slice(fieldBuffer, regBuffer, off + 4, off + 4 + p->clubeLen);
    playerSetClube(p, fieldBuffer);

    // Retorna o jogador criado
    return p;
}

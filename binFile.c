#include "binFile.h"

// Funções internas

// Copia uma parte da string de origem para a string de destino
void slicea(char *dest, char *src, int start, int end) {
    int k = 0;

    for (int i = start; i < end; i++) {
        dest[k++] = src[i];
    }
    dest[k] = '\0';
}

// Altera o status do arquivo 
void setStatus(FILE *fd, uint8_t status) {
    fseek(fd, 0, SEEK_SET);
    fwrite(&status, 1, 1, fd);
}

// Seta o número de registros no cabeçalho
void setNumDeRegistros(FILE *fd, uint32_t num) {
    fseek(fd, 17, SEEK_SET);
    fwrite(&num, 4, 1, fd);
}

// Seta o próximo offset livre no cabeçalho
void setProxOffset(FILE *fd, uint64_t num) {
    fseek(fd, 9, SEEK_SET);
    fwrite(&num, 4, 1, fd);
}

// Extrai o cabeçalho do arquivo
HEADER* extraiHeader(FILE *fd) {
    HEADER *h = malloc(sizeof(HEADER));
    if (ftell(fd) != 0)
        fseek(fd, 0, SEEK_SET);
    char hBuffer[25], fBuffer[8];

    fread(hBuffer, 1, 25, fd);
    h->status = hBuffer[0];

    slicea(fBuffer, hBuffer, 1, 9);
    memcpy(&(h->topo), fBuffer, 8);

    slicea(fBuffer, hBuffer, 9, 17);
    memcpy(&(h->offset), fBuffer, 8);

    slicea(fBuffer, hBuffer, 17, 21);
    memcpy(&(h->nReg), fBuffer, 4);

    slicea(fBuffer, hBuffer, 21, 25);
    memcpy(&(h->nRem), fBuffer, 4);

    return h;
}

// Converte o arquivo .csv para um binário, seguindo as especificações do trabalho
void csvToBin(char *srcFile, char *destFile) {
    initFile(destFile);
    uint64_t offset = 25;   
    uint32_t nRegistros = 0;

    FILE *src = fopen(srcFile, "r");
    FILE *data =  fopen(destFile, "r+b");
    setStatus(data, '0');
    fseek(data, offset, SEEK_SET);
    PLAYER *player;
    char tempstr[100]; 
    fgets(tempstr, 100, src);
    memset(tempstr, 0, 100);

    while (fgets(tempstr, 100, src) != NULL) {
        player = parseLine(tempstr);
        escreveRegistro(data, offset, player);
        offset += playerTamanho(player);
        memset(tempstr, 0, 100);
        playerFree(&player);
        nRegistros += 1;
    }

    setStatus(data, '1');
    setNumDeRegistros(data, nRegistros);
    setProxOffset(data, offset);
    fclose(data);
    fclose(src);
}

// Inicializa o cabeçalho do arquivo
void initFile(char *filename) {
    FILE *data = fopen(filename, "w+b");
    int64_t temp8bytes;
    int32_t temp4bytes;
    int8_t tempByte = '1';

    // Escreve o indicador de status
    fwrite(&tempByte, 1, 1, data);
    // Escreve o próximo logicamente removido
    temp8bytes = -1;
    fwrite(&temp8bytes, 8, 1, data);
    // Escreve o próximo offset disponível
    temp8bytes = 0;
    fwrite(&temp8bytes, 8, 1, data);
    // Escreve o número de registros
    temp4bytes = 0;
    fwrite(&temp4bytes, 4, 1, data);
    // Escreve o número de registros removidos logicamente
    fwrite(&temp4bytes, 4, 1, data);
    fclose(data);
}

// Escreve no arquivo a informação de um jogador a partir da struct
void escreveRegistro(FILE *data, uint64_t offset, PLAYER *player) {
    int8_t tempByte = '0';
    int64_t temp8bytes = -1;
    fwrite(&tempByte, 1, 1, data); // Removido
    int32_t regSize = playerTamanho(player);

    fwrite(&regSize, 4, 1, data); // Tamanho do registro
    fwrite(&temp8bytes, 8, 1, data); // Próximo offset
    fwrite(&(player->id), 4, 1, data); // ID
    fwrite(&(player->idade), 4, 1, data);
    fwrite(&(player->nomeLen), 4, 1, data); // Tamanho do nome

    if (player->nomeLen > 0)
        fwrite((player->nome), 1, player->nomeLen, data);

    fwrite(&(player->paisLen), 4, 1, data); // Tamanho do país

    if (player->paisLen > 0)
        fwrite((player->pais), 1, player->paisLen, data);

    fwrite(&(player->clubeLen), 4, 1, data); // Tamanho do clube
    
    if (player->clubeLen > 0)
        fwrite((player->clube), 1, player->clubeLen, data);   
}

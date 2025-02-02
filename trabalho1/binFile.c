#include"binFile.h"
/*
================================================
Arquivo para manipulacao direta dos arquivos binarios
================================================
*/


//altera o status do arquivo 
void setStatus(FILE *fd, uint8_t status){
    fseek(fd,0, SEEK_SET);
    fwrite(&status, 1, 1, fd);
}

//Seta o numero de registros no cabecalho
void setNumDeRegistros(FILE *fd, uint32_t num){
    fseek(fd, 17, SEEK_SET);
    fwrite(&num, 4, 1 ,fd);
}
//Seta o proximo offset livre no cabecalho
void setProxOffset(FILE *fd, uint64_t num){
    fseek(fd, 9, SEEK_SET);
    fwrite(&num, 4, 1 ,fd);
}
//Extrai as informacoes do header e poe na struct
HEADER* extraiHeader(FILE *fd){
    HEADER* h = malloc(sizeof(HEADER));
    if(ftell(fd) != 0) //acessa inicio do arquivo
        fseek(fd, 0, SEEK_SET);
    char hBuffer[25], *ptr;
    fread(hBuffer, 1, 25, fd);
    ptr = hBuffer;
    h->status = hBuffer[0];
    ptr += 1;
    memcpy(&(h->topo), ptr, 8);
    ptr += 8;
    memcpy(&(h->offset), ptr, 8);
    ptr += 8;
    memcpy(&(h->nReg), ptr, 4);
    ptr += 4;
    memcpy(&(h->nRem), ptr, 4);
    if(h->offset == 0){
        fseek(fd, 0, SEEK_END);
        h->offset = ftell(fd);
    }
    return h;
}

//Converte o arquivo .csv para um binario, seguindo as especificacoes do trabalho
void csvToBin(char* srcFile, char* destFile){
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

    while(fgets(tempstr, 100, src) != NULL){ //itera pelo .csv
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

//Inicializa o cabecalho do arquivo
void initFile(char* filename){
    FILE *data = fopen(filename, "w+b");
    int64_t temp8bytes;
    int32_t temp4bytes;
    int8_t tempByte = '1';
    //Escreve o indicador de status
    fwrite(&tempByte, 1, 1, data);
    //Escreve o proximo logicamente removido
    temp8bytes = -1;
    fwrite(&temp8bytes, 8, 1, data);
    //Escreve o proximo offset disponivel
    temp8bytes = 0;
    fwrite(&temp8bytes, 8, 1, data);
    //Escreve o n de registros
    temp4bytes = 0;
    fwrite(&temp4bytes, 4, 1, data);
    //Escreve o n de registros removidos logicamente
    fwrite(&temp4bytes, 4, 1, data);
    fclose(data);
}



//Escreve no arquivo a info de um jogador a partir da struct
void escreveRegistro(FILE* data, uint64_t offset, PLAYER* player){
    int8_t tempByte = '0';
    int64_t temp8bytes = -1;
    fwrite(&tempByte, 1, 1, data); //removido
    int32_t regSize = playerTamanho(player);
    
    fwrite(&regSize,4, 1, data); //tamanho do registro
    fwrite(&temp8bytes, 8, 1, data); //prox offset
    fwrite(&(player->id), 4, 1, data); //id
    fwrite(&(player->idade), 4, 1, data);
    fwrite(&(player->nomeLen), 4, 1, data); //tamanho nome
    if(player->nomeLen > 0)
        fwrite((player->nome), 1,player->nomeLen, data);
    fwrite(&(player->paisLen), 4, 1, data); //tamanho pais
    if(player->paisLen > 0)
        fwrite((player->pais), 1,player->paisLen, data);
    fwrite(&(player->clubeLen), 4, 1, data); //tamanho clube
    if(player->clubeLen > 0)
        fwrite((player->clube), 1,player->clubeLen, data);   
} 

#include"binFile.h"



void initFile(){
    FILE *data = fopen("data.bin", "w+b");
    int64_t temp8bytes;
    uint32_t temp4bytes;
    uint8_t tempByte = 1;
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



void escreveRegistro(FILE* data, uint64_t offset, PLAYER* player){
    uint8_t tempByte = 0;
    int64_t temp8bytes = -1;
    fseek(data, offset, SEEK_SET);
    fwrite(&tempByte, 1, 1, data); //removido
    size_t regSize = playerTamanho(player);
    fwrite(&regSize,4, 1, data); //tamanho do registro
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
    fclose(data);
} 

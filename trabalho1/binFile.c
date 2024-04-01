#include"binFile.h"



FILE *initFile(){
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
        
}

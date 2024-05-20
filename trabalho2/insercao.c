#include "binFile.h"
#include "index.h"



//O indice eh guardado em RAM, n em disco, da uma olhada dps no meu codigo
void atualizarIndice(FILE *arquivoIndice, int id, long posicao) {
    INDEX novoIndice;
    novoIndice->data->id = id;
    novoIndice->data->offset = posicao;

    fseek(arquivoIndice, 0, SEEK_END);
    fwrite(&novoIndice, sizeof(INDEX), 1, arquivoIndice);

}


//A insercao  n eh so no final, tem q usar best fit, procurar pelos logicamente removidos, tem nos slides
//sobre reuso de espaco
void insertion(char *arqDados, char *arqInd, int n) {

    arquivoDados = fopen(arqDados, "ab");
    arquivoIndice = fopen(arqInd, "ab");//Msm coisa, ta em memoria principal

    if (arquivoDados == NULL || arquivoIndice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < n; i++) {
        PLAYER *newPlayer;
        newPlayer = playerInit();

        //Dentro das funcoes fornecidas tem uma que le string entre aspas, usa ela q fica mais legivel
        scanf("%d, %d, \"%[^\"]\", \"%[^\"]\", \"%[^\"]\"", &newPlayer->id, &newPlayer->idade, newPlayer->nome, newPlayer->pais, newPlayer->clube);




        int8_t tempByte = '0';
        int64_t temp8bytes = -1;
        fwrite(&tempByte, 1, 1, arqDados);
        int32_t regSize = playerTamanho(newPlayer);
        
        fwrite(&regSize,4, 1, arquivoDados);
        fwrite(&temp8bytes, 8, 1, arquivoDados); 
        fwrite(&(newPlayer->id), 4, 1, arquivoDados);
        fwrite(&(newPlayer->idade), 4, 1, arquivoDados);
        fwrite((newPlayer->nome), 1,player->nomeLen, arquivoDados);
        fwrite((newPlayer->pais), 1,player->paisLen, arquivoDados);
        fwrite((newPlayer->clube), 1,player->clubeLen, arquivoDados); 

        atualizarIndice(arquivoIndice, newPlayer->id, ftell(arquivoDados));

    }

    fclose(arquivoDados);
    fclose(arquivoIndice);
}
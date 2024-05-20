#include "binFile.h"
#include "index.h"

// O indice eh guardado em RAM, n em disco, da uma olhada dps no meu codigo (edicao a ser feita)
void atualizarIndice(FILE *arquivoIndice, int id, long posicao) {
    INDEX novoIndice;
    novoIndice->data->id = id;
    novoIndice->data->offset = posicao;

    fseek(arquivoIndice, 0, SEEK_END);
    fwrite(&novoIndice, sizeof(INDEX), 1, arquivoIndice);

}

void escreverRegistro(FILE *arquivoDados, PLAYER *newPlayer, long posicao, FILE *arquivoIndice) {
    fwrite(&newPlayer->id, sizeof(int), 1, arquivoDados);
    fwrite(&newPlayer->idade, sizeof(int), 1, arquivoDados);
    fwrite(newPlayer->nome, sizeof(char), newPlayer->nomeLen, arquivoDados);
    fwrite(newPlayer->pais, sizeof(char), newPlayer->paisLen, arquivoDados);
    fwrite(newPlayer->clube, sizeof(char), newPlayer->clubeLen, arquivoDados);

}

void insertion(char *arqDados, char *arqInd, int n) {
    FILE *arquivoDados, *arquivoIndice;
    arquivoDados = fopen(arqDados, "r+b");
    arquivoIndice = fopen(arqInd, "r+b");//Msm coisa, ta em memoria principal

    if (arquivoDados == NULL || arquivoIndice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        PLAYER *newPlayer;
        char str1[32], str2[32], str3[32];
        newPlayer = playerInit();
        newPlayer->nome, newPlayer->pais, newPlayer->clube

        scanf("%d, %d, %s, %s, %s", &newPlayer->id, &newPlayer->idade, str1, str2, str3);
        scan_quote_string(str1);
        scan_quote_string(str2);
        scan_quote_string(str3);
        newPlayer->nome = strdup(str1);
        newPlayer->pais = strdup(str2);
        newPlayer->clube = strdup(str3);
        newPlayer->nomeLen = strlen(newPlayer->nome) + 1; // +1 do caractere nulo
        newPlayer->paisLen = strlen(newPlayer->pais) + 1;
        newPlayer->clubeLen = strlen(newPlayer->clube) + 1;

        // encontrar o registro lógico removido 
        // registro removido e marcar o lixo com '$'

        // caso nao tenha registros removidos
        } else {

        }
    }

    fclose(arquivoDados);
}

#include "myMysql.h"

// Função para selecionar registros com base em condições específicas
void selectWhere(char* filename, int numOfParameters, char** fields, char** values) {
    FILE *fd = fopen(filename, "rb");
    if (fd == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Inicialização do offset para o início dos registros
    int64_t offset = 25;
    bool flag = false; // Flag para verificar se algum registro foi encontrado

    // Loop para percorrer o arquivo e ler os registros
    while (!feof(fd)) {
        PLAYER *p = playerFromBin(fd, offset); // Ler um registro do arquivo
        if (checkPlayer(p, numOfParameters, fields, values)) { // Verificar se o registro satisfaz as condições
            playerPrint(p); // Imprimir o registro
            flag = true; // Atualizar a flag indicando que pelo menos um registro foi encontrado
        }
        offset += playerTamanho(p); // Atualizar o offset para apontar para o próximo registro
        playerFree(&p); // Liberar a memória alocada para o registro
    }

    // Verificar se algum registro foi encontrado
    if (!flag)
        printf("Registro inexistente.\n\n");
}

// Função para selecionar registros de acordo com uma consulta SQL específica
void selectSQL(char* filename) {
    FILE* fd = fopen(filename, "rb");
    if (fd == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    bool flag = false; // Flag para verificar se algum registro foi encontrado

    // Extrair o cabeçalho do arquivo
    HEADER *h = extraiHeader(fd);

    // Inicialização do offset para o início dos registros
    int64_t offset = 25;

    // Loop para percorrer o arquivo e ler os registros
    while (!feof(fd) && h->offset > offset) {
        PLAYER *p = playerFromBin(fd, offset); // Ler um registro do arquivo

        // Verificar se o registro satisfaz a condição específica
        if (strcmp(p->nome,  "M. RISTIC") == 0)
            printf("parou"); // Imprimir uma mensagem de teste

        if (p->status == '0') { // Verificar se o status do jogador é válido
            playerPrint(p); // Imprimir o registro
            flag = true; // Atualizar a flag indicando que pelo menos um registro foi encontrado
        }

        offset += playerTamanho(p); // Atualizar o offset para apontar para o próximo registro
        playerFree(&p); // Liberar a memória alocada para o registro
    }

    // Verificar se algum registro foi encontrado
    if (!flag)
        printf("Registro inexistente.\n\n");
}

// Função para criar uma tabela a partir de um arquivo CSV
void createTable(char* srcName, char* destName) {
    // Verificar se o arquivo de origem existe
    if (fopen(srcName, "rb") == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Converter o arquivo CSV para binário
    csvToBin(srcName, destName);
}
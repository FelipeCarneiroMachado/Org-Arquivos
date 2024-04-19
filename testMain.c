#include"binFile.h"
#include<ctype.h>
#include"myMysql.h"

// Função para exibir conteúdo binário na tela
void binarioNaTela(char *nomeArquivoBinario) {
	// Declaração de variáveis
	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;

	// Verifica se o nome do arquivo é válido e tenta abrir o arquivo binário para leitura
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}

	// Obtém o tamanho do arquivo
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);

	// Aloca memória para armazenar o conteúdo do arquivo binário
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	// Calcula a soma dos bytes do arquivo
	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}

	// Exibe o resultado da soma na tela
	printf("%lf\n", (cs / (double) 100));

	// Libera a memória alocada e fecha o arquivo
	free(mb);
	fclose(fs);
}

// Função para ler uma string delimitada por aspas
void scan_quote_string(char *str) {
	char R;

	// Ignora espaços em branco
	while((R = getchar()) != EOF && isspace(R));

	// Verifica se o campo é nulo
	if(R == 'N' || R == 'n') {
		getchar(); getchar(); getchar(); // Ignora o "ULO" de NULO.
		strcpy(str, ""); // Copia uma string vazia
	} else if(R == '\"') { // Campo entre aspas
		if(scanf("%[^\"]", str) != 1) { // Lê até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // Ignora as aspas de fechamento
	} else if(R != EOF){ // Leitura normal para outros tipos de dados
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

// Função para alocar um array de strings
char **stringArray(int n_str, int size_buffer){
	char **temp = malloc(n_str * sizeof(char*));
	for(int i = 0; i < n_str; i++){
		temp[i] = malloc(size_buffer);
	} 
	return temp;
}

int main(){
	char commandBuffer[256];
	char *src, *dest;
	int nOfQueries, nOfFields;

	// Loop principal para receber comandos
	while (fgets(commandBuffer, 256, stdin) != NULL){
		switch(commandBuffer[0]){
			case '1':
				// Criação de tabela
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				dest = strtok(NULL, "\n");
				createTable(src, dest);
				binarioNaTela(dest);
				break;
			case '2':
				// Seleção SQL
				strtok(commandBuffer, " ");
				src = strtok(NULL, "\n");
				selectSQL(src);
				break;
			case '3':
				// Seleção com condição
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				nOfQueries = atoi(strtok(NULL, "\n"));
				char **fields = stringArray(5, 32);
				char **values = stringArray(5, 32);
				for(int i = 0; i < nOfQueries; i++){
					printf("Busca %d\n\n", i + 1);
					scanf("%d", &nOfFields);
					for(int j = 0; j < nOfFields; j++){
						scanf("%s", fields[j]);
						if(strcmp(fields[j], "nome") == 0 || strcmp(fields[j], "nomeClube") == 0 || strcmp(fields[j], "nacionalidade") == 0)
							scan_quote_string(values[j]);
						else
							scanf("%s", values[j]);
					}
					selectWhere(src, nOfFields, fields, values);
				}
			default:
				break;
		}
	}
	return 0;
}

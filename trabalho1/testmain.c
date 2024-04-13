#include"binFile.h"
#include<ctype.h>
#include"myMysql.h"

//Arquivo main para testar o resto do codigo ao passo que se trabalha

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


char **stringArray(int n_str, int size_buffer){
	char **temp = malloc(n_str * sizeof(char*));
	for(int i = 0; i < n_str; i++){
		temp[i] = malloc(size_buffer);
	} 
	return temp;
}


int main(){
	//char c[100];
	//fgets(c, 100, stdin);
	// PLAYER* p = parseLine("190653,,I. BRIZUELA,,\n");
	// playerPrint(p);
    //csvToBin("./casosTeste/arquivos/dado2.csv", "data.bin");
    //binarioNaTela("data.bin");
	// char id[20] = "NACIONALIDADE\0", idVal[20] = "SPAIN\0";
	// char **par = malloc(8);
	// char **val = malloc(8);
	// par[0] = malloc(20);
	// val[0]= malloc(20);
	// strcpy(par[0], id);
	// strcpy(val[0], idVal);
	// selectWhere("data.bin", 1, par, val);
	char commandBuffer[256];
	char *src, *dest, *intBuffer;
	int nOfQueries, nOfFields;
	while (fgets(commandBuffer, 256, stdin) != NULL){
		switch(commandBuffer[0]){
			case '1':		
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				dest = strtok(NULL, "\n");
				createTable(src, dest);
				binarioNaTela(dest);
				break;
			case '2':
				strtok(commandBuffer, " ");
				src = strtok(NULL, "\n");
				selectSQL(src);
				break;
			case '3':
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				nOfQueries = atoi(strtok(NULL, "\n"));
				char **fields = stringArray(5, 32);
				char **values = stringArray(5, 32);
				for(int i = 0; i < nOfQueries; i++){
					scanf("%d", &nOfFields);
					for(int j = 0; j < nOfFields; j++){
						scanf("%s", fields[j]);
						if(strcmp(fields[j], "nome") == 0 || strcmp(fields[j], "clube") == 0 || strcmp(fields[j], "nacionalidade") == 0)
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
	



	
}
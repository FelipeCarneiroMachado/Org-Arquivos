#include"binFile.h"
#include"myMysql.h"	
#include"utils.h"

//Arquivo main para testar o resto do codigo ao passo que se trabalha




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
	



	
}
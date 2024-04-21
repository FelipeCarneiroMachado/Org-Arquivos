#include"binFile.h"
#include"interface.h"	
#include"utils.h"
// Realizado pela dupla:
//     Felipe Carneiro Machado - NUSP: 14569373
//     Matheus Araujo Pinheiro - NUSP: 14676810
/*
================================================
Arquivo main do projeto
================================================
*/
int main(){
	char commandBuffer[256];
	char *src, *dest, *intBuffer;
	int nOfQueries, nOfFields;
	while (fgets(commandBuffer, 256, stdin) != NULL){
		switch(commandBuffer[0]){
			case '1':	//Criacao de tabela a partir de .csv (funcionalidade 1)	
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				dest = strtok(NULL, "\n");
				createTable(src, dest);
				binarioNaTela(dest);
				break;
			case '2':    //retornar todos os registros (funcionalidade 2)
				strtok(commandBuffer, " ");
				src = strtok(NULL, "\n");
				selectAll(src);
				break;
			case '3':    //retornar condicionalmente os registros (funcionalidade 3)
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
				freeStringArray(&fields, 5);
				freeStringArray(&values, 5);
			default:
				break;
			
		}
	}
	
}
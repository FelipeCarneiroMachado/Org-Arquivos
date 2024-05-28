#include"binFile.h"
#include"interface.h"	
#include"utils.h"
//Link do video: https://drive.google.com/file/d/1G40-3ix879ICT-oErW8Zs52jxyukylWs/view?usp=drive_link
// Realizado pela dupla (Grupo 12):
//     Felipe Carneiro Machado - NUSP: 14569373
//     Matheus Araujo Pinheiro - NUSP: 14676810
/*
================================================
Arquivo main do projeto
================================================
*/
int main(){
	FILE *bin = NULL;
	HEADER *header = NULL;
	INDEX* index = NULL;
	char commandBuffer[256];
	char *src, *dest, *intBuffer, indexName;
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
			case '4':
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				indexName = strtok(NULL, "\n");
				if(bin == NULL)
					bin = fopen(src, "r+b");
				index = createIndex(bin, indexName);
				break;
			case '5':
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				if(bin == NULL)
					bin == fopen(src ,"r+b");
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
					delete(bin, index, nOfFields, fields, values);
				}
				fclose(bin);
				binarioNaTela(src);
				freeStringArray(&fields, 5);
				freeStringArray(&values, 5);
				break;
			default:
				break;
			
		}
	}
	
}
#include"binFile.h"
#include"interface.h"	
#include"utils.h"
//Link do video: https://drive.google.com/file/d/1AyvIsoHaJeqoMSfXkC6RaOFIbvVBA3Zb/view?usp=sharing
// Realizado pela dupla (Grupo 12):
//     Felipe Carneiro Machado - NUSP: 14569373
//     Matheus Araujo Pinheiro - NUSP: 14676810
/*
================================================
Arquivo main do projeto
================================================
*/
int main(){
	FILE *bin = NULL, *csv = NULL, *bTree = NULL;
	HEADER *header = NULL;
	INDEX* index = NULL;
	BT_HEADER *bth;
	char commandBuffer[256];
	char *src, *dest, *intBuffer, *indexName, **fields, **values, idBuf[5];
	int nOfQueries, nOfFields;
	fgets(commandBuffer, 256, stdin);
	if(commandBuffer[1] == '0')
		commandBuffer[0] = 'I';
	switch(commandBuffer[0]){
		case '1':	//Criacao de tabela a partir de .csv (funcionalidade 1)	
			strtok(commandBuffer, " ");
			src = strtok(NULL, " ");
			dest = strtok(NULL, "\n");
			csv = fopen(src, "r");
			bin = initFile(dest);
			createTable(csv, bin);
			fclose(csv);
			fclose(bin);
			binarioNaTela(dest);
			break;
		case '2':    //retornar todos os registros (funcionalidade 2)
			strtok(commandBuffer, " ");
			src = strtok(NULL, "\n");
			if((bin = fopen(src, "rb")) == NULL ){
				printf("Falha no processamento do arquivo.\n");
				break;
			}
			if(header->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				break;
			}
			header = extraiHeader(bin);
			selectAll(bin, header);
			fclose(bin);
			free(header);
			break;
		case '3':    //retornar condicionalmente os registros (funcionalidade 3)
			strtok(commandBuffer, " ");
			src = strtok(NULL, " ");
			if((bin = fopen(src, "rb")) == NULL ){
				printf("Falha no processamento do arquivo.\n");
				break;
			}
			if(header->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				break;
			}
			header = extraiHeader(bin);
			nOfQueries = atoi(strtok(NULL, "\n"));
			fields = stringArray(5, 32);
			values = stringArray(5, 32);
			for(int i = 0; i < nOfQueries; i++){
				printf("Busca %d\n\n", i + 1);
				scanf("%d", &nOfFields);
				for(int j = 0; j < nOfFields; j++){
					scanf("%s", fields[j]);
					if(strcmp(fields[j], "nomeJogador") == 0 || strcmp(fields[j], "nomeClube") == 0 || strcmp(fields[j], "nacionalidade") == 0)
						scan_quote_string(values[j]);
					else
						scanf("%s", values[j]);
				}
				selectWhere(bin, header, nOfFields, fields, values);
			}
			freeStringArray(&fields, 5);
			freeStringArray(&values, 5);
			fclose(bin);
			free(header);
		case '4':
			strtok(commandBuffer, " ");
			src = strtok(NULL, " ");
			indexName = strtok(NULL, "\n");
			if((bin = fopen(src, "rb")) == NULL ){
				printf("Falha no processamento do arquivo.\n");
				break;
			}
			header = extraiHeader(bin);
			if(header->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				free(header);
				break;
			}
			index = createIndex(bin, header, indexName);
			binarioNaTela(indexName);
			fclose(bin);
			free(header);
			indexFree(&index);
			break;
		case '5':
			strtok(commandBuffer, " ");
			src = strtok(NULL, " ");
			if((bin = fopen(src, "r+b")) == NULL ){
				printf("Falha no processamento do arquivo.\n");
				break;
			}
			header = extraiHeader(bin);
			if(header->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				free(header);
				break;
			}
			indexName = strtok(NULL, " ");
			index = createIndex(bin, header, indexName);
			nOfQueries = atoi(strtok(NULL, "\n"));
			fields = stringArray(5, 32);
			values = stringArray(5, 32);
			for(int i = 0; i < nOfQueries; i++){
				scanf("%d", &nOfFields);
				for(int j = 0; j < nOfFields; j++){
					scanf("%s", fields[j]);
					if(strcmp(fields[j], "nomeJogador") == 0 || strcmp(fields[j], "nomeClube") == 0 || strcmp(fields[j], "nacionalidade") == 0)
						scan_quote_string(values[j]);
					else
						scanf("%s", values[j]);
				}
				delete(bin, header, index, nOfFields, fields, values);
			}
			updateHeader(bin, header);
			writeIndex(index, indexName);
			free(header);
			fclose(bin);
			binarioNaTela(src);
			binarioNaTela(indexName);
			indexFree(&index);
			freeStringArray(&fields, 5);
			freeStringArray(&values, 5);
			break;
		case '6':
			strtok(commandBuffer, " ");
			src = strtok(NULL, " ");
			if((bin = fopen(src, "r+b")) == NULL ){
				printf("Falha no processamento do arquivo.\n");
				break;
			}
			header = extraiHeader(bin);
			if(header->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				free(header);
				break;
			}
			indexName = strtok(NULL, " ");
			index = createIndex(bin, header, indexName);
			nOfQueries = atoi(strtok(NULL, "\n"));
			for(int i = 0; i < nOfQueries; i++){
				PLAYER *p = playerInput(commandBuffer);
				insert(bin, header, index, p);
				playerFree(&p);
			}
			updateHeader(bin, header);
			writeIndex(index, indexName);
			fclose(bin);
			binarioNaTela(src);
			binarioNaTela(indexName);
			free(header);
			indexFree(&index);
			break;
		case '7':
			strtok(commandBuffer, " ");
			src = strtok(NULL, " ");
			indexName = strtok(NULL, "\n");
			if((bin = fopen(src, "rb")) == NULL ){
				printf("Falha no processamento do arquivo.\n");
				break;
			}
			header = extraiHeader(bin);
			if(header->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				free(header);
				break;
			}
			bth = createBtree(bin, indexName, header);
			FILE *bt = fopen(indexName, "r+b");
			BT_writeHeader(bth, bt, true);
			fclose(bt);
			binarioNaTela(indexName);
			fclose(bin);
			free(header);
			free(bth);
			break;
		case '8':
			strtok(commandBuffer, " ");
			src = strtok(NULL, " ");
			if((bin = fopen(src, "rb")) == NULL ){
				printf("Falha no processamento do arquivo.\n");
				break;
			}
			header = extraiHeader(bin);
			if(header->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				break;
			}
			indexName = strtok(NULL, " ");
			if((bTree = fopen(indexName, "rb")) == NULL){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				free(header);
				break;
			}
			bth = bt_headerRead(bTree);
			if(bth->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bTree);
				fclose(bin);
				free(header);
				break;
			}
			nOfQueries = atoi(strtok(NULL, "\n"));
			int id;
			int64_t foundOff = -1;
			for(int i = 0; i < nOfQueries; i++){
				printf("BUSCA %d\n\n", i + 1);
				scanf("%s", idBuf);
				scanf("%d", &id);
				foundOff = BT_search(bth, id, bTree);
				if(foundOff == -1)
					printf("Registro inexistente.\n\n");
				else{
					PLAYER *p = playerFromBin(bin, foundOff);
					playerPrint(p);
					playerFree(&p);
				}
			}
			free(bth);
			fclose(bin);
			fclose(bTree);
			free(header);
			break;
		case '9':
			strtok(commandBuffer, " ");
			src = strtok(NULL, " ");
			if((bin = fopen(src, "rb")) == NULL ){
				printf("Falha no processamento do arquivo.\n");
				break;
			}
			header = extraiHeader(bin);
			if(header->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				break;
			}
			indexName = strtok(NULL, " ");
			if((bTree = fopen(indexName, "rb")) == NULL){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				free(header);
				break;
			}
			bth = bt_headerRead(bTree);
			if(bth->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bTree);
				fclose(bin);
				free(header);
				break;
			}
			nOfQueries = atoi(strtok(NULL, "\n"));
			fields = stringArray(5, 32);
			values = stringArray(5, 32);
			for(int i = 0; i < nOfQueries; i++){
				printf("Busca %d\n\n", i + 1);
				scanf("%d", &nOfFields);
				for(int j = 0; j < nOfFields; j++){
					scanf("%s", fields[j]);
					if(strcmp(fields[j], "nomeJogador") == 0 || strcmp(fields[j], "nomeClube") == 0 || strcmp(fields[j], "nacionalidade") == 0)
						scan_quote_string(values[j]);
					else
						scanf("%s", values[j]);
				}
				selectWithBtree(bin ,bTree, bth, header, nOfFields, fields, values);
			}
			freeStringArray(&fields, 5);
			freeStringArray(&values, 5);
			fclose(bin);
			free(header);
			break;
		case 'I':
			strtok(commandBuffer, " ");
			src = strtok(NULL, " ");
			if((bin = fopen(src, "r+b")) == NULL ){
				printf("Falha no processamento do arquivo.\n");
				break;
			}
			header = extraiHeader(bin);
			if(header->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				free(header);
				break;
			}
			indexName = strtok(NULL, " ");
			if((bTree = fopen(indexName, "r+b")) == NULL){
				printf("Falha no processamento do arquivo.\n");
				fclose(bin);
				free(header);
				break;
			}
			bth = bt_headerRead(bTree);
			if(bth->status == '0'){
				printf("Falha no processamento do arquivo.\n");
				fclose(bTree);
				fclose(bin);
				free(header);
				break;
			}
			nOfQueries = atoi(strtok(NULL, "\n"));
			for(int i = 0; i < nOfQueries; i++){
				PLAYER *p = playerInput(commandBuffer);
				insertWithBtree(bin, bTree, bth, header, p);
				updateHeader(bin, header);
				playerFree(&p);
			}
			BT_writeHeader(bth, bTree, true);
			fclose(bTree);
			fclose(bin);
			binarioNaTela(src);
			binarioNaTela(indexName);
			free(bth);
			free(header);
		default:
			break;
		
	}
}
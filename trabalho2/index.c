#include"index.h"

/*
TODO
Escrita e leitura a partir do arquivo de indice
Insercao e remocao no indice
Integrar o indice a interface (Bptei um pouco mas ta mal feito)
*/




//Funcoes para o sorting do indice
void swap(struct data* arr, int a, int b){
    struct data temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

int partition(struct data* arr, int low, int high){
    struct data pivot = arr[(low + high) / 2];
    int i = low, j = high;
    while(i < j){
        if(arr[i].id >= pivot.id && arr[j].id <= pivot.id)
            swap(arr, i, j);
        if(arr[i].id < pivot.id)
            i++;
        if(arr[j].id > pivot.id)
            j--;
    }
}


void quickSort(struct data* arr, int low, int high){
    if(low < high){
        int pvt = partition(arr, low, high);
        quickSort(arr, low, pvt - 1);
        quickSort(arr, pvt + 1, high);
    }
}


void indexSort(INDEX* i){
    quickSort(i->array, 0, i->nReg - 1);
}
//Printar para debug, printa num arquivo de log
void printArr(INDEX *i){
    FILE * fd = fopen("log.log", "a");
    for(int j = 0; j < i->nReg - 1; j++){
        fprintf(fd, " (%d %d)  ", i->array[j].id, i->array[j].offset);
    }
    fclose(fd);
}
INDEX* indexInit(int base_len){
    INDEX *i = (INDEX*)malloc(sizeof(INDEX));
    i->nReg = 0;
    i->arrLen = base_len;
    i->status = '1';
    i->array = (struct data*)malloc(base_len * sizeof(struct data));
}

//Cria o indice a partir do arquivo de dados
INDEX* makeIndex(FILE *fd){
    FILE *log = fopen("log.log", "w");
    if(fd == NULL){
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }
    HEADER *h = extraiHeader(fd);
    if(h->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }
    //Alocacoes
    INDEX *i = indexInit(h->nReg);
    //Leitura de header
    if(ftell(fd) != 0)
        fseek(fd, 0, SEEK_SET);
    
    //Itera pelo arquivo de dados
    uint64_t offset = 25;
    while(offset < h->offset){
        if(offset == 3156)
            fprintf(log, "halt\n");
        int *p = idFromBin(fd, offset);//Recupera id e tamanho
        if(p[1] != -1){
            //Append na lista
            i->array[i->nReg].id = p[1];
            i->array[i->nReg].offset = offset;
            i->nReg++;
            fprintf(log, "reg escrito %d / %ld\n", p[1], offset);
            fflush(log);
        }
        //Logica de realocacao, sera removida dps pq tem jeito melhor
        if(i->nReg == i->arrLen){
            i->arrLen *= 2;
            i->array =(struct data*) realloc(i->array, i->arrLen * sizeof(struct data));
            fprintf(log, "realocando: de %d para %d\n", i->arrLen /2, i->arrLen);
            fflush(log);
        }
        offset += p[0];
        free(p);
        // if(i->nReg == 20)
        //     break;
    }
    fprintf(log, "sort inciado\n");
    fflush(log);
    indexSort(i);
    fprintf(log, "sort concluido\n");
    fflush(log);
    printArr(i);
    return i;
    fclose(fd);
}

//Uma busca binaria simples
int64_t indexSearchAux(struct data *arr, int target, int lo, int hi){
    if(lo < hi){
        struct data half = arr[(lo + hi)/2]; 
        if(half.id == target)
            return half.offset;
        if(half.id > target)
            return indexSearchAux(arr, target, lo, (lo + hi)/2);
        if(half.id < target)
            return indexSearchAux(arr, target, ((lo + hi)/2) + 1, hi);
    }
    return -1;
}
//Wrapper da BB, pertence a interface
int64_t indexSearch(INDEX* index, int id){
    return indexSearchAux(index->array, id, 0, index->nReg - 1);
}


//Insercao no registro em memoria principal
int indexSearchPositionAux(struct data *arr, int target, int lo, int hi){
    if(lo < hi){
        int half = (lo + hi)/2; 
        if(arr[half].id <= target && arr[half].id >= target)
            return (lo + hi)/2;
        if(arr[half].id > target)
            return indexSearchAux(arr, target, lo, (lo + hi)/2);
        if(arr[half].id < target)
            return indexSearchAux(arr, target, ((lo + hi)/2) + 1, hi);
    }
    return -1;
}

int64_t indexSearchPosition(INDEX* index, int id){
    if(index->array[0].id > id)
        return 0;
    if(index->array[index->nReg - 1].id > id)
        return index->nReg - 1;
    return indexSearchAux(index->array, id, 0, index->nReg - 1);
}
void shiftRight(INDEX* index, int pos){
    for(int i = index->nReg - 1; i >= pos; i++){
        index->array[i + 1] = index->array[i];
    }
}

void indexInsert(INDEX *index, int id, int offset){
    int indToInsert = indexSearchPosition(index, id);
    shiftRight(index, indToInsert);
    index->array[indToInsert].id = id; 
    index->array[indToInsert].offset = offset;
    index->nReg++;
    if(index->nReg == index->arrLen){
        index->arrLen *= 2;
        index->array =(struct data*) realloc(index->array, index->arrLen * sizeof(struct data));
    }
}


//Funcoes de escrita do indice em arquivo de indice
void writeIndexReg(struct data reg, FILE *fd, uint64_t offset){
    if(ftell(fd) != offset)
        fseek(fd, offset, SEEK_SET);
    fwrite(&(reg.id), 4, 1, fd);
    fwrite(&(reg.offset), 8, 1, fd);
}

void writeIndex(INDEX* index, char* filename){
    FILE *fd = fopen(filename, "wb");
    char tempByte = '0';
    fwrite(&tempByte, 1, 1, fd);
    for(int offset = 1, i = 0; i < index->nReg; offset += sizeof(struct data), i++){
        writeIndexReg(index->array[i], fd, offset);
    }
    fseek(fd, 0, SEEK_SET);
    tempByte = '1';
    fwrite(&tempByte, 1, 1, fd);
    fclose(fd);
    binarioNaTela(filename);
}

//Funcoes de carregamento do indice

INDEX *inconsistentIndexHandler(){
    //Futuramente deve fazer a reconstrucao do indice
    printf("Falha no processamento do arquivo.\n");
    return NULL;
}

void indexRegFromBin(FILE *fd, struct data *reg, uint64_t offset){
    if(ftell(fd) != offset)
        fseek(fd, offset, SEEK_SET);
    fread(&(reg->id), 4, 1, fd);
    fread(&(reg->offset), 8, 1, fd);
}

INDEX* loadIndex(char *filename){
    FILE *fd = fopen(filename, "rb");
    INDEX *index = indexInit(100);
    if(ftell(fd) != 0)
        fseek(fd, 0, SEEK_SET);
    char status;
    fread(&status, 1, 1 ,fd);
    if(status != '0')
        return inconsistentIndexHandler();
    uint64_t offset = 1;
    while(!feof(fd)){
        indexRegFromBin(fd, &(index->array[index->nReg]), offset);
        index->nReg++;
        if(index->nReg == index->arrLen){
            index->arrLen *= 2;
            index->array =(struct data*) realloc(index->array, index->arrLen * sizeof(struct data));
        }
    }
    fclose(fd);
    return index;
}


void shiftLeft(INDEX* index, int position){
    for(int i = position; i < index->nReg - 1; i++)
        index->array[i] = index->array[i+1];
}

void indexRemove(INDEX* index, int id){
    int position = indexSearchPosition(index, id);
    shiftLeft(index, position);
    index->nReg--;
}
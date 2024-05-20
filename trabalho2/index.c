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
    FILE * fd = fopen("log.log", "w");
    for(int j = 0; j < i->nReg - 1; j++){
        fprintf(fd, " (%d %d)  ", i->array[j].id, i->array[j].offset);
    }
    fclose(fd);
}
//Cria o indice
INDEX* createIndex(FILE *fd){
    if(fd == NULL){
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }
    //Alocacoes
    INDEX *i = (INDEX*)malloc(sizeof(INDEX));
    i->nReg = 0;
    i->arrLen = BASE_SIZE;
    i->status = '0';
    i->array = (struct data*)malloc(BASE_SIZE * sizeof(struct data));
    //Leitura de header
    if(ftell(fd) != 0)
        fseek(fd, 0, SEEK_SET);
    HEADER *h = extraiHeader(fd);
    if(h->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }
    //Itera pelo arquivo de dados
    uint64_t offset = 25;
    while(offset < h->offset){
        int *p = idFromBin(fd, offset);//Recupera id e tamanho
        if(p[1] != -1){
            //Append na lista
            i->array[i->nReg].id = p[1];
            i->array[i->nReg].offset = offset;
            i->nReg++;
        }
        //Logica de realocacao, sera removida dps pq tem jeito melhor
        if(i->nReg == i->arrLen){
            i->arrLen *= 2;
            i->array =(struct data*) realloc(i->array, i->arrLen * sizeof(struct data));
        }
        offset += p[0];
        free(p);
        // if(i->nReg == 20)
        //     break;
    }
    indexSort(i);
    printArr(i);
    return i;
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
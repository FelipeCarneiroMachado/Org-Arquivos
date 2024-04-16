#include"utils.h"

void slice(char * dest, char* src, int start, int end){
    int k = 0;
    for(int i = start; i < end; i++){
        dest[k++] = src[i];
    }
    dest[k] = '\0';
}
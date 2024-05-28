#include"interface.h"
#include"index.h"

int main(){
    INDEX *i = createIndex(fopen("Entrada/binario1.bin", "r+b"));
    writeIndex(i, "testi.bin");
}
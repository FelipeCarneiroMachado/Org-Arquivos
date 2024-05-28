#include"interface.h"
#include"index.h"

int main(){
    INDEX *i = createIndex(fopen("Entrada/binario1.bin", "r+b"));
    writeIndex(i, "testi.bin");
    //fclose("Entrada/binario1.bin");
    FILE *bin = fopen("Entrada/binarioTeste.bin", "r+b");
    HEADER *h =  extraiHeader(bin);
    removeInDisk(bin, h, 0xa6);
    removeInDisk(bin, h, 25);
    removeInDisk(bin, h, 0xe2);
    fclose(bin);
}
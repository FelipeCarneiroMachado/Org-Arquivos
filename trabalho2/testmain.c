#include"interface.h"
#include"index.h"

int main(){
    INDEX *i = makeIndex(fopen("Entrada/binario1.bin", "r+b"));
    writeIndex(i, "testi.bin");
    //fclose("Entrada/binario1.bin");
    FILE *bin = fopen("Entrada/binarioTeste.bin", "r+b");
    HEADER *h =  extraiHeader(bin);
    removeInDisk(bin, h, 0xa6);
    removeInDisk(bin, h, 25);
    removeInDisk(bin, h, 0xe2);
    //insertPlayer(bin, h, playerFromBin(bin, 0x778));
    //insertPlayer(bin, h, playerFromBin(bin, 0x1db));
    insertPlayer(bin, h, playerFromBin(bin, 0x59c));
    fclose(bin);
}
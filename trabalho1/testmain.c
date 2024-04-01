#include"binFile.h"


int main(){
    char str[] = "247106,21,A. BERNABEI,ARGENTINA,CELTIC\n";
    PLAYER* p = parseLine(str);
    initFile();
    playerPrint(p);
    FILE *data = fopen("data.bin", "a+b");
    escreveRegistro(data, 25, p);



}
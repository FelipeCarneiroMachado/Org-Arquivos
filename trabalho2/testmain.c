#include"interface.h"
#include"index.h"

int main(){
    INDEX *i = createIndex(fopen("bin.bin", "r+b"));
    printf("%d", indexSearch(i, 206594));
}
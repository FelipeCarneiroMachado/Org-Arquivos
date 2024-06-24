#include"interface.h"
#include"bTree.h"
#include"index.h"

void nop(){

}

int main(){

    FILE *fd = fopen("indice1.bin", "r+b");
    BT_HEADER *bth = bt_headerRead(fd);
    printf("nreg = %d, prox RRN = %d, root = %d\n", bth->nroChaves, bth->proxRRN, bth->noRaiz);
    printNormal(bth->noRaiz, fd);
    fclose(fd);
    return 0;
    //BT_HEADER *bth = bTreeInit("mytreetest.bin");
    //FILE *fd = fopen("mytreetest.bin", "r+b");
    int n, temp;
    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        scanf("%d", &temp);
        if(temp == 60)
            nop();        
        BT_insert(fd, bth, makeTrio(temp, 666, -1));
        printf("new = %d\n", temp);
        printf("nreg = %d, prox RRN = %d, root = %d\n", bth->nroChaves, bth->proxRRN, bth->noRaiz);
        // BT_print(bth->noRaiz, fd);    
        if(BT_printWrap(bth, fd))
            break;
        printf("\n\n");
        fflush(fd);
    }
    // printf("nreg = %d, prox RRN = %d, root = %d\n", bth->nroChaves, bth->proxRRN, bth->noRaiz);
    // BT_print(bth->noRaiz, fd);    
    fclose(fd);
    
    //printf("root = %d\n", bth->noRaiz);
}
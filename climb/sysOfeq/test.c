#include <stdio.h>

int main() {
    int i;
    for (i=0;i<10000;i++) {
        if (i%(100+(i/100)) == 0) printf("1");
        else printf("0");
        if (i%100 == 99) printf("\n\n");
        else printf(" ");
    }
}

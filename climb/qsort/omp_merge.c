
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"

void merge (int *a, int n, int m) {
    int i, j, k;
    int *x = malloc(n * sizeof (int));
    for (i = 0, j = m, k = 0; k < n; k++) {
        x[k] = j == n      ? a[i++]
             : i == m      ? a[j++]
             : a[j] < a[i] ? a[j++]
             :               a[i++];
    }
    for (i = 0; i < n; i++) {
        a[i] = x[i];
    }
    free(x);
}
 
void merge_sort (int *a, int n) {
    if (n < 2)
        return;
    int m = n / 2;
    merge_sort(a, m);
    merge_sort(a + m, n - m);
    merge(a, n, m);
}

int main()
{
    char *line = NULL;
    size_t size;
    int read;
    int p, j;
    int *nums = (int*)malloc(10000000*sizeof(int));
    int c = 0;
    while ((read = getline(&line, &size, stdin)) != -1) {
        if (*line == 'p') {
            p = atoi(&(line[2]));
        } else if (*line == 'q') {
            char *tok = strtok(line, " ");
            tok = strtok(NULL, " ");
            while (tok != NULL) {
                nums[c++] = atoi(tok);
                tok = strtok(NULL, " ");
            }
        }
    } 


    int h = c/2;
    int q = h/2;

    #pragma omp parallel for
    for (j=0;j<4;j++)
    {
        if (0==j)
            merge_sort(nums, q);
        if (1==j)
            merge_sort(nums + q, h - q);
        if (2==j)
            merge_sort(nums + h, q);
        else
            merge_sort(nums + h + q, c - h - q);
    }
    
    merge(nums, h, q);
    
    merge(nums + h, c - h, q);

    merge(nums, c, h);


    printf("s ");
    for (j = 0; j<c; j++) {
        printf("%d ", nums[j]);
    }
    printf("\n");


    free(nums);

    return 0;
}

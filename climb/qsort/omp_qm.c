
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"


void quick_sort (int *a, int n) {
    int i = partition(a, n);
    if (-1 == i)
        return;
    quick_sort(a, i);
    quick_sort(a + i, n - i);
}

int partition (int *a, int n) {
    int i, j, p, t;
    if (n < 2)
        return -1;
    p = a[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i] < p)
            i++;
        while (p < a[j])
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    return i;
} 

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
            //printf("%d\n", p);
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
            quick_sort(nums, q);
        if (1==j)
            quick_sort(nums + q, h - q);
        if (2==j)
            quick_sort(nums + h, q);
        else
            quick_sort(nums + h + q, c - h - q);
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

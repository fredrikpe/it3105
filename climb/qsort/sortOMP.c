
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
    
    // Makes two parts
    p = partition(nums, c);

    int pivot[2];
    // Makes four parts, in two threads.
    #pragma omp parallel for
    for (j=0;j<2;j++)
    {
        pivot[j] = partition(nums + p*j, c*j + p - 2*j*p);
    }
    // The rest in four threads (if available).
    #pragma omp parallel for
    for (j=0;j<4;j++)
    {
        if (0==j)
            quick_sort(nums, pivot[0]);
        else if (1==j)
            quick_sort(nums + pivot[0], p - pivot[0]);
        else if (2==j)
            quick_sort(nums + p, pivot[1]);
        else
            quick_sort(nums + p + pivot[1], c - p - pivot[1]);
    }

    printf("s ");
    for (j = 0; j<c; j++) {
        printf("%d ", nums[j]);
    }
    printf("\n");


    free(nums);

    return 0;
}

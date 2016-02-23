
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"

int compare (const void *a, const void *b)
{
    const int *ia = (const int *)a; // casting pointer types 
    const int *ib = (const int *)b;
    return *ia  - *ib; 
}

/*
void partition (int* v, int lower, int upper, int *pivot );

void quicksort (int *v, int start, int end ) {
    if(start < end) {
        int pivot;
        partition(v,start,end,&pivot);
        quicksort(v,start,pivot-1);
        quicksort(v,pivot+1,end);
    }
}

void partition (int* v, int lower, int upper, int *pivot )
{
    int x = v[lower];
    printf("lower: %d, upper: %d\n", lower, upper);
    int up = lower+1;   
    int down = upper;  
    while(up < down)
    {
        while((up < down) && (v[up] <= x)) up++;
        while((up < down) && (v[down] > x)) down--;
        if(up == down){ 
            printf("Break");
            break;
        }
        int tmp = v[up];
        v[up] = v[down]; 
        v[down] = tmp;
    }
    if(v[up] > x) up--;
    v[lower] = v[up]; v[up] = x;
    *pivot = up;
}*/

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

    p = partition(nums, c);

    omp_set_num_threads(2);
    #pragma omp parallel
    {
        quick_sort(nums, p);
        quick_sort(nums + p, c - p);
    }

    printf("s ");
    for (j = 0; j<c; j++) {
        printf("%d ", nums[j]);
    }
    printf("\n");


    free(nums);

    return 0;
}

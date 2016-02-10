
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
int cmpfunc (const void * a, const void * b)
{
       return ( *(int*)a - *(int*)b );
}

int main()
{
    char *line = NULL;
    size_t size;
    int read;
    int p;
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

    qsort(nums, c, sizeof(int), cmpfunc);
    
    printf("s ");
    int j;
    for (j = 0; j<c; j++) {
        printf("%d ", nums[j]);
    }
    printf("\n");
    free(nums);
    return 0;
}

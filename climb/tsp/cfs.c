

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <unistd.h>


int p; 
int *mat;
int source;
 
void read_input() { 
    char *line = NULL;
    size_t size;
    int read, b, c, d, i;
    while ((read = getline(&line, &size, stdin)) != -1) {
        if (*line == 'p') {
            p = atoi(&(line[2]));
            *mat = (int *)malloc(p * p * sizeof(int));
        } else if (*line == 'a') {
            char *tok = strtok(line, " ");
            printf("ads");
            tok = strtok(NULL, " ");
            while (tok != NULL) {
                printf("ads");
                b = atoi(tok);
                tok = strtok(NULL, " ");
                c = atoi(tok);
                tok = strtok(NULL, " ");
                d = atoi(tok);
                tok = strtok(NULL, " ");
                mat[b*p + c] = d; 
                mat[c*p + b] = d; 
            }
        } else if (*line == 'q') {
            source = atoi(&(line[2]));
        }
    }
}


int main()
{


    read_input();

    int i = 0;

    return 0;
}

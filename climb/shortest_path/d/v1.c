
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"

int *graph;
int *prev;
int last;

int N;

struct PQelem *PQ;

struct PQelem {
    unsigned int dist;
    int node;
};

struct PQelem pop() { return PQ[--last]; }

void prop_change(int nv, int node) {
    int i, op = -1, np = 0;
    for (i=0; i<last; i++) {
        //printf("node: %d, PQ[i].node :: %d\n", node, PQ[i].node);
        if (PQ[i].node == node) {
            op = i;
            continue;
        } 
        if (op != -1) { 
            if (PQ[i].dist > nv) {
                np = i;
                PQ[i-1] = PQ[i];
            } else {
                break;
            }
        }
    }
    if (np) {
        PQ[np].node = node;
        PQ[np].dist = nv;
    }
}

void ppq() {
    int i;
    for (i=0;i<last;i++) {
        printf("node: %d, dist: %d\n", PQ[i].node, PQ[i].dist);
    }
    printf("\n");
}
int contains(int node) {
    int i;
    for (i=0;i<last;i++) {
        if (PQ[i].node == node)
            return 1;
    }
    return 0;
}

int dist(int node) {
    int i;
    for (i=0;i<last;i++) {
        if (PQ[i].node == node)
            return PQ[i].dist;
    }
    printf("ERROR\n");
    return -1;
}

void print_path(int sink) {
    if (prev[sink] != -1)
        print_path(prev[sink]);
    printf("%d ", sink);
}

void dijkstra(int source, int sink) {
    prop_change(0, source);        

    while (last) {
        struct PQelem cur = pop(); 
        //remove u from Q 
        if (cur.node == sink) {
            printf("s ");
            print_path(sink);
            printf("\n");
            int j;
            return;
        }
        int i, alt;
        for (i=0; i<N; i++) { //each neighbor v of u:  where v is still in Q.
            if (graph[cur.node * N + i]) {
                if (contains(i)) {
                    alt = cur.dist + graph[cur.node*N + i];
                    if (alt < dist(i)) {
                       prop_change(alt, i); 
                       prev[i] = cur.node; 
                    }
                }
            }
        }
    }
    printf("NO SOLUTION\n");
}

int main()
{
    char buffer[4096];

    int u, v, d;
    int source, sink;
    int i, j;

    char *line = NULL;
    size_t size;
    int read;
    while ((read = getline(&line, &size, stdin)) != -1) {
        if (*line == 'p') {
            // n
            // number of nodes in a graph
            N = atoi(&(line[2]));
            graph = (int*)calloc(N*N, sizeof(int));
        } else if (*line == 'a') {
            char *t = strtok(line, " ");
            t = strtok(NULL, " ");
            u = atoi(t);
            t = strtok(NULL, " ");
            v = atoi(t);
            t = strtok(NULL, " ");
            d = atoi(t);
            graph[u*N + v] = d;
        } else if (*line == 'q') {
            char *t = strtok(line, " ");
            t = strtok(NULL, " ");
            source = atoi(t);
            t = strtok(NULL, " ");
            sink = atoi(t);

            last = N;
            prev = (int*)malloc(N * sizeof(int));
            PQ = (struct PQelem*)malloc(N * sizeof(struct PQelem));
            for (i=0; i<N; i++) {
                prev[i] = -1;
                PQ[i].dist = (unsigned int) 1 << 30;
                PQ[i].node = i;
            }
            dijkstra(source, sink);
        }
    }
    
    free(graph);
    return 0;
}

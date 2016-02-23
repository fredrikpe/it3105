
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

struct PQelem pop() { return PQ[last--]; }

void prop_change(int nv, int node) {
    int i, op = -1, np = 0;
    printf("last: %d\n", last);
    for (i=0; i<=last; i++) {
        printf("node: %d, PQ[i].node :: %d\n", node, PQ[i].node);
        if (PQ[i].node == node) {
            printf("asasdfkjasdkfj2");
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
        printf("KUUUUK\n");
        PQ[np].node = node;
        PQ[np].dist = nv;
    }
}

void ppq() {
    int i;
    for (i=0;i<=last;i++) {
        printf("node: %d, dist: %d\n", PQ[i].node, PQ[i].dist);
    }
    printf("\n");
}
int contains(int node) {
    int i;
    for (i=0;i<=last;i++) {
        if (PQ[i].node == node)
            return 1;
    }
    return 0;
}

int dist(int node) {
    int i;
    for (i=0;i<=last;i++) {
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
    printf("source: %d, sink: %d\n", source, sink);
    prop_change(0, source);                        // Distance from source to source

    while (last >= 0) {
        ppq();
        struct PQelem cur = pop(); // u ← vertex in Q with min dist[u]    // Source node will be selected first
        //remove u from Q 
        if (cur.node == sink) {
            ppq();
            printf("s ");
            print_path(sink);
            printf("\n");
            return;
        }
        int i, alt;
        for (i=0; i<N; i++) { //each neighbor v of u:           // where v is still in Q.
            if (graph[cur.node * N + i] != -1) {
                //printf("cur.node: %d, i: %d, d(n,i): %d\n", cur.node, i, graph[cur.node*n + i]);
                if (contains(i)) {
                    printf("cont: %d, dist(i): %d\n", i, dist(i));
                    alt = cur.dist + graph[cur.node*N + i];
                    if (alt < dist(i)) {//:               // A shorter path to v has been found
                       prop_change(alt, i);// dist[v] ← alt 
                       prev[i] = cur.node; 
                    }
                }
            }
        }
    }
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
            last = N-1;
            graph = (int*)malloc(N*N* sizeof(int));
            prev = (int*)malloc(N * sizeof(int));
            PQ = (struct PQelem*)malloc(N * sizeof(struct PQelem));
            for (i=0; i<N; i++) {
                for (j=0;j<N;j++) {
                    graph[i*j] = -1;
                }
                prev[i] = -1;
                PQ[i].dist = (unsigned int) 1 << 20;
                PQ[i].node = i;
            }
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

            
            dijkstra(source, sink);
        }
    }
    
    free(graph);
    return 0;
}

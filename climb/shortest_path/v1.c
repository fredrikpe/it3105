
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"

int *prev;
int last;

int N;

struct PQelem *PQ;

struct PQelem {
    unsigned int dist;
    int node;
};

struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};
 
struct AdjList
{
    struct AdjListNode *head;
};
 
struct Graph
{
    int V;
    struct AdjList* array;
};

struct Graph *G;

struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
 
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}


struct PQelem pop() { return PQ[--last]; }

void prop_change(int nv, int node) {
    int i, op = -1, np = 0;
    for (i=0; i<last; i++) {
        //printf("node: %d, PQ[i].node :: %d\n", node, PQ[i].node);
        if (PQ[i].node == node) {
            op = i;
            for (int j=i+1;j<last;j++) {
                if (PQ[j].dist > nv) {
                    np = j;
                    PQ[j-1] = PQ[j];
                } else {
                    break;
                }
            }
            break;
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
        if (cur.node == sink) {
            printf("s ");
            print_path(sink);
            printf("\n");
            return;
        }
        int alt;
        struct AdjListNode* pCrawl = G->array[cur.node].head;
        while (pCrawl != NULL) { 
            int i = pCrawl->dest;
            if (contains(i)) {
                alt = cur.dist + pCrawl->weight;
                if (alt < dist(i)) {
                   prop_change(alt, i); 
                   prev[i] = cur.node; 
                }
            }
            pCrawl = pCrawl->next;
        }
    }
    printf("NO SOLUTION\n");
}

int main()
{
    char buffer[128];

    int u, v, d;
    int source, sink;
    int i, j;

    char *line = NULL;
    size_t size;
    int read;
    while ((read = getline(&line, &size, stdin)) != -1) {
        if (*line == 'p') {
            N = atoi(&(line[2]));
            G = createGraph(N);
        } else if (*line == 'a') {
            sscanf(line+1, " %d %d %d ", &u, &v, &d);
            addEdge(G, u, v, d);
        } else if (*line == 'q') {
            sscanf(line+1, " %d %d ", &source, &sink);
            last = N;
            prev = (int*)malloc(N * sizeof(int));
            PQ = (struct PQelem*)malloc(N * sizeof(struct PQelem));
            for (i=0; i<N; i++) {
                prev[i] = -1;
                PQ[i].dist = 1 << 30;
                PQ[i].node = i;
            }
            dijkstra(source, sink);
        }
    }
    
    free(prev);
    free(PQ);
    free(G);
    return 0;
}

#include <stdio.h>
#include <limits.h>

#define MAX_V 100
#define MAX_E 1000

typedef struct {
    int src, dest, weight;
} Edge;

int main(void) {
    int V, E, source;
    Edge edges[MAX_E];
    long long dist[MAX_V];

    printf("Enter number of vertices and edges: ");
    if (scanf("%d %d", &V, &E) != 2 ||
        V < 1 || V > MAX_V || E < 0 || E > MAX_E) {
        printf("Invalid graph size.\n");
        return 1;
    }

    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < E; i++) {
        if (scanf("%d %d %d", &edges[i].src,
                  &edges[i].dest, &edges[i].weight) != 3 ||
            edges[i].src < 0 || edges[i].src >= V ||
            edges[i].dest < 0 || edges[i].dest >= V) {
            printf("Invalid edge.\n");
            return 1;
        }
    }

    printf("Enter source vertex: ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= V) {
        printf("Invalid source.\n");
        return 1;
    }

    for (int i = 0; i < V; i++)
        dist[i] = LLONG_MAX;
    dist[source] = 0;

    /* Relax all edges V - 1 times */
    for (int i = 1; i < V; i++) {
        int updated = 0;

        for (int j = 0; j < E; j++) {
            int u = edges[j].src;
            int v = edges[j].dest;
            int w = edges[j].weight;

            if (dist[u] != LLONG_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                updated = 1;
            }
        }

        if (!updated)
            break;
    }

    /* Check for a reachable negative-weight cycle */
    for (int j = 0; j < E; j++) {
        int u = edges[j].src;
        int v = edges[j].dest;
        int w = edges[j].weight;

        if (dist[u] != LLONG_MAX && dist[u] + w < dist[v]) {
            printf("Negative-weight cycle reachable from source.\n");
            return 0;
        }
    }

    printf("\nVertex\tDistance from source\n");
    for (int i = 0; i < V; i++) {
        if (dist[i] == LLONG_MAX)
            printf("%d\tINF\n", i);
        else
            printf("%d\t%lld\n", i, dist[i]);
    }

    return 0;
}
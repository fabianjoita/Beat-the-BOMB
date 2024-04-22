#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES_GRAPH 100
#define MAX_VERTICES_SUBGRAPH 100
#define MAX_LINE_LENGTH 1000

struct node {
    int dest;
    int continut_nod_graf_principal;
    struct node* next;
};

struct Graph {
    struct node* head[MAX_VERTICES_GRAPH];
};

struct Edge {
    int src, dest;
};

struct Graph* createGraph(struct Edge edges[], int num_edges, int num_vertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

    for (int i = 0; i < num_vertices; i++) {
        graph->head[i] = NULL;
    }

    for (int i = 0; i < num_edges; i++) {
        int src = edges[i].src;
        int dest = edges[i].dest;

        struct node* newnode = (struct node*)malloc(sizeof(struct node));
        newnode->dest = dest;
        newnode->next = graph->head[src];
        graph->head[src] = newnode;

    }
    return graph;
}

void printGraph(struct Graph* graph, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        struct node* ptr = graph->head[i];
        while (ptr != NULL) {
            printf("(%d -> %d) ", i, ptr->dest);
            ptr = ptr->next;
        }
        printf("\n");
    }
}

int main(void) {
    FILE *fisier = fopen("input1.csv", "r");
    if (fisier == NULL) {
        fprintf(stderr, "Nu s-a putut deschide fisierul.\n");
        return 1;
    }

    int src, dest;
    char continut_nod_graf_principal[1000];
    int num_vertices = 0;
    int num_edges = 0;

    char line[MAX_LINE_LENGTH];
    char *token;

    while (fgets(line, MAX_LINE_LENGTH, fisier) != NULL) {
        token = strtok(line, ",");
        src = atoi(token);

        token = strtok(NULL, ",");
        dest = atoi(token);

        token = strtok(NULL, ",");

        FILE *fisier_continut_quiz = fopen("quiz.txt", "r");
        if (fisier_continut_quiz == NULL) {
            fprintf(stderr, "Nu s-a putut deschide fisierul.\n");
            return 1;
        }

        if (fgets(continut_nod_graf_principal, sizeof(continut_nod_graf_principal), fisier_continut_quiz) != NULL) {
            // Nu facem conversie la int deoarece citim deja un șir de caractere
        }

        if (src > num_vertices) num_vertices = src;
        if (dest > num_vertices) num_vertices = dest;
        num_edges++;
    }

    num_vertices++;

    struct Edge edges[num_edges];
    int index = 0;

    rewind(fisier);

    while (fgets(line, MAX_LINE_LENGTH, fisier) != NULL) {
        token = strtok(line, ",");
        src = atoi(token);

        token = strtok(NULL, ",");
        dest = atoi(token);

        token = strtok(NULL, ",");

        FILE *fisier_continut_quiz = fopen("quiz.txt", "r");
        if (fisier_continut_quiz == NULL) {
            fprintf(stderr, "Nu s-a putut deschide fisierul.\n");
            return 1;
        }

        if (fgets(continut_nod_graf_principal, sizeof(continut_nod_graf_principal), fisier_continut_quiz) != NULL) {
            // Nu facem conversie la int deoarece citim deja un șir de caractere
        }

        edges[index].src = src;
        edges[index].dest = dest;
        index++;
    }

    fclose(fisier);

    struct Graph* graph = createGraph(edges, num_edges, num_vertices);

    printf("Graful:\n");
    printGraph(graph, num_vertices);
    printf("\n");

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.c"

void answerToTheQuestion(struct Graph* graph, struct Edge edges[], int num_edges, int num_vertices) {

    int number_of_lives = 3;
    int score = 0; //daca raspunzi corect la o intrebare, scorul creste cu 100

    for (int i = 0; i < num_vertices; i++) {

        struct node* ptr = graph->head[i];
        if (ptr != NULL) {

            printf("Nod principal %d: %s\n", i, ptr->continut_nod_graf_principal);
            struct node_secund* ptrs = ptr->next_secundar;

            while (ptrs != NULL) {
                printf("  - Nod secundar %d: %s\n", ptrs->dest, ptrs->continut_nod_graf_secundar);

                char answer_utilizator[2];
                printf("Answer with true(T)/false(F) : \n");
                scanf("%s", answer_utilizator);
                //printf("%s \n"ptrs->answer);
                if(strcmp(ptrs->answer ,answer_utilizator) == 0) {
                    score += 100;
                    printf("\nBravo! Scorul tau este: %d\n\n", score);
                }
                else {
                    number_of_lives --;
                    printf("\nAi gresit! Mai ai %d vieti.\n\n", number_of_lives);

                }


                ptrs = ptrs->next;
            }
             ptr = ptr->next; // Trecem la urmatorul nod principal
            printf("\n");
        }
    }
}

int main(void) {
    FILE *fisier = fopen("input1.csv", "r");
    if (fisier == NULL) {
        fprintf(stderr, "Nu s-a putut deschide fisierul .\n");
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

        if (src > num_vertices) num_vertices = src;
        if (dest > num_vertices) num_vertices = dest;
        num_edges++;
    }

    num_vertices++;
    printf("%d si %d\n", num_edges, num_vertices);

    struct Edge edges[num_edges]; // pentru graf principal
    int index = 0;

    rewind(fisier);

    // pentru graful principal
    while (fgets(line, MAX_LINE_LENGTH, fisier) != NULL) {
        token = strtok(line, ",");
        src = atoi(token);

        token = strtok(NULL, ",");
        dest = atoi(token);

        edges[index].src = src;
        edges[index].dest = dest;
        index++;
    }

    fclose(fisier);

    struct Graph* graph = createGraph(edges, num_edges, num_vertices);

    //--------------------------afisare necesara pt a vedea structura corecta a datelor
    //printf("Graful:\n");
    printGraph(graph, num_vertices);
    printf("\n");

    //am afisat graful urmeaza sa raspundem corect la intrebari
    //intai vom afisa categoria din care facem parte, apoi intrebare si Answer: input utilizator
    //daca termini de raspuns la intrebari treci mai departe la urmatoarea categorie
    answerToTheQuestion(graph, edges, num_edges, num_vertices);
    return 0;
}
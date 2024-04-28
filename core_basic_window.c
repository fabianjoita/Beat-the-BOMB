#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.c"
#include <raylib.h>

// Definirea dimensiunilor ferestrei
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800


void drawCategories(int screenWidth, int screenHeight, int numCategories, struct Graph* graph, int scrollOffset) {
    int categoryWidth = screenWidth / 2; // Lățimea ferestrei categoriilor
    int categoryPosX = 0; // Poziția x pentru fereastra categoriilor
    int categoryPosY = 0; // Poziția y pentru fereastra categoriilor

    // Desenarea fundalului ferestrei categoriilor
    DrawRectangle(categoryPosX, categoryPosY, categoryWidth, screenHeight, LIGHTGRAY);

    // Calcularea spațiului disponibil pentru desenarea categoriilor
    int availableHeight = screenHeight - categoryPosY;
    int visibleCategories = availableHeight / 20; // Numărul de categorii vizibile pe ecran

    // Desenarea fiecărei categorii în fereastră
    for (int i = scrollOffset; i < scrollOffset + visibleCategories && i < numCategories; i++) {
        struct node* ptr = graph->head[i];
        if (ptr != NULL) {
            DrawText(ptr->continut_nod_graf_principal, categoryPosX + 5, categoryPosY + 10 + (i - scrollOffset) * 30, 10, BLACK);
        }
    }
}

// Funcție pentru verificarea dacă cursorul mouse-ului se află peste un dreptunghi
bool isMouseOver(Rectangle rect) {
    return CheckCollisionPointRec(GetMousePosition(), rect);
}

// Funcție pentru tratarea răspunsului utilizatorului
void handleUserInput(char userAnswer, char correctAnswer) {
    // Verificăm dacă răspunsul utilizatorului este corect și afișăm un mesaj corespunzător
    if (correctAnswer == userAnswer) {
        DrawText("Bravo! Ai răspuns corect.", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 100, 20, DARKGREEN);
    } else {
        DrawText("Hopa! Ai răspuns greșit.", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 100, 20, RED);
    }
}

// Funcție pentru desenarea întrebării curente
void drawCurrentQuestion(int screenWidth, int screenHeight, struct node_secund currentQuestion, int *action_button) {
    // Desenarea butoanelor și textului întrebării
    DrawRectangle(screenWidth / 2 - 100 - 10, screenHeight / 2, 100, 50, GRAY);
    DrawRectangle(screenWidth / 2 + 10, screenHeight / 2, 100, 50, GRAY);
    DrawText("TRUE", screenWidth / 2 - 100 / 2 - MeasureText("TRUE", 20) / 2, screenHeight / 2 + 15, 20, BLACK);
    DrawText("FALSE", screenWidth / 2 + 100 / 2 - MeasureText("FALSE", 20) / 2, screenHeight / 2 + 15, 20, BLACK);

    // Desenarea întrebării în mijlocul ecranului
    DrawText(currentQuestion.continut_nod_graf_secundar, screenWidth / 2, screenHeight / 2 - 100, 20, DARKGREEN);

    // Verificăm dacă cursorul mouse-ului se află peste butonul "TRUE" sau "FALSE" și dacă utilizatorul a făcut clic
    Rectangle buttonARect = { screenWidth / 2 - 100 - 10, screenHeight / 2, 100, 50 };
    Rectangle buttonBRect = { screenWidth / 2 + 10, screenHeight / 2, 100, 50 };

    if (!(*action_button)) { // Așteptăm răspunsul utilizatorului doar dacă nu a fost acționat deja un buton
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (isMouseOver(buttonARect)) {
                handleUserInput('T', currentQuestion.continut_nod_graf_secundar[0]);
                (*action_button) = 1;
            } else if (isMouseOver(buttonBRect)) {
                handleUserInput('F', currentQuestion.continut_nod_graf_secundar[0]);
                (*action_button) = 1;
            }
        }
    }
}

int main(void) {
    FILE *file = fopen("input1.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Nu s-a putut deschide fișierul.\n");
        exit(EXIT_FAILURE);
    }

    int src, dest;
    int num_vertices = 0;
    int num_edges = 0;

    char line[MAX_LINE_LENGTH];
    char *token;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        token = strtok(line, ",");
        src = atoi(token);

        token = strtok(NULL, ",");
        dest = atoi(token);

        if (src > num_vertices) num_vertices = src;
        if (dest > num_vertices) num_vertices = dest;
        num_edges++;
    }

    num_vertices++;
    printf("%d și %d\n", num_edges, num_vertices);

    struct Edge edges[num_edges];
    int index = 0;

    rewind(file);

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        token = strtok(line, ",");
        src = atoi(token);

        token = strtok(NULL, ",");
        dest = atoi(token);

        edges[index].src = src;
        edges[index].dest = dest;
        index++;
    }

    fclose(file);
    struct Graph* graph = createGraph(edges, num_edges, num_vertices);

    // Inițializare fereastră
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Beat The Bomb");
    SetTargetFPS(300); // Reducem FPS-ul pentru a limita utilizarea excesivă a CPU-ului
    int question_count = 0;

    // Definirea variabilelor pentru gestionarea acțiunilor butoanelor
    int action_button = 0;
    int category_index = 0;

    drawCategories(SCREEN_WIDTH, SCREEN_HEIGHT, 20, graph, 0);


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenarea categoriilor
        drawCategories(SCREEN_WIDTH, SCREEN_HEIGHT, 21, graph, 0);

        printf("question count %d", question_count);

        if (question_count == 4) {
            category_index++;
            question_count = 0;
        }
        // Afișarea întrebării curente
        struct node* category_ptr = graph->head[category_index];
        struct node_secund* question_ptr = category_ptr->next_secundar;
        int contor = 0;

        while (question_ptr != NULL && question_count > contor) {
            question_ptr = question_ptr->next;
            contor ++;
        }

       if (question_ptr != NULL) {
            drawCurrentQuestion(SCREEN_WIDTH, SCREEN_HEIGHT, *question_ptr, &action_button);
            if (action_button == 1) {
                action_button = 0; // Resetează acțiunea butonului pentru următoarea întrebare
                question_count++;   // Avansăm la următoarea întrebare
            }
        }

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
        }
    }

    return 0;
}

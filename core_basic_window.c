//core-basic-window.c
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.c"
#include <raylib.h>


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
        exit(EXIT_FAILURE);
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
    printf("Graful:\n");
    printGraph(graph, num_vertices);
    printf("\n");

    //am afisat graful urmeaza sa raspundem corect la intrebari
    //intai vom afisa categoria din care facem parte, apoi intrebare si Answer: input utilizator
    //daca termini de raspuns la intrebari treci mai departe la urmatoarea categorie
    //answerToTheQuestion(graph, edges, num_edges, num_vertices);


    const int screenWidth = 1000;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Loading Bar Timer Example");


    // Random number initialization

    /*srand((unsigned)time(NULL));
    int num1 = rand() % 90 + 10;
    int num2 = rand() % 90 + 10;*/


    //int sum = num1 + num2;

    // Variables
    float rectWidth = 300, rectHeight = 300;
    float rectPosX = (screenWidth - rectWidth) / 2;
    float rectPosY = (screenHeight - rectHeight - 50) / 2;

    char Question[50];
    for (int i = 0; i < num_vertices; i++) {
        struct node* ptr = graph->head[i];
        if (ptr != NULL) {
            printf("Nod principal %d: %s\n", i, ptr->continut_nod_graf_principal);
            struct node_secund* ptrs = ptr->next_secundar;
            while (ptrs != NULL) {
                sprintf(Question, "%s", ptrs->continut_nod_graf_secundar);
                ptrs = ptrs->next;
            }
            ptr = ptr->next; // Trecem la urmatorul nod principal
            printf("\n");
        }
    }


    char userInput[10] = ""; // Array to store user input
    int inputIndex = 0; // Current index for user input
    bool inputSubmitted = false; // Flag to track if input has been submitted
    bool answerCorrect = false; // Flag to track if the answer is correct

    // Loading bar
    float barWidth = 600;
    float barHeight = 20;
    float barPosX = (screenWidth - barWidth) / 2;
    float barPosY = screenHeight - barHeight - 50;
    float duration = 10.0f; // Timer duration in seconds
    float startTime = GetTime();
    bool stopTimer = false; // Flag to stop the timer

    // Main game loop
    while (!WindowShouldClose()) {
        // Update timer and check for game over condition
        if (!stopTimer) {
            float currentTime = GetTime();
            float elapsedTime = currentTime - startTime;
            float progress = elapsedTime / duration;

            if (elapsedTime >= duration) {
                stopTimer = true; // End the timer when the duration is reached
            }

            // Check for user input
            int keyPressed = GetCharPressed();
            if (keyPressed >= 48 && keyPressed <= 57 && inputIndex < sizeof(userInput) - 1 && !inputSubmitted) {
                // Add the inputted digit to the user input array
                userInput[inputIndex++] = (char)keyPressed;
                userInput[inputIndex] = '\0'; // Null-terminate the string
            } else if (IsKeyPressed(KEY_BACKSPACE) && inputIndex > 0 && !inputSubmitted) {
                // Handle backspace input
                inputIndex--;
                userInput[inputIndex] = '\0'; // Null-terminate the string
            } else if (IsKeyPressed(KEY_ENTER) && !inputSubmitted) {
                // Handle enter key submission
                inputSubmitted = true;
                char userAnswer = atoi(userInput); // Convert user input to integer
                answerCorrect = (userAnswer == "T"); // Check if the answer is correct

                // Stop the timer when the answer is submitted
                stopTimer = true;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw rectangles and loading bar
        DrawRectangle(rectPosX, rectPosY, rectWidth, rectHeight, GRAY);
        DrawRectangleLinesEx((Rectangle){rectPosX, rectPosY, rectWidth, rectHeight}, 5, BLACK);
        DrawRectangle((screenWidth - 200) / 2, (screenHeight - 100) / 2 - 100, 200, 50, YELLOW);
        DrawRectangleLinesEx((Rectangle){(screenWidth - 200) / 2, (screenHeight - 100) / 2 - 100, 200, 50}, 5, ORANGE);
        DrawRectangle((screenWidth - 130) / 2, (screenHeight - 100) / 2, 130, 150, BLACK);

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                float squareWidth = 130 / 3;
                float squareHeight = 150 / 3;
                float squarePosX = (screenWidth - 130) / 2 + col * squareWidth;
                float squarePosY = (screenHeight - 100) / 2 + row * squareHeight;

                DrawRectangle(squarePosX, squarePosY, squareWidth, squareHeight, DARKGRAY);
                DrawRectangleLinesEx((Rectangle){squarePosX, squarePosY, squareWidth, squareHeight}, 3, BLACK);
            }
        }

        // Draw math expression and user input
        DrawText(Question, (screenWidth - MeasureText(Question, 20)) / 2, (screenHeight - 100) / 2 - 100 + 15, 20, BLACK);
        DrawText(userInput, (screenWidth - MeasureText(userInput, 20)) / 2 + 70, (screenHeight - 100) / 2 - 100 + 15, 20, BLUE);

        // Display feedback if the input has been submitted
        if (inputSubmitted) {
            if (answerCorrect) {
                DrawText("Correct!", (screenWidth - MeasureText("Correct!", 20)) / 2, (screenHeight - 100) / 2 - 100 + 50, 20, GREEN);
            } else {
                DrawText("Incorrect!", (screenWidth - MeasureText("Incorrect!", 20)) / 2, (screenHeight - 100) / 2 - 100 + 50, 20, RED);
            }
        }

        // Draw loading bar if the game is not over
        if (!stopTimer) {
            float currentTime = GetTime();
            float elapsedTime = currentTime - startTime;
            float progress = elapsedTime / duration;
            float filledWidth = barWidth * progress;

            DrawRectangle(barPosX, barPosY, barWidth, barHeight, LIGHTGRAY);
            DrawRectangle(barPosX, barPosY, filledWidth, barHeight, RED);

            char counterText[20];
            sprintf(counterText, "%.2f s", elapsedTime);
            int counterTextWidth = MeasureText(counterText, 20);
            DrawText(counterText, barPosX + (barWidth - counterTextWidth) / 2, barPosY + barHeight + 10, 20, BLACK);
        } else {
            // Display "Game Over" when the timer stops
            if(!answerCorrect)
                DrawText("Game Over", (screenWidth - MeasureText("Game Over", 40)) / 2, screenHeight / 2, 40, RED);
        }

        EndDrawing();
    }

    // Cleanup and close window
    CloseWindow();

    return 0;
}
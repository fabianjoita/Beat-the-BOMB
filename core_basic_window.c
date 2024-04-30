#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.c"
#include <raylib.h>
#include <time.h>

// Definirea dimensiunilor ferestrei
#define SCREEN_WIDTH GetScreenWidth()
#define SCREEN_HEIGHT GetScreenHeight()

void WrapText(const char* text, float maxWidth, float x, float y, int fontSize, Color color) {
    // Create a buffer to hold the current line
    char buffer[MAX_LINE_LENGTH];
    char wordBuffer[MAX_LINE_LENGTH];
    int bufferIndex = 0;

    // Split the text into words and accumulate their width
    const char* ptr = text;
    float currentWidth = 0.0f;
    float spaceWidth = MeasureText(" ", fontSize);

    while (*ptr != '\0') {
        // Find the next word
        int wordLength = 0;
        while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n') {
            wordBuffer[wordLength++] = *ptr++;
        }

        // Null-terminate the word
        wordBuffer[wordLength] = '\0';

        // Measure the word's width
        float wordWidth = MeasureText(wordBuffer, fontSize);

        // Check if adding the word would exceed the maxWidth
        if (currentWidth + wordWidth > maxWidth) {
            // Terminate the current line buffer
            buffer[bufferIndex] = '\0';

            // Draw the line
            DrawText(buffer, x, y, fontSize, color);

            // Move to the next line
            y += fontSize + 5; // Adjust line height as needed

            // Reset the line buffer
            bufferIndex = 0;
            currentWidth = 0.0f;
        }

        // Add the word to the current line buffer
        strncpy(&buffer[bufferIndex], wordBuffer, wordLength);
        bufferIndex += wordLength;
        currentWidth += wordWidth;

        // Add a space if necessary
        if (*ptr == ' ') {
            buffer[bufferIndex++] = ' ';
            currentWidth += spaceWidth;
            ptr++;
        }
    }

    // Draw any remaining text in the buffer
    buffer[bufferIndex] = '\0';
    DrawText(buffer, x, y, fontSize, color);
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

    printGraph(graph, num_vertices);

     // Initialization
    const int screenWidth = 1900;
    const int screenHeight = 1000;
    InitWindow(screenWidth, screenHeight, "Loading Bar Timer Example");

    int math;
    // Variables
    float rectWidth = 300, rectHeight = 300;
    float rectPosX = (screenWidth - rectWidth) / 2;
    float rectPosY = (screenHeight - rectHeight - 50) / 2;
    char mathExpression[10000];
    int category_index=0;
    char mathExpressionOld[10000];

    bool showCorrectMessage = false;
    float correctStartTime = 0.0f;

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
    float elapsedTime = 0.0f;
    bool stopTimer = false; // Flag to stop the timer
    bool gameStarted = false; // Flag to track if the game has started
    bool gameOver = false;
    int sum;

    struct node* ptr = graph->head[0];
    struct node_secund* ptrs = ptr->next_secundar;
    // Main game loop
    while (!WindowShouldClose()) {

        if (!gameStarted) {
            if (IsKeyPressed(KEY_SPACE)) {
                gameStarted = true; // Start the game when space is pressed
                gameOver = false;
                stopTimer = false; // Reset stopTimer flag
                inputIndex = 0; // Reset input index
                userInput[0] = '\0'; // Reset user input
                answerCorrect = false;
                math=0;
                sprintf(mathExpressionOld, "%s", mathExpression);
                sprintf(mathExpression, "%s",ptrs->continut_nod_graf_secundar);

                ptrs = ptrs->next;
                // Reset the timer
                startTime = GetTime();
                elapsedTime = 0.0f;
            }
        } else {


            // Update timer and check for game over condition
            if (!stopTimer) {
                float currentTime = GetTime();
                elapsedTime = currentTime - startTime;
                float progress = elapsedTime / duration;

                if (elapsedTime >= duration || category_index==20) {
                    stopTimer = true; // End the timer when the duration is reached
                }


                // Check for user input
                int keyPressed = GetCharPressed();
                // am pus asa ca voiam sa pun T/F dar am lasat deocamdata, puneti doar 0 ca raspuns
                if (keyPressed == 48 || keyPressed == 57 && inputIndex < sizeof(userInput) - 1 && !inputSubmitted) {
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
                    int userAnswer = atoi(userInput); // Convert user input to integer
                    answerCorrect = (userAnswer == sum); // Check if the answer is correct
                    math++;



                    if (answerCorrect) {
                        // Recover 5 seconds from the timer if the answer is correct
                        if(elapsedTime>5){
                            elapsedTime -= 5.0f;
                            startTime = currentTime - elapsedTime;
                        }
                        else{
                            elapsedTime= 0;
                            startTime = currentTime - elapsedTime;
                        }

                        showCorrectMessage = true;
                        correctStartTime = GetTime();
                        // Generate a new math expression


                        if(ptrs != NULL) {
                            sprintf(mathExpressionOld, "%s", mathExpression);
                            sprintf(mathExpression, "%s", ptrs->continut_nod_graf_secundar);
                            ptrs = ptrs->next;
                        }
                        else {
                            category_index++;
                            if(category_index!=20) {


                                ptr = graph->head[category_index];
                                ptrs = ptr->next_secundar;
                                sprintf(mathExpressionOld, "%s", mathExpression);
                                sprintf(mathExpression, "%s", ptrs->continut_nod_graf_secundar);

                                ptrs = ptrs->next;
                            }

                        }



                        // Reset user input
                        inputIndex = 0;
                        userInput[0] = '\0';
                        inputSubmitted = false;
                    }
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(GRAY);

        if (!gameStarted && !gameOver) {
            // Display message to press Space to start the game
            DrawText("Press SPACE to start", (screenWidth - MeasureText("Press SPACE to start", 40)) / 2, screenHeight / 2, 40, BLACK);
        } else {


            // Draw rectangles and loading bar
            //DrawRectangle(rectPosX, rectPosY, rectWidth, rectHeight, GRAY);
            // DrawRectangleLinesEx((Rectangle){rectPosX, rectPosY, rectWidth, rectHeight}, 5, BLACK);
            //DrawRectangle((screenWidth - 200) / 2, (screenHeight - 100) / 2 - 100, 200, 50, YELLOW);
            // DrawRectangleLinesEx((Rectangle){(screenWidth - 200) / 2, (screenHeight - 100) / 2 - 100, 200, 50}, 5, ORANGE);
            //DrawRectangle((screenWidth - 130) / 2, (screenHeight - 100) / 2, 130, 150, BLACK);

            // for (int row = 0; row < 3; row++) {
            //for (int col = 0; col < 3; col++) {
            //float squareWidth = 130 / 3;
            //float squareHeight = 150 / 3;
            //float squarePosX = (screenWidth - 130) / 2 + col * squareWidth;
            // float squarePosY = (screenHeight - 100) / 2 + row * squareHeight;

            //DrawRectangle(squarePosX, squarePosY, squareWidth, squareHeight, DARKGRAY);
            //DrawRectangleLinesEx((Rectangle){squarePosX, squarePosY, squareWidth, squareHeight}, 3, BLACK);
            //}
            //}

            // Draw math expression and user input
            if(!showCorrectMessage) {
                WrapText(mathExpression, 1300, 500, 365, 20, BLACK);
                DrawText(userInput, (screenWidth - MeasureText(userInput, 20)) / 2, (screenHeight - 100) / 2 - 100 + 50,
                         20, BLUE);
            }
            // Display feedback if the input has been submitted

            if(math!=0 && !answerCorrect)
                DrawText("Incorrect!", (screenWidth - MeasureText("Incorrect!", 20)) / 2, (screenHeight - 100) / 2 - 100 + 50, 20, RED);

            if (showCorrectMessage) {
                float currentCorrectTime = GetTime();
                float correctElapsedTime = currentCorrectTime - correctStartTime;

                if (correctElapsedTime < 1.0f) {
                    // Continue showing the math expression and "Correct" message
                    WrapText(mathExpressionOld, 1300, 500, 365, 20, BLACK);

                    DrawText("Correct!", (screenWidth - MeasureText("Correct!", 20)) / 2, (screenHeight - 100) / 2 - 100 - 50, 20, GREEN);
                } else {
                    // Stop showing the correct message and move on
                    showCorrectMessage = false;
                }
            }


            // Draw loading bar if the game is not over
            if (!stopTimer) {
                float progress = elapsedTime / duration;
                float filledWidth = barWidth * progress;

                DrawRectangle(barPosX, barPosY, barWidth, barHeight, LIGHTGRAY);
                DrawRectangle(barPosX, barPosY, filledWidth, barHeight, RED);

                char counterText[20];
                sprintf(counterText, "%.2f s", elapsedTime);
                int counterTextWidth = MeasureText(counterText, 20);
                DrawText(counterText, barPosX + (barWidth - counterTextWidth) / 2, barPosY + barHeight + 10, 20, BLACK);
            } else if(stopTimer==1 && category_index<=19){
                // Display "Game Over" when the timer stops
                if (stopTimer && (!inputSubmitted || !answerCorrect)) {
                    DrawText("Game Over", (screenWidth - MeasureText("Game Over", 40)) / 2, screenHeight / 2, 40, RED);
                    DrawText("Press SPACE to restart", (screenWidth - MeasureText("Press SPACE to restart", 20)) / 2, screenHeight / 2 + 150, 20, BLACK);
                    gameStarted=false;
                    gameOver=true;
                    ptr = graph->head[0];
                    ptrs = ptr->next_secundar;
                }
            }
            else if(category_index==20 && stopTimer==1){
                DrawText("You WIN!", (screenWidth - MeasureText("Game Over", 40)) / 2, screenHeight / 2, 40, GREEN);
                gameStarted=false;
                gameOver=true;
                DrawText("Press SPACE to restart", (screenWidth - MeasureText("Press SPACE to restart", 20)) / 2, screenHeight / 2 + 150, 20, BLACK);
                ptr = graph->head[0];
                ptrs = ptr->next_secundar;
            }
        }

        EndDrawing();
    }

    // Cleanup and close window
    CloseWindow();

    return 0;
}
#include "raylib.h"
#include <stdlib.h> // For random number generation
#include <time.h>   // For time function
#include <stdio.h>

int main(void) {
    // Initialization
    const int screenWidth = 1000;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Loading Bar Timer Example");


    // Random number initialization
    srand((unsigned)time(NULL));
    int num1 = rand() % 90 + 10;
    int num2 = rand() % 90 + 10;



    int sum = num1 + num2;

    // Variables
    float rectWidth = 300, rectHeight = 300;
    float rectPosX = (screenWidth - rectWidth) / 2;
    float rectPosY = (screenHeight - rectHeight - 50) / 2;
    char mathExpression[50];
    sprintf(mathExpression, "%d + %d =", num1, num2);

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
                int userAnswer = atoi(userInput); // Convert user input to integer
                answerCorrect = (userAnswer == sum); // Check if the answer is correct

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
        DrawText(mathExpression, (screenWidth - MeasureText(mathExpression, 20)) / 2, (screenHeight - 100) / 2 - 100 + 15, 20, BLACK);
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
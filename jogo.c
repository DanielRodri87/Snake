#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 10

int gameOver;
int score;
int snakeLength;
int snakePosX, snakePosY;
int fruitPosX, fruitPosY;
int tailX[100], tailY[100];

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum Direction dir;

void Setup() {
    gameOver = 0;
    dir = STOP;
    snakePosX = WIDTH / 2;
    snakePosY = HEIGHT / 2;
    fruitPosX = rand() % WIDTH;
    fruitPosY = rand() % HEIGHT;
    score = 0;
}

void Draw() {
    system("cls");

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("#");
            
            if (i == snakePosY && j == snakePosX)
                printf("O"); // Snake head
            else if (i == fruitPosY && j == fruitPosX)
                printf("F"); // Fruit
            else {
                int isTail = 0;
                for (int k = 0; k < snakeLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o"); // Snake tail
                        isTail = 1;
                    }
                }
                if (!isTail)
                    printf(" ");
            }

            if (j == WIDTH - 1)
                printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    printf("Score: %d\n", score);
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = 1;
            break;
        }
    }
}

void Algorithm() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = snakePosX;
    tailY[0] = snakePosY;

    for (int i = 1; i < snakeLength; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:
        snakePosX--;
        break;
    case RIGHT:
        snakePosX++;
        break;
    case UP:
        snakePosY--;
        break;
    case DOWN:
        snakePosY++;
        break;
    default:
        break;
    }

    if (snakePosX < 0 || snakePosX >= WIDTH || snakePosY < 0 || snakePosY >= HEIGHT)
        gameOver = 1;

    for (int i = 0; i < snakeLength; i++) {
        if (tailX[i] == snakePosX && tailY[i] == snakePosY)
            gameOver = 1;
    }

    if (snakePosX == fruitPosX && snakePosY == fruitPosY) {
        score += 10;
        fruitPosX = rand() % WIDTH;
        fruitPosY = rand() % HEIGHT;
        snakeLength++;
    }
}

int main() {
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Algorithm();
        Sleep(100);  // Delay for a smoother gameplay
    }

    printf("Game Over! Your score: %d\n", score);

    return 0;
}

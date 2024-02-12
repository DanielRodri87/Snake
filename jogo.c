#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define MAX_WIDTH 40
#define MAX_HEIGHT 20
#define MIN_WIDTH 10
#define MIN_HEIGHT 5

int WIDTH, HEIGHT;

int gameOver;
int score;
int snakeLength;
int snakePosX, snakePosY;
int fruitPosX, fruitPosY;
int tailX[MAX_WIDTH * MAX_HEIGHT];
int tailY[MAX_WIDTH * MAX_HEIGHT];
char field[MAX_HEIGHT][MAX_WIDTH];

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum Direction dir;

char ufpi_sequence[] = {'u', 'f', 'p', 'i'};
int ufpi_index = 0;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Setup() {
    gameOver = 0;
    dir = STOP;
    snakePosX = WIDTH / 2;
    snakePosY = HEIGHT / 2;
    fruitPosX = rand() % WIDTH;
    fruitPosY = rand() % HEIGHT;
    score = 0;
    snakeLength = 0;
}

void Draw() {
    system("cls");

    gotoxy(0, 0);
    for (int i = 0; i < WIDTH + 2; i++) {
        SetColor(3); 
        printf("*");
    }

    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(0, i + 1);
        SetColor(3); 
        printf("*");
        for (int j = 0; j < WIDTH; j++) {
            if (i == snakePosY && j == snakePosX) {
                SetColor(10); 
                printf("O");
            } else if (i == fruitPosY && j == fruitPosX) {
                SetColor(12); 
                printf("o");
            } else {
                int isTail = 0;
                for (int k = 0; k < snakeLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        SetColor(14); 
                        printf("%c", ufpi_sequence[(k + ufpi_index) % 4]);
                        isTail = 1;
                        break;
                    }
                }
                if (!isTail) {
                    SetColor(15); 
                    printf(" ");
                }
            }
        }
        SetColor(3); 
        printf("*\n");
    }

    for (int i = 0; i < WIDTH + 2; i++) {
        SetColor(3); 
        printf("*");
    }
    printf("\n");

    SetColor(7);
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

void ChooseFieldSize() {
    int choice;
    printf("Choose the field size:\n");
    printf("1 - Facil\n");
    printf("2 - Medio\n");
    printf("3 - Dificil\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            WIDTH = MAX_WIDTH;
            HEIGHT = MAX_HEIGHT;
            break;
        case 2:
            WIDTH = (MAX_WIDTH + MIN_WIDTH) / 2;
            HEIGHT = (MAX_HEIGHT + MIN_HEIGHT) / 2;
            break;
        case 3:
            WIDTH = MIN_WIDTH;
            HEIGHT = MIN_HEIGHT;
            break;
        default:
            printf("Invalid choice. Setting to medium size.\n");
            WIDTH = (MAX_WIDTH + MIN_WIDTH) / 2;
            HEIGHT = (MAX_HEIGHT + MIN_HEIGHT) / 2;
            break;
    }
}

int main() {
    srand(time(NULL)); 
    ChooseFieldSize();
    Setup();

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = ' ';
        }
    }

    while (!gameOver) {
        Draw();
        Input();
        Algorithm();
        Sleep(100); 
    }

    printf("Game Over! Your score: %d\n", score);

    return 0;
}

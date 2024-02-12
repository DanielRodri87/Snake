#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h> 

#pragma comment(lib, "winmm.lib")

#define LARGURA_MAX 40
#define ALTURA_MAX 20
#define LARGURA_MIN 10
#define ALTURA_MIN 5

int LARGURA, ALTURA;

int fimDeJogo;
int pontuacao;
int comprimentoCobra;
int posicaoXCobra, posicaoYCobra;
int posicaoXFruta, posicaoYFruta;
int caudaX[LARGURA_MAX * ALTURA_MAX];
int caudaY[LARGURA_MAX * ALTURA_MAX];
char campo[ALTURA_MAX][LARGURA_MAX];

enum Direcao { PARAR = 0, ESQUERDA, DIREITA, CIMA, BAIXO };
enum Direcao dir;

char sequenciaUFPI[] = {'u', 'f', 'p', 'i'};
int indiceUFPI = 0;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

void tocarEfeitoSonoro(const char* arquivoSom) {
    PlaySound(arquivoSom, NULL, SND_FILENAME | SND_ASYNC);
}

void configuracao() {
    fimDeJogo = 0;
    dir = PARAR;
    posicaoXCobra = LARGURA / 2;
    posicaoYCobra = ALTURA / 2;
    posicaoXFruta = rand() % LARGURA;
    posicaoYFruta = rand() % ALTURA;
    pontuacao = 0;
    comprimentoCobra = 0;
}

void desenhar() {
    system("cls");

    gotoxy(0, 0);
    for (int i = 0; i < LARGURA + 2; i++) {
        setColor(3); 
        printf("*");
    }

    for (int i = 0; i < ALTURA; i++) {
        gotoxy(0, i + 1);
        setColor(3); 
        printf("*");
        for (int j = 0; j < LARGURA; j++) {
            if (i == posicaoYCobra && j == posicaoXCobra) {
                setColor(10); 
                printf("O");
            } else if (i == posicaoYFruta && j == posicaoXFruta) {
                setColor(12); 
                printf("o");
            } else {
                int ehCauda = 0;
                for (int k = 0; k < comprimentoCobra; k++) {
                    if (caudaX[k] == j && caudaY[k] == i) {
                        setColor(14); 
                        printf("%c", sequenciaUFPI[(k + indiceUFPI) % 4]);
                        ehCauda = 1;
                        break;
                    }
                }
                if (!ehCauda) {
                    setColor(15); 
                    printf(" ");
                }
            }
        }
        setColor(3); 
        printf("*\n");
    }

    for (int i = 0; i < LARGURA + 2; i++) {
        setColor(3); 
        printf("*");
    }
    printf("\n");

    setColor(7);
    printf("Pontuacao: %d\n", pontuacao);
}

void entrada() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                dir = ESQUERDA;
                break;
            case 'd':
                dir = DIREITA;
                break;
            case 'w':
                dir = CIMA;
                break;
            case 's':
                dir = BAIXO;
                break;
            case 'x':
                fimDeJogo = 1;
                break;
        }
    }
}

void algoritmo() {
    int prevX = caudaX[0];
    int prevY = caudaY[0];
    int prev2X, prev2Y;
    caudaX[0] = posicaoXCobra;
    caudaY[0] = posicaoYCobra;

    for (int i = 1; i < comprimentoCobra; i++) {
        prev2X = caudaX[i];
        prev2Y = caudaY[i];
        caudaX[i] = prevX;
        caudaY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case ESQUERDA:
            posicaoXCobra--;
            break;
        case DIREITA:
            posicaoXCobra++;
            break;
        case CIMA:
            posicaoYCobra--;
            break;
        case BAIXO:
            posicaoYCobra++;
            break;
        default:
            break;
    }

    if (posicaoXCobra < 0 || posicaoXCobra >= LARGURA || posicaoYCobra < 0 || posicaoYCobra >= ALTURA)
        fimDeJogo = 1;

    for (int i = 0; i < comprimentoCobra; i++) {
        if (caudaX[i] == posicaoXCobra && caudaY[i] == posicaoYCobra)
            fimDeJogo = 1;
    }

    if (posicaoXCobra == posicaoXFruta && posicaoYCobra == posicaoYFruta) {
        pontuacao += 10;
        tocarEfeitoSonoro("comida.wav");
        posicaoXFruta = rand() % LARGURA;
        posicaoYFruta = rand() % ALTURA;
        comprimentoCobra++;
    }
}

void escolherTamanhoCampo() {
    int escolha;
    printf("Escolha o tamanho do campo:\n");
    printf("1 - Facil\n");
    printf("2 - Medio\n");
    printf("3 - Dificil\n");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            LARGURA = LARGURA_MAX;
            ALTURA = ALTURA_MAX;
            break;
        case 2:
            LARGURA = (LARGURA_MAX + LARGURA_MIN) / 2;
            ALTURA = (ALTURA_MAX + ALTURA_MIN) / 2;
            break;
        case 3:
            LARGURA = LARGURA_MIN;
            ALTURA = ALTURA_MIN;
            break;
        default:
            printf("Escolha invalida. Configurando para tamanho medio.\n");
            LARGURA = (LARGURA_MAX + LARGURA_MIN) / 2;
            ALTURA = (ALTURA_MAX + ALTURA_MIN) / 2;
            break;
    }
}

void telaInicial() {
    tocarEfeitoSonoro("musicafundo.wav"); // Toca a música de fundo
    printf("**************************************************\n");
    printf("*                                                *\n");
    printf("*          Oufpi                                 *\n");
    printf("*              u                                 *\n");
    printf("*              f       SNAKE                     *\n");
    printf("*              p                                 *\n");
    printf("*              iufpiufpiufpiufpiufpiufpiufpi     *\n");
    printf("*                                          u     *\n");
    printf("            Por favor, digite seu nome:    f     *\n");
    printf("*                                          p     *\n");
    printf("*                                          iufpi *\n");
    printf("*                                                *\n");
    printf("*                                                *\n");
    printf("**************************************************\n");
    char nome[21];
    scanf("%20s", nome);
    printf("Ola, %s! Pressione qualquer tecla para iniciar...\n", nome);
    _getch(); 
    PlaySound(NULL, NULL, 0); // Encerra a música de fundo
}

int main() {
    srand(time(NULL)); 
    telaInicial();
    escolherTamanhoCampo();
    configuracao();

    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            campo[i][j] = ' ';
        }
    }

    while (!fimDeJogo) {
        desenhar();
        entrada();
        algoritmo();
        Sleep(100); 
    }

    printf("Fim de Jogo! Sua pontuacao: %d\n", pontuacao);

    return 0;
}

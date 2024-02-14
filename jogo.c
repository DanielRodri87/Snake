#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h> 

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
char nome[21];


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


void exibirHistorico();
void telaInicial();
void escolherTamanhoCampo();
void algoritmo(FILE *arquivoPartidas);
void entrada();
void desenhar();
void configuracao();

int main() {
    srand(time(NULL)); 

    while (1) { 
        telaInicial();

        FILE *arquivoPartidas;
        arquivoPartidas = fopen("partidas.txt", "a");
        if (arquivoPartidas == NULL) {
            printf("Erro ao abrir o arquivo de partidas.\n");
            return 1;
        }

        for (int i = 0; i < ALTURA; i++) {
            for (int j = 0; j < LARGURA; j++) {
                campo[i][j] = ' ';
            }
        }

        while (!fimDeJogo) {
            desenhar();
            entrada();
            algoritmo(arquivoPartidas);

            // Ajuste do tempo de espera com base na direção da cobra
            int tempoEspera;
            if (dir == CIMA || dir == BAIXO) {
                tempoEspera = 200; // Tempo de espera mais longo para movimento vertical
            } else {
                tempoEspera = 100; // Tempo de espera mais curto para movimento horizontal
            }

            Sleep(tempoEspera); 
        }

        fclose(arquivoPartidas);

        printf("Fim de Jogo! Sua pontuação: %d\n", pontuacao);
        _getch();
    }

    return 0;
}

void exibirHistorico() {
    FILE *arquivoPartidas;
    char linha[100];

    arquivoPartidas = fopen("partidas.txt", "r");
    if (arquivoPartidas == NULL) {
        printf("Nenhuma partida registrada.\n");
        return;
    }

    printf("Histórico de Partidas:\n");
    while (fgets(linha, sizeof(linha), arquivoPartidas) != NULL) {
        printf("%s", linha);
    }

    fclose(arquivoPartidas);

    printf("\nPressione qualquer tecla para voltar ao menu inicial...");
    _getch(); 
    telaInicial();
}

void telaInicial() {
    tocarEfeitoSonoro("musicafundo.wav"); 
    printf("fuO----------------------SNAKE----------------------Ofu\n");
    printf("p                                                     p\n");
    printf("i                                                     i\n");
    printf("|                      By:                            |\n");
    printf("|                         Rodrigames                  |\n");
    printf("|                                                     |\n");
    printf("i                                                     i\n");
    printf("p                                                     p\n");
    printf("ufO----------------------SNAKE----------------------Ouf\n");

    
    printf("------------------------- MENU -------------------------\n");
    printf("1 - Jogar\n");
    printf("2 - Ver Partidas\n");
    printf("3 - Fechar\n");

    int escolha;
    scanf("%d", &escolha);
    switch (escolha) {
        case 1:
            printf("Digite seu nome: ");
            scanf("%20s", nome);
            escolherTamanhoCampo();
            configuracao();
            break;
        case 2:
            exibirHistorico();
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("Escolha invalida. Tente novamente.\n");
            break;
    }
}


void escolherTamanhoCampo() {
    int escolha;
    printf("Escolha o tamanho do campo:\n");
    printf("1 - Facil\n");
    printf("2 - Medio\n");
    printf("3 - Difícil\n");
    scanf("%d", &escolha);
    PlaySound(NULL, NULL, 0); 

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
            printf("Escolha inválida. Configurando para tamanho médio.\n");
            LARGURA = (LARGURA_MAX + LARGURA_MIN) / 2;
            ALTURA = (ALTURA_MAX + ALTURA_MIN) / 2;
            break;
    }
}

void algoritmo(FILE *arquivoPartidas) {
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

    if (fimDeJogo) {
        tocarEfeitoSonoro("derrota.wav");
        fprintf(arquivoPartidas, "Nome: %s\n", nome);
        fprintf(arquivoPartidas, "Pontuação: %d\n", pontuacao);
        fprintf(arquivoPartidas, "Tamanho do Campo: %dx%d\n", LARGURA, ALTURA);
        fprintf(arquivoPartidas, "---------------------------\n");
    }
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

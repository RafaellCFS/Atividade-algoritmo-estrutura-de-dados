#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

#define LARGURA_JANELA 900
#define ALTURA_JANELA  600
#define TAM_CELULA     30

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float   raio;
    Color   cor;
} Bola;

int **criarMatriz(int linhas, int colunas) {
    int **matriz = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = 0;
        }
    }
    return matriz;
}

void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void desenharMatriz(int **matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Color corCelula = (matriz[i][j] == 1)
                                   ? (Color){ 210, 230, 255, 255 }
                                   : (Color){ 25, 25, 35, 255 };
            DrawRectangle(j * TAM_CELULA, i * TAM_CELULA,
                           TAM_CELULA - 1, TAM_CELULA - 1, corCelula);
        }
    }
}

Bola *criarBolas(int quantidade) {
    Bola *bolas = (Bola *)malloc(quantidade * sizeof(Bola));
    for (int i = 0; i < quantidade; i++) {
        Bola *b = (bolas + i);
        b->pos  = (Vector2){ (float)GetRandomValue(50, LARGURA_JANELA - 50),
                              (float)GetRandomValue(50, ALTURA_JANELA - 50) };
        b->vel  = (Vector2){ (float)GetRandomValue(-4, 4),
                              (float)GetRandomValue(-4, 4) };
        if (b->vel.x == 0) b->vel.x = 2;
        if (b->vel.y == 0) b->vel.y = 2;
        b->raio = 10.0f;
        b->cor  = (Color){ GetRandomValue(80, 255), GetRandomValue(80, 255),
                            GetRandomValue(80, 255), 255 };
    }
    return bolas;
}

void atualizarBola(Bola *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    if (b->pos.x - b->raio < 0 || b->pos.x + b->raio > LARGURA_JANELA)
        b->vel.x *= -1;
    if (b->pos.y - b->raio < 0 || b->pos.y + b->raio > ALTURA_JANELA)
        b->vel.y *= -1;
}

void marcarCelulaVisitada(Bola *b, int **matriz, int linhas, int colunas, int *contador) {
    int coluna = (int)(b->pos.x / TAM_CELULA);
    int linha  = (int)(b->pos.y / TAM_CELULA);

    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
        if (matriz[linha][coluna] == 0) {
            matriz[linha][coluna] = 1;
            (*contador)++;
        }
    }
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 1 - Alocacao Dinamica (raylib)");
    SetTargetFPS(60);

    int linhas  = ALTURA_JANELA / TAM_CELULA;
    int colunas = LARGURA_JANELA / TAM_CELULA;

    int **grade = criarMatriz(linhas, colunas);

    int quantidadeBolas = 12;
    Bola *bolas = criarBolas(quantidadeBolas);

    int celulasVisitadas = 0;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_SPACE)) {
            quantidadeBolas++;
            Bola *novo = (Bola *)realloc(bolas, quantidadeBolas * sizeof(Bola));
            if (novo != NULL) {
                bolas = novo;
                Bola *b = (bolas + (quantidadeBolas - 1));
                b->pos  = (Vector2){ (float)GetRandomValue(50, LARGURA_JANELA - 50),
                                      (float)GetRandomValue(50, ALTURA_JANELA - 50) };
                b->vel  = (Vector2){ (float)GetRandomValue(-4, 4),
                                      (float)GetRandomValue(-4, 4) };
                if (b->vel.x == 0) b->vel.x = 2;
                if (b->vel.y == 0) b->vel.y = 2;
                b->raio = 10.0f;
                b->cor  = (Color){ GetRandomValue(80, 255), GetRandomValue(80, 255),
                                    GetRandomValue(80, 255), 255 };
            } else {
                quantidadeBolas--;
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && quantidadeBolas > 0) {
            quantidadeBolas--;
            Bola *novo = (Bola *)realloc(bolas, quantidadeBolas * sizeof(Bola));
            if (novo != NULL || quantidadeBolas == 0) {
                bolas = novo;
            }
        }

        for (int i = 0; i < quantidadeBolas; i++) {
            atualizarBola(bolas + i);
            marcarCelulaVisitada(bolas + i, grade, linhas, colunas, &celulasVisitadas);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        desenharMatriz(grade, linhas, colunas);

        for (int i = 0; i < quantidadeBolas; i++) {
            DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
        }

        DrawText(TextFormat("Bolas: %d", quantidadeBolas), 10, 10, 20, WHITE);
        DrawText(TextFormat("Celulas visitadas: %d / %d", celulasVisitadas, linhas * colunas),
                  10, 35, 20, WHITE);
        DrawText("ESPACO: adiciona bola   BACKSPACE: remove bola", 10, ALTURA_JANELA - 25, 18, GRAY);

        EndDrawing();
    }

    free(bolas);
    liberarMatriz(grade, linhas);

    CloseWindow();
    return 0;
}

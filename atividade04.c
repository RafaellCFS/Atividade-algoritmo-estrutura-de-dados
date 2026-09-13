#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define TOTAL_INIMIGOS 8
#define VIDA_INICIAL   60
#define DANO_TIRO      15
#define CURA_AREA      10

typedef enum {
    INIMIGO_VIVO,
    INIMIGO_MORTO
} EstadoInimigo;

typedef struct {
    Vector2       pos;
    float         raio;
    int           vida;
    EstadoInimigo estado;
} Inimigo;

void inicializarInimigos(Inimigo *vetor, int n) {
    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i);
        ini->pos = (Vector2){ (float)GetRandomValue(30, LARGURA_JANELA - 30),
                               (float)GetRandomValue(30, ALTURA_JANELA - 30) };
        ini->raio = 16.0f;
        ini->vida = VIDA_INICIAL;
        ini->estado = INIMIGO_VIVO;
    }
}

void atingirInimigo(Inimigo *inimigo, int dano) {
    if (inimigo == NULL || inimigo->estado == INIMIGO_MORTO) return;

    inimigo->vida -= dano;
    if (inimigo->vida <= 0) {
        inimigo->vida = 0;
        inimigo->estado = INIMIGO_MORTO;
    }
}

Inimigo *encontrarInimigoMaisProximo(Inimigo *vetor, int n, Vector2 posJogador) {
    Inimigo *maisProximo = NULL;
    float menorDistancia = 0.0f;

    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i);
        if (ini->estado == INIMIGO_MORTO) continue;

        float dx = ini->pos.x - posJogador.x;
        float dy = ini->pos.y - posJogador.y;
        float distancia = sqrtf(dx * dx + dy * dy);

        if (maisProximo == NULL || distancia < menorDistancia) {
            maisProximo = ini;
            menorDistancia = distancia;
        }
    }
    return maisProximo;
}

Inimigo *encontrarInimigoMaisFraco(Inimigo *vetor, int n) {
    Inimigo *maisFraco = NULL;

    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i);
        if (ini->estado == INIMIGO_MORTO) continue;

        if (maisFraco == NULL || ini->vida < maisFraco->vida) {
            maisFraco = ini;
        }
    }
    return maisFraco;
}

void curarTodos(Inimigo *vetor, int n, int cura) {
    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i);
        if (ini->estado == INIMIGO_MORTO) continue;

        ini->vida += cura;
        if (ini->vida > VIDA_INICIAL) ini->vida = VIDA_INICIAL;
    }
}

void desenharInimigo(Inimigo *ini) {
    if (ini->estado == INIMIGO_MORTO) return;

    Color cor = (Color){220, 60, 60, 255};
    DrawCircleV(ini->pos, ini->raio, cor);
    DrawCircleLines((int)ini->pos.x, (int)ini->pos.y, ini->raio, BLACK);

    float largura = ini->raio * 2.0f;
    float proporcao = (float)ini->vida / (float)VIDA_INICIAL;
    Rectangle fundo = { ini->pos.x - largura / 2, ini->pos.y - ini->raio - 12, largura, 6 };
    Rectangle vida = { fundo.x, fundo.y, largura * proporcao, 6 };
    DrawRectangleRec(fundo, DARKGRAY);
    DrawRectangleRec(vida, GREEN);
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 4 - Ponteiros para Struct e Vetores de Struct");
    SetTargetFPS(60);

    Vector2 jogador = {LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f};
    float raioJogador = 15.0f;
    float velocidade = 250.0f;

    Inimigo *inimigos = (Inimigo *)malloc(TOTAL_INIMIGOS * sizeof(Inimigo));
    inicializarInimigos(inimigos, TOTAL_INIMIGOS);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) jogador.x += velocidade * dt;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) jogador.x -= velocidade * dt;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) jogador.y += velocidade * dt;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) jogador.y -= velocidade * dt;

        if (jogador.x < raioJogador) jogador.x = raioJogador;
        if (jogador.x > LARGURA_JANELA - raioJogador) jogador.x = LARGURA_JANELA - raioJogador;
        if (jogador.y < raioJogador) jogador.y = raioJogador;
        if (jogador.y > ALTURA_JANELA - raioJogador) jogador.y = ALTURA_JANELA - raioJogador;

        if (IsKeyPressed(KEY_SPACE)) {
            Inimigo *alvo = encontrarInimigoMaisProximo(inimigos, TOTAL_INIMIGOS, jogador);
            atingirInimigo(alvo, DANO_TIRO);
        }

        if (IsKeyPressed(KEY_TAB)) {
            Inimigo *alvo = encontrarInimigoMaisFraco(inimigos, TOTAL_INIMIGOS);
            atingirInimigo(alvo, DANO_TIRO);
        }

        if (IsKeyPressed(KEY_C)) {
            curarTodos(inimigos, TOTAL_INIMIGOS, CURA_AREA);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < TOTAL_INIMIGOS; i++) {
            desenharInimigo(inimigos + i);
        }

        DrawCircleV(jogador, raioJogador, BLUE);
        DrawText("SETAS/WASD: mover", 10, 10, 18, DARKGRAY);
        DrawText("ESPACO: atirar no mais proximo", 10, 32, 18, DARKGRAY);
        DrawText("TAB: atirar no mais fraco", 10, 54, 18, DARKGRAY);
        DrawText("C: curar todos os inimigos vivos", 10, 76, 18, DARKGRAY);

        EndDrawing();
    }

    free(inimigos);

    CloseWindow();
    return 0;
}

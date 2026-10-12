#include "raylib.h"
#include "entidade.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define MAX_ENTIDADES  50
#define TOTAL_INIMIGOS_INICIAL 5
#define TOTAL_ITENS_INICIAL    6
#define DANO_JOGADOR 40

Entidade *vetorEntidades[MAX_ENTIDADES];
int totalEntidades = 0;

bool adicionarEntidade(Entidade *e) {
    if (e == NULL || totalEntidades >= MAX_ENTIDADES) return false;
    vetorEntidades[totalEntidades] = e;
    totalEntidades++;
    return true;
}

void removerEntidade(int indice) {
    if (indice < 0 || indice >= totalEntidades) return;

    free(vetorEntidades[indice]);
    vetorEntidades[indice] = vetorEntidades[totalEntidades - 1];
    totalEntidades--;
}

Vector2 posicaoAleatoria(void) {
    return (Vector2){ (float)GetRandomValue(30, LARGURA_JANELA - 30),
                       (float)GetRandomValue(30, ALTURA_JANELA - 30) };
}

void ordenarMaisProximoParaSegundaPosicao(Entidade *jogador) {
    if (totalEntidades < 3) return;

    int indiceMaisProximo = 1;
    float menorDistancia = 0.0f;
    bool primeiro = true;

    for (int i = 1; i < totalEntidades; i++) {
        Entidade *e = vetorEntidades[i];
        float dx = e->pos.x - jogador->pos.x;
        float dy = e->pos.y - jogador->pos.y;
        float distancia = sqrtf(dx * dx + dy * dy);

        if (primeiro || distancia < menorDistancia) {
            menorDistancia = distancia;
            indiceMaisProximo = i;
            primeiro = false;
        }
    }

    if (indiceMaisProximo != 1) {
        Entidade *tmp = vetorEntidades[1];
        vetorEntidades[1] = vetorEntidades[indiceMaisProximo];
        vetorEntidades[indiceMaisProximo] = tmp;
    }
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 8 - Modulos em C");
    SetTargetFPS(60);

    Entidade *jogador = entidadeCriar(ENTIDADE_JOGADOR, (Vector2){LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f});
    adicionarEntidade(jogador);

    for (int i = 0; i < TOTAL_INIMIGOS_INICIAL; i++) {
        adicionarEntidade(entidadeCriar(ENTIDADE_INIMIGO, posicaoAleatoria()));
    }
    for (int i = 0; i < TOTAL_ITENS_INICIAL; i++) {
        adicionarEntidade(entidadeCriar(ENTIDADE_ITEM, posicaoAleatoria()));
    }

    float velocidade = 250.0f;
    int pontos = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (entidadeEstaViva(jogador)) {
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) jogador->pos.x += velocidade * dt;
            if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) jogador->pos.x -= velocidade * dt;
            if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) jogador->pos.y += velocidade * dt;
            if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) jogador->pos.y -= velocidade * dt;
        }

        if (jogador->pos.x < jogador->raio) jogador->pos.x = jogador->raio;
        if (jogador->pos.x > LARGURA_JANELA - jogador->raio) jogador->pos.x = LARGURA_JANELA - jogador->raio;
        if (jogador->pos.y < jogador->raio) jogador->pos.y = jogador->raio;
        if (jogador->pos.y > ALTURA_JANELA - jogador->raio) jogador->pos.y = ALTURA_JANELA - jogador->raio;

        if (IsKeyPressed(KEY_N) && totalEntidades < MAX_ENTIDADES) {
            adicionarEntidade(entidadeCriar(ENTIDADE_ITEM, posicaoAleatoria()));
        }

        for (int i = totalEntidades - 1; i >= 1; i--) {
            Entidade *e = vetorEntidades[i];

            if (entidadeColidiu(jogador, e)) {
                if (e->tipo == ENTIDADE_INIMIGO) {
                    entidadeAplicarDano(jogador, e->extra.dano);
                    entidadeAplicarDano(e, DANO_JOGADOR);
                    if (!entidadeEstaViva(e)) removerEntidade(i);
                } else if (e->tipo == ENTIDADE_ITEM) {
                    pontos += e->extra.valor;
                    removerEntidade(i);
                }
            }
        }

        ordenarMaisProximoParaSegundaPosicao(jogador);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < totalEntidades; i++) {
            entidadeDesenhar(vetorEntidades[i]);
        }

        if (totalEntidades > 1) {
            DrawCircleLines((int)vetorEntidades[1]->pos.x, (int)vetorEntidades[1]->pos.y,
                             vetorEntidades[1]->raio + 5, LIME);
        }

        DrawText(TextFormat("Vida: %d", jogador->vida), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Pontos: %d", pontos), 10, 35, 20, DARKGRAY);
        DrawText(TextFormat("Entidades: %d", totalEntidades), 10, 60, 20, DARKGRAY);
        DrawText("SETAS/WASD: mover | N: spawnar item", 10, ALTURA_JANELA - 25, 16, GRAY);

        if (!entidadeEstaViva(jogador)) {
            const char *texto = "FIM DE JOGO";
            int largura = MeasureText(texto, 40);
            DrawText(texto, (LARGURA_JANELA - largura) / 2, ALTURA_JANELA / 2 - 20, 40, RED);
        }

        EndDrawing();
    }

    for (int i = 0; i < totalEntidades; i++) {
        free(vetorEntidades[i]);
    }

    CloseWindow();
    return 0;
}

#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA  600
#define QUANTIDADE_ITENS 12

typedef struct {
    float dano;
    int   alcance;
} DadosArma;

typedef struct {
    int cura;
} DadosPocao;

typedef struct {
    int absorcao;
} DadosEscudo;

typedef union {
    DadosArma   arma;
    DadosPocao  pocao;
    DadosEscudo escudo;
} DadosItem;

typedef enum {
    ITEM_ARMA,
    ITEM_POCAO,
    ITEM_ESCUDO
} TipoItem;

typedef struct {
    Vector2   pos;
    float     raio;
    TipoItem  tipo;
    DadosItem dados;
    bool      coletado;
} Item;

typedef struct {
    Vector2 pos;
    float   raio;
    int     vida;
    float   dano;
    int     armadura;
} Jogador;

Item *criarItens(int quantidade) {
    Item *itens = (Item *)malloc(quantidade * sizeof(Item));
    for (int i = 0; i < quantidade; i++) {
        Item *it = (itens + i);
        it->pos = (Vector2){ (float)GetRandomValue(30, LARGURA_JANELA - 30),
                              (float)GetRandomValue(30, ALTURA_JANELA - 30) };
        it->raio = 12.0f;
        it->coletado = false;
        it->tipo = (TipoItem)GetRandomValue(ITEM_ARMA, ITEM_ESCUDO);

        if (it->tipo == ITEM_ARMA) {
            it->dados.arma.dano = (float)GetRandomValue(2, 8);
            it->dados.arma.alcance = GetRandomValue(1, 3);
        } else if (it->tipo == ITEM_POCAO) {
            int chanceVeneno = GetRandomValue(0, 9);
            if (chanceVeneno < 3) {
                it->dados.pocao.cura = -GetRandomValue(5, 20);
            } else {
                it->dados.pocao.cura = GetRandomValue(10, 30);
            }
        } else {
            it->dados.escudo.absorcao = GetRandomValue(5, 15);
        }
    }
    return itens;
}

void aplicarItem(Jogador *j, Item *item) {
    switch (item->tipo) {
        case ITEM_ARMA:
            j->dano += item->dados.arma.dano;
            break;
        case ITEM_POCAO:
            j->vida += item->dados.pocao.cura;
            if (j->vida < 0) j->vida = 0;
            break;
        case ITEM_ESCUDO:
            j->armadura += item->dados.escudo.absorcao;
            break;
    }
    item->coletado = true;
}

bool tentarColetar(Item *item, Vector2 posJogador, float raioJogador) {
    if (item->coletado) return false;

    float dx = item->pos.x - posJogador.x;
    float dy = item->pos.y - posJogador.y;
    float distancia = (dx * dx + dy * dy);
    float somaRaios = (item->raio + raioJogador) * (item->raio + raioJogador);

    return (distancia <= somaRaios);
}

Color corDoItem(Item *item) {
    switch (item->tipo) {
        case ITEM_ARMA:
            return (Color){200, 60, 60, 255};
        case ITEM_POCAO:
            return (item->dados.pocao.cura < 0)
                ? (Color){140, 40, 200, 255}
                : (Color){60, 200, 90, 255};
        case ITEM_ESCUDO:
            return (Color){80, 140, 220, 255};
        default:
            return WHITE;
    }
}

void desenharItem(Item *item) {
    if (item->coletado) return;
    DrawCircleV(item->pos, item->raio, corDoItem(item));
    DrawCircleLines((int)item->pos.x, (int)item->pos.y, item->raio, BLACK);
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 3 - Union + Enum (Tagged Union)");
    SetTargetFPS(60);

    Jogador jogador;
    jogador.pos = (Vector2){LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f};
    jogador.raio = 15.0f;
    jogador.vida = 100;
    jogador.dano = 5.0f;
    jogador.armadura = 0;

    float velocidade = 250.0f;

    Item *itens = criarItens(QUANTIDADE_ITENS);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) jogador.pos.x += velocidade * dt;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) jogador.pos.x -= velocidade * dt;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) jogador.pos.y += velocidade * dt;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) jogador.pos.y -= velocidade * dt;

        if (jogador.pos.x < jogador.raio) jogador.pos.x = jogador.raio;
        if (jogador.pos.x > LARGURA_JANELA - jogador.raio) jogador.pos.x = LARGURA_JANELA - jogador.raio;
        if (jogador.pos.y < jogador.raio) jogador.pos.y = jogador.raio;
        if (jogador.pos.y > ALTURA_JANELA - jogador.raio) jogador.pos.y = ALTURA_JANELA - jogador.raio;

        for (int i = 0; i < QUANTIDADE_ITENS; i++) {
            Item *it = (itens + i);
            if (tentarColetar(it, jogador.pos, jogador.raio)) {
                aplicarItem(&jogador, it);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < QUANTIDADE_ITENS; i++) {
            desenharItem(itens + i);
        }

        DrawCircleV(jogador.pos, jogador.raio, BLUE);
        DrawText(TextFormat("Vida: %d", jogador.vida), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Dano: %.1f", jogador.dano), 10, 35, 20, DARKGRAY);
        DrawText(TextFormat("Armadura: %d", jogador.armadura), 10, 60, 20, DARKGRAY);
        DrawText("Setas/WASD para mover", 10, ALTURA_JANELA - 25, 16, GRAY);

        EndDrawing();
    }

    free(itens);

    CloseWindow();
    return 0;
}

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LARGURA        1000
#define ALTURA         700
#define PONTUACAO_MAX  100
#define N_INICIAL      40
#define N_MIN          10
#define N_MAX          640
#define TOPO_GRAFICO   270

typedef struct {
    char nome[16];
    int  pontuacao;
} Placar;

Placar *criarPlacares(int quantidade) {
    Placar *placares = (Placar *)malloc(quantidade * sizeof(Placar));
    for (int i = 0; i < quantidade; i++) {
        Placar *p = (placares + i);
        TextCopy(p->nome, TextFormat("J%02d", i + 1));
        p->pontuacao = GetRandomValue(10, PONTUACAO_MAX);
    }
    return placares;
}

void ordenarBubbleSort(Placar *vetor, int n, long *comparacoes, long *trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (vetor[j].pontuacao > vetor[j + 1].pontuacao) {
                Placar temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
                (*trocas)++;
            }
        }
    }
}

void ordenarInsertionSort(Placar *vetor, int n, long *comparacoes, long *trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for (int i = 1; i < n; i++) {
        Placar chave = vetor[i];
        int j = i - 1;

        while (j >= 0) {
            (*comparacoes)++;
            if (vetor[j].pontuacao > chave.pontuacao) {
                vetor[j + 1] = vetor[j];
                (*trocas)++;
                j--;
            } else {
                break;
            }
        }
        vetor[j + 1] = chave;
    }
}

int buscaSequencial(Placar *vetor, int n, int alvo, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n; i++) {
        (*comparacoes)++;
        if (vetor[i].pontuacao == alvo) return i;
    }
    return -1;
}

int buscaBinaria(Placar *vetor, int n, int alvo, long *comparacoes) {
    *comparacoes = 0;
    int inicio = 0, fim = n - 1;

    while (inicio <= fim) {
        (*comparacoes)++;
        int meio = (inicio + fim) / 2;

        if (vetor[meio].pontuacao == alvo) return meio;
        if (vetor[meio].pontuacao < alvo) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

typedef struct {
    long   compBubble, trocBubble;
    long   compInsertion, trocInsertion;
    double msBubble, msInsertion;
    long   compSeq, compBin;
    int    idxSeq, idxBin;
} Estatisticas;

void zerarOrdenacao(Estatisticas *e) {
    e->compBubble = e->trocBubble = -1;
    e->compInsertion = e->trocInsertion = -1;
    e->msBubble = e->msInsertion = -1.0;
}

void zerarBuscas(Estatisticas *e) {
    e->compSeq = e->compBin = -1;
    e->idxSeq = e->idxBin = -1;
}

void novoVetor(Placar **vetor, Placar **original, int n) {
    free(*vetor);
    free(*original);
    *vetor = criarPlacares(n);
    *original = (Placar *)malloc(n * sizeof(Placar));
    memcpy(*original, *vetor, n * sizeof(Placar));
}

int main(void) {
    InitWindow(LARGURA, ALTURA, "Atividade 7 - Complexidade: busca e ordenacao");
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));

    int n = N_INICIAL;
    Placar *vetor = NULL;
    Placar *original = NULL;
    novoVetor(&vetor, &original, n);

    bool ordenado = false;
    int alvo = vetor[GetRandomValue(0, n - 1)].pontuacao;
    char mensagem[128] = "Pressione B ou I para ordenar, S ou W para buscar.";

    Estatisticas est;
    zerarOrdenacao(&est);
    zerarBuscas(&est);

    while (!WindowShouldClose()) {

        bool regenerar = false;

        if (IsKeyPressed(KEY_R)) regenerar = true;
        if (IsKeyPressed(KEY_UP) && n * 2 <= N_MAX) { n *= 2; regenerar = true; }
        if (IsKeyPressed(KEY_DOWN) && n / 2 >= N_MIN) { n /= 2; regenerar = true; }

        if (regenerar) {
            novoVetor(&vetor, &original, n);
            ordenado = false;
            zerarOrdenacao(&est);
            zerarBuscas(&est);
            alvo = vetor[GetRandomValue(0, n - 1)].pontuacao;
            snprintf(mensagem, sizeof(mensagem), "Novo vetor com %d placares.", n);
        }

        if (IsKeyPressed(KEY_B)) {
            memcpy(vetor, original, n * sizeof(Placar));
            double inicio = GetTime();
            ordenarBubbleSort(vetor, n, &est.compBubble, &est.trocBubble);
            double fim = GetTime();
            est.msBubble = (fim - inicio) * 1000.0;
            ordenado = true;
            zerarBuscas(&est);
            snprintf(mensagem, sizeof(mensagem), "Vetor ordenado com bubble sort.");
        }

        if (IsKeyPressed(KEY_I)) {
            memcpy(vetor, original, n * sizeof(Placar));
            double inicio = GetTime();
            ordenarInsertionSort(vetor, n, &est.compInsertion, &est.trocInsertion);
            double fim = GetTime();
            est.msInsertion = (fim - inicio) * 1000.0;
            ordenado = true;
            zerarBuscas(&est);
            snprintf(mensagem, sizeof(mensagem), "Vetor ordenado com insertion sort.");
        }

        if (IsKeyPressed(KEY_U)) {
            memcpy(vetor, original, n * sizeof(Placar));
            ordenado = false;
            zerarBuscas(&est);
            snprintf(mensagem, sizeof(mensagem), "Vetor original restaurado (desordenado).");
        }

        int passo = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? 10 : 1;
        if (IsKeyPressed(KEY_LEFT))  alvo -= passo;
        if (IsKeyPressed(KEY_RIGHT)) alvo += passo;
        if (alvo < 1) alvo = 1;
        if (alvo > PONTUACAO_MAX) alvo = PONTUACAO_MAX;

        if (IsKeyPressed(KEY_T)) {
            alvo = vetor[GetRandomValue(0, n - 1)].pontuacao;
            zerarBuscas(&est);
        }

        if (IsKeyPressed(KEY_S)) {
            est.idxSeq = buscaSequencial(vetor, n, alvo, &est.compSeq);
            snprintf(mensagem, sizeof(mensagem), est.idxSeq >= 0
                     ? "Busca sequencial: alvo %d encontrado na posicao %d."
                     : "Busca sequencial: alvo %d nao encontrado.",
                     alvo, est.idxSeq);
        }

        if (IsKeyPressed(KEY_W)) {
            if (!ordenado) {
                snprintf(mensagem, sizeof(mensagem),
                         "Busca binaria exige vetor ordenado! Pressione B ou I antes.");
            } else {
                est.idxBin = buscaBinaria(vetor, n, alvo, &est.compBin);
                snprintf(mensagem, sizeof(mensagem), est.idxBin >= 0
                         ? "Busca binaria: alvo %d encontrado na posicao %d."
                         : "Busca binaria: alvo %d nao encontrado.",
                         alvo, est.idxBin);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        char linha[200];

        DrawText("Atividade 7 - Complexidade de algoritmos: busca e ordenacao", 10, 8, 20, DARKGRAY);
        snprintf(linha, sizeof(linha), "n = %d   |   alvo = %d   |   vetor %s",
                 n, alvo, ordenado ? "ORDENADO" : "DESORDENADO");
        DrawText(linha, 10, 34, 18, ordenado ? DARKGREEN : MAROON);

        if (est.compBubble >= 0)
            snprintf(linha, sizeof(linha),
                     "Bubble sort    : %ld comparacoes, %ld trocas, %.4f ms",
                     est.compBubble, est.trocBubble, est.msBubble);
        else
            snprintf(linha, sizeof(linha), "Bubble sort    : (pressione B)");
        DrawText(linha, 10, 64, 18, BLACK);

        if (est.compInsertion >= 0)
            snprintf(linha, sizeof(linha),
                     "Insertion sort : %ld comparacoes, %ld deslocamentos, %.4f ms",
                     est.compInsertion, est.trocInsertion, est.msInsertion);
        else
            snprintf(linha, sizeof(linha), "Insertion sort : (pressione I)");
        DrawText(linha, 10, 88, 18, BLACK);

        if (est.compSeq >= 0)
            snprintf(linha, sizeof(linha), "Busca sequencial: %ld comparacoes  (O(n))", est.compSeq);
        else
            snprintf(linha, sizeof(linha), "Busca sequencial: (pressione S)");
        DrawText(linha, 10, 118, 18, RED);

        if (est.compBin >= 0)
            snprintf(linha, sizeof(linha), "Busca binaria   : %ld comparacoes  (O(log n))", est.compBin);
        else
            snprintf(linha, sizeof(linha), "Busca binaria   : (pressione W, requer vetor ordenado)");
        DrawText(linha, 10, 142, 18, PURPLE);

        DrawText(mensagem, 10, 176, 18, DARKBLUE);

        DrawText("B bubble | I insertion | U original | S seq | W binaria | T sortear alvo | "
                 "<- -> alvo | CIMA/BAIXO n | R novo vetor",
                 10, 206, 14, GRAY);
        DrawText("Altura das barras proporcional a pontuacao.", 10, 226, 14, GRAY);

        int base = ALTURA - 20;
        float escala = (float)(base - TOPO_GRAFICO) / PONTUACAO_MAX;
        float larg = (float)(LARGURA - 20) / n;
        int largBarra = (larg >= 4.0f) ? (int)larg - 1 : (int)larg;
        if (largBarra < 1) largBarra = 1;

        for (int i = 0; i < n; i++) {
            int x = 10 + (int)(i * larg);
            int h = (int)(vetor[i].pontuacao * escala);
            Color cor = ordenado ? GREEN : SKYBLUE;
            if (est.compSeq >= 0 && i == est.idxSeq) cor = RED;
            if (est.compBin >= 0 && i == est.idxBin) cor = PURPLE;
            DrawRectangle(x, base - h, largBarra, h, cor);

            if (n <= 20)
                DrawText(TextFormat("%d", vetor[i].pontuacao), x, base - h - 16, 12, DARKGRAY);
        }

        int yAlvo = base - (int)(alvo * escala);
        DrawLine(10, yAlvo, LARGURA - 10, yAlvo, MAROON);
        DrawText(TextFormat("alvo %d", alvo), LARGURA - 90, yAlvo - 18, 14, MAROON);

        EndDrawing();
    }

    free(vetor);
    free(original);
    CloseWindow();
    return 0;
}

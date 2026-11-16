#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int dado;
    struct No *proximo;
} No;

typedef struct Pilha {
    No *topo;
} Pilha;

/* Inicializar a pilha: topo recebe NULL */
void inicializar(Pilha *pilha) {
    pilha->topo = NULL;
}

/* Retorna 1 se vazia, 0 caso contrario */
int estaVazia(Pilha *pilha) {
    return pilha->topo == NULL;
}

/* PUSH: insere no topo. Retorna 1 em sucesso, 0 se falhar a alocacao */
int push(Pilha *pilha, int valor) {
    No *novo = (No *) malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return 0;
    }
    novo->dado = valor;
    novo->proximo = pilha->topo;   /* novo no aponta para o antigo topo */
    pilha->topo = novo;            /* atualiza o topo */
    return 1;
}

/*
 * POP: remove o elemento do topo.
 * Estrategia: o valor removido e devolvido por ponteiro (parametro "valor")
 * e o retorno da funcao indica sucesso (1) ou falha (0, pilha vazia).
 * Assim, qualquer inteiro (inclusive -1 ou 0) pode ser um dado valido
 * sem ser confundido com um codigo de erro.
 */
int pop(Pilha *pilha, int *valor) {
    if (estaVazia(pilha)) {
        return 0;
    }
    No *remover = pilha->topo;         /* guarda o endereco do no */
    *valor = remover->dado;            /* guarda o valor */
    pilha->topo = remover->proximo;    /* atualiza o topo */
    free(remover);                     /* libera o no removido */
    return 1;
}

/* PEEK: consulta o topo sem remover. Retorna 1 em sucesso, 0 se vazia */
int peek(Pilha *pilha, int *valor) {
    if (estaVazia(pilha)) {
        return 0;
    }
    *valor = pilha->topo->dado;
    return 1;
}

/* Imprime do topo para a base usando ponteiro auxiliar */
void imprimirPilha(Pilha *pilha) {
    if (estaVazia(pilha)) {
        printf("Pilha vazia.\n");
        return;
    }
    No *aux = pilha->topo;
    printf("TOPO\n  |\n  v\n");
    while (aux != NULL) {
        printf("  %d\n  |\n", aux->dado);
        aux = aux->proximo;
    }
    printf(" NULL\n");
}

int contarElementos(Pilha *pilha) {
    int cont = 0;
    No *aux = pilha->topo;
    while (aux != NULL) {
        cont++;
        aux = aux->proximo;
    }
    return cont;
}

void liberarPilha(Pilha *pilha) {
    No *aux = pilha->topo;
    while (aux != NULL) {
        No *prox = aux->proximo;
        free(aux);
        aux = prox;
    }
    pilha->topo = NULL;
}

/* Le um inteiro com validacao; retorna 1 se leu, 0 se EOF */
static int lerInteiro(const char *msg, int *out) {
    int c;
    printf("%s", msg);
    while (scanf("%d", out) != 1) {
        if (feof(stdin)) return 0;
        while ((c = getchar()) != '\n' && c != EOF);
        if (c == EOF) return 0;
        printf("Entrada invalida. Tente novamente: ");
    }
    return 1;
}

int main() {
    Pilha pilha;
    int opcao, valor;

    inicializar(&pilha);

    do {
        printf("\n===== MENU - PILHA (LISTA ENCADEADA) =====\n");
        printf("1 - Inserir elemento (PUSH)\n");
        printf("2 - Remover elemento do topo (POP)\n");
        printf("3 - Consultar topo (PEEK)\n");
        printf("4 - Verificar se a pilha esta vazia\n");
        printf("5 - Exibir elementos\n");
        printf("6 - Quantidade de elementos\n");
        printf("7 - Liberar toda a memoria (esvaziar)\n");
        printf("0 - Sair\n");
        if (!lerInteiro("Opcao: ", &opcao)) break;

        switch (opcao) {
            case 1:
                if (!lerInteiro("Valor a inserir: ", &valor)) { opcao = 0; break; }
                if (push(&pilha, valor))
                    printf("%d inserido no topo.\n", valor);
                break;
            case 2:
                if (pop(&pilha, &valor))
                    printf("Elemento removido: %d\n", valor);
                else
                    printf("Operacao invalida: a pilha esta vazia.\n");
                break;
            case 3:
                if (peek(&pilha, &valor))
                    printf("Topo: %d\n", valor);
                else
                    printf("Nao ha elemento: a pilha esta vazia.\n");
                break;
            case 4:
                printf(estaVazia(&pilha) ? "A pilha esta vazia.\n"
                                         : "A pilha NAO esta vazia.\n");
                break;
            case 5:
                imprimirPilha(&pilha);
                break;
            case 6:
                printf("Quantidade de elementos: %d\n", contarElementos(&pilha));
                break;
            case 7:
                liberarPilha(&pilha);
                printf("Memoria liberada. Pilha vazia.\n");
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    liberarPilha(&pilha);
    return 0;
}

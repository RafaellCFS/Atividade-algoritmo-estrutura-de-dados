# Pilha em C com Lista Simplesmente Encadeada

Implementação de uma **pilha de números inteiros** na linguagem C, usando uma **lista simplesmente encadeada** com alocação dinâmica. O programa oferece um menu interativo e segue a política **LIFO** (*Last In, First Out*: o último a entrar é o primeiro a sair).

Atividade da disciplina **Algoritmos e Estruturas de Dados I**.

**Autores:** Gabriel Duarte e Rafael Caetano, 2026

---

## Funcionalidades

- Inserir um elemento no topo (PUSH)
- Remover o elemento do topo (POP)
- Consultar o topo sem remover (PEEK)
- Verificar se a pilha está vazia
- Exibir todos os elementos, do topo até a base
- Consultar a quantidade de elementos
- Liberar toda a memória utilizada

---

## Como compilar e executar

É necessário um compilador C, como o GCC.

```bash
gcc -Wall -Wextra -o pilha pilha.c
```

**Linux / macOS**
```bash
./pilha
```

**Windows (PowerShell)**
```powershell
gcc -Wall -Wextra -o pilha.exe pilha.c
.\pilha.exe
```

> No PowerShell é necessário o `.\` antes do nome do executável.

Para verificar vazamentos de memória (Linux):
```bash
valgrind --leak-check=full ./pilha
```

---

## Uso do menu

```
===== MENU - PILHA (LISTA ENCADEADA) =====
1 - Inserir elemento (PUSH)
2 - Remover elemento do topo (POP)
3 - Consultar topo (PEEK)
4 - Verificar se a pilha esta vazia
5 - Exibir elementos
6 - Quantidade de elementos
7 - Liberar toda a memoria (esvaziar)
0 - Sair
```

Exemplo de exibição com os valores 10, 20 e 30 inseridos nessa ordem:

```
TOPO
  |
  v
  30
  |
  20
  |
  10
  |
 NULL
```

---

## Estrutura de dados

```c
typedef struct No {
    int dado;
    struct No *proximo;
} No;

typedef struct Pilha {
    No *topo;
} Pilha;
```

Cada nó guarda um valor inteiro e um ponteiro para o nó inserido antes dele. O ponteiro `topo` aponta para o primeiro nó da lista (o último valor inserido). Uma pilha vazia tem `topo == NULL`.

```
topo -> [30|*] -> [20|*] -> [10|*] -> NULL
```

---

## Funções

| Função | Descrição | Retorno |
|---|---|---|
| `void inicializar(Pilha *pilha)` | Define `topo = NULL` | n/a |
| `int estaVazia(Pilha *pilha)` | Verifica se há elementos | `1` vazia, `0` não vazia |
| `int push(Pilha *pilha, int valor)` | Insere no topo | `1` sucesso, `0` falha de memória |
| `int pop(Pilha *pilha, int *valor)` | Remove o topo e devolve o valor em `*valor` | `1` sucesso, `0` pilha vazia |
| `int peek(Pilha *pilha, int *valor)` | Lê o topo sem remover | `1` sucesso, `0` pilha vazia |
| `void imprimirPilha(Pilha *pilha)` | Exibe do topo até a base | n/a |
| `int contarElementos(Pilha *pilha)` | Conta os nós | quantidade |
| `void liberarPilha(Pilha *pilha)` | Libera todos os nós e zera o topo | n/a |

### Estratégia do POP

Em `pop` o valor removido é devolvido por ponteiro (`int *valor`), e o retorno da função indica apenas sucesso (`1`) ou falha (`0`). Assim, qualquer inteiro, inclusive `-1` ou `0`, pode ser um dado válido, sem risco de ser confundido com um código de erro. O mesmo padrão é usado em `peek`.

### Cuidados com a memória

- `push` verifica se o `malloc` funcionou.
- `pop` guarda o endereço do nó antes de atualizar o topo e só depois chama `free`.
- `imprimirPilha`, `contarElementos` e `liberarPilha` usam um ponteiro auxiliar para percorrer a lista.
- `liberarPilha` guarda o próximo nó antes de liberar o atual.
- O programa chama `liberarPilha` ao sair.

---

## Complexidade

| Operação | Complexidade | Justificativa |
|---|---|---|
| `inicializar` | O(1) | Apenas atribui `NULL` |
| `push` | O(1) | Insere no início, sem percorrer a lista |
| `pop` | O(1) | Remove o primeiro nó |
| `peek` | O(1) | Lê `topo->dado` |
| `estaVazia` | O(1) | Compara `topo` com `NULL` |
| `imprimirPilha` | O(n) | Visita todos os nós |
| `contarElementos` | O(n) | Percorre toda a lista |
| `liberarPilha` | O(n) | Chama `free` em cada nó |

Inserção e remoção são O(1) porque ocorrem sempre no início da lista, que é onde o ponteiro `topo` já aponta.

---

## Testes realizados

| # | Situação | Resultado esperado |
|---|---|---|
| 1 | Inicializar pilha | Pilha vazia |
| 2 | `push(10)` | Topo igual a 10 |
| 3 | `push(20)` | Topo igual a 20 |
| 4 | `push(30)` | Topo igual a 30 |
| 5 | `pop()` | Remove 30 |
| 6 | `peek()` | Consulta 20, sem remover |
| 7 | Exibir elementos | 20 seguido de 10 |
| 8 | Contar elementos | 2 |
| 9 | Desempilhar todos | Pilha vazia |
| 10 | `pop()` em pilha vazia | Mensagem de operação inválida |
| 11 | `peek()` em pilha vazia | Mensagem de que não há elemento |
| 12 | Liberar pilha | Todos os nós liberados |

Também foram testados: inserção de um único elemento, de vários elementos, remoção sucessiva, consulta ao topo sem alteração da pilha e inserção após a pilha ficar vazia.

---

## Estrutura do projeto

```
.
├── pilha.c      # código-fonte completo
└── README.md    # este arquivo
```

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// ===================================================
//   Definições
// ===================================================
#define TAM_FILA 5
#define TAM_PILHA 3

// ===================================================
//   Struct da peça
// ===================================================
typedef struct {
    char nome[2]; // I, O, T, L etc.
    int id;
} Peca;

// ===================================================
//   Fila Circular
// ===================================================
typedef struct {
    Peca vet[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ===================================================
//   Pilha
// ===================================================
typedef struct {
    Peca vet[TAM_PILHA];
    int topo;
} Pilha;

// ===================================================
//   Geração de Peça
// ===================================================
Peca gerarPeca() {
    Peca p;
    char tipos[4] = {'I','O','T','L'};

    p.nome[0] = tipos[rand() % 4];
    p.nome[1] = '\0';
    p.id = rand() % 1000; // número aleatório de identificação

    return p;
}

// ===================================================
//   Funções da Fila
// ===================================================
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

void enqueue(Fila *f, Peca x) {
    if (filaCheia(f)) return;

    f->vet[f->fim] = x;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

Peca dequeue(Fila *f) {
    Peca aux = {"X", -1};

    if (filaVazia(f)) return aux;

    aux = f->vet[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;

    return aux;
}

// ===================================================
//   Funções da Pilha
// ===================================================
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->vet[++p->topo] = x;
}

Peca pop(Pilha *p) {
    Peca aux = {"X", -1};
    if (pilhaVazia(p)) return aux;
    return p->vet[p->topo--];
}

// ===================================================
//   Impressão da Fila e Pilha
// ===================================================
void mostrarFila(Fila *f) {
    printf("\nFila de Peças Futuras:\n");
    printf("-----------------------\n");

    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        return;
    }

    int i, pos = f->inicio;
    for (i = 0; i < f->quantidade; i++) {
        printf("[%s | id %d]  ", f->vet[pos].nome, f->vet[pos].id);
        pos = (pos + 1) % TAM_FILA;
    }
    printf("\n");
}

void mostrarPilha(Pilha *p) {
    printf("\nPilha de Peças Reservadas:\n");
    printf("--------------------------\n");

    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("[%s | id %d]\n", p->vet[i].nome, p->vet[i].id);
    }
    printf("\n");
}

// ===================================================
//   Programa Principal
// ===================================================
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++)
        enqueue(&fila, gerarPeca());

    int opcao;

    do {
        printf("\n======= TETRIS STACK - Nível Aventureiro =======\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Reservar peça (push)\n");
        printf("3 - Usar peça reservada (pop)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            // Jogar peça
            Peca jogada = dequeue(&fila);
            if (jogada.id == -1) {
                printf("\n⚠ Não há peças na fila!\n");
            } else {
                printf("\nVocê jogou a peça [%s | id %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca());
            }

        } else if (opcao == 2) {
            // Reservar peça
            if (pilhaCheia(&pilha)) {
                printf("\n⚠ Pilha cheia! Não é possível reservar mais peças.\n");
            } else {
                Peca frente = dequeue(&fila);
                push(&pilha, frente);
                printf("\nPeça [%s | id %d] reservada!\n", frente.nome, frente.id);
                enqueue(&fila, gerarPeca());
            }

        } else if (opcao == 3) {
            // Usar peça reservada
            if (pilhaVazia(&pilha)) {
                printf("\n⚠ Nenhuma peça reservada!\n");
            } else {
                Peca usada = pop(&pilha);
                printf("\nVocê usou a peça reservada [%s | id %d]\n", usada.nome, usada.id);
            }
        }

        mostrarFila(&fila);
        mostrarPilha(&pilha);

    } while (opcao != 0);

    printf("\nEncerrando programa...\n");

    return 0;
}
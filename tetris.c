#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// ===================================================
//    DEFINIÇÕES
// ===================================================
#define TAM_FILA 5
#define TAM_PILHA 3
#define MAX_HIST 20

// ===================================================
//    STRUCTS
// ===================================================
typedef struct {
    char nome[2];
    int id;
} Peca;

// --- Fila Circular ---
typedef struct {
    Peca vet[TAM_FILA];
    int inicio, fim, quantidade;
} Fila;

// --- Pilha ---
typedef struct {
    Peca vet[TAM_PILHA];
    int topo;
} Pilha;

// --- Histórico para desfazer ---
typedef struct {
    Peca jogadas[MAX_HIST];
    int topo;
} Historico;


// ===================================================
//    FUNÇÕES UTILITÁRIAS
// ===================================================
Peca gerarPeca() {
    Peca p;
    char tipos[4] = {'I','O','T','L'};
    p.nome[0] = tipos[rand() % 4];
    p.nome[1] = '\0';
    p.id = rand() % 1000;
    return p;
}

void copiarPeca(Peca *dest, Peca src) {
    strcpy(dest->nome, src.nome);
    dest->id = src.id;
}


// ===================================================
//    FILA
// ===================================================
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

int filaVazia(Fila *f) { return f->quantidade == 0; }
int filaCheia(Fila *f) { return f->quantidade == TAM_FILA; }

void enqueue(Fila *f, Peca x) {
    if (filaCheia(f)) return;
    f->vet[f->fim] = x;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

Peca dequeue(Fila *f) {
    Peca erro = {"X", -1};
    if (filaVazia(f)) return erro;
    Peca aux = f->vet[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return aux;
}


// ===================================================
//    PILHA
// ===================================================
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->vet[++p->topo] = x;
}

Peca pop(Pilha *p) {
    Peca erro = {"X", -1};
    if (pilhaVazia(p)) return erro;
    return p->vet[p->topo--];
}


// ===================================================
//    HISTÓRICO (para desfazer)
// ===================================================
void inicializarHistorico(Historico *h) {
    h->topo = -1;
}

int historicoVazio(Historico *h) { return h->topo == -1; }
int historicoCheio(Historico *h) { return h->topo == MAX_HIST - 1; }

void salvarHistorico(Historico *h, Peca x) {
    if (historicoCheio(h)) return;
    h->jogadas[++h->topo] = x;
}

Peca desfazer(Historico *h) {
    Peca erro = {"X", -1};
    if (historicoVazio(h)) return erro;
    return h->jogadas[h->topo--];
}


// ===================================================
//    EXIBIÇÃO
// ===================================================
void mostrarFila(Fila *f) {
    printf("\nFila (Próximas peças):\n-------------------------\n");
    if (filaVazia(f)) { printf("Vazia!\n"); return; }

    int i, pos = f->inicio;
    for (i = 0; i < f->quantidade; i++) {
        printf("[%s | %d] ", f->vet[pos].nome, f->vet[pos].id);
        pos = (pos + 1) % TAM_FILA;
    }
    printf("\n");
}

void mostrarPilha(Pilha *p) {
    printf("\nPilha (Reservadas):\n-------------------------\n");
    if (pilhaVazia(p)) { printf("Vazia!\n"); return; }

    for (int i = p->topo; i >= 0; i--)
        printf("[%s | %d]\n", p->vet[i].nome, p->vet[i].id);
}

void mostrarTudo(Fila *f, Pilha *p) {
    mostrarFila(f);
    mostrarPilha(p);
    printf("\n");
}


// ===================================================
//    FUNÇÕES AVANÇADAS
// ===================================================

// 4 – Trocar topo da pilha com frente da fila
void trocar(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\nNão é possível trocar (fila ou pilha vazia)\n");
        return;
    }

    Peca temp;
    copiarPeca(&temp, f->vet[f->inicio]);

    copiarPeca(&f->vet[f->inicio], p->vet[p->topo]);
    copiarPeca(&p->vet[p->topo], temp);

    printf("\nPeças trocadas com sucesso!\n");
}

// 5 – Desfazer última jogada
void desfazerJogada(Fila *f, Historico *h) {
    if (historicoVazio(h)) {
        printf("\nNada para desfazer!\n");
        return;
    }

    Peca ultima = desfazer(h);
    enqueue(f, ultima);

    printf("\nJogada desfeita! Peça [%s | %d] voltou para a fila.\n",
           ultima.nome, ultima.id);
}

// 6 – Inverter fila e pilha
void inverter(Fila *f, Pilha *p) {
    Fila novaFila;
    Pilha novaPilha;

    inicializarFila(&novaFila);
    inicializarPilha(&novaPilha);

    // Move pilha → fila
    while (!pilhaVazia(p))
        enqueue(&novaFila, pop(p));

    // Move fila → pilha
    while (!filaVazia(f))
        push(&novaPilha, dequeue(f));

    // Copia de volta
    *f = novaFila;
    *p = novaPilha;

    printf("\nFila e Pilha foram invertidas!\n");
}


// ===================================================
//    MAIN – JOGO COMPLETO
// ===================================================
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    Historico hist;

    inicializarFila(&fila);
    inicializarPilha(&pilha);
    inicializarHistorico(&hist);

    // Fila começa cheia
    for (int i = 0; i < TAM_FILA; i++)
        enqueue(&fila, gerarPeca());

    int op;

    do {
        printf("\n=========== TETRIS STACK – Nível Mestre ===========\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Desfazer última jogada\n");
        printf("6 - Inverter fila com pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op) {

            case 1: {
                Peca jogada = dequeue(&fila);

                if (jogada.id == -1) {
                    printf("\nFila vazia!\n");
                } else {
                    printf("\nVocê jogou [%s | %d]\n", jogada.nome, jogada.id);
                    salvarHistorico(&hist, jogada);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }

            case 2: {
                if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia!\n");
                } else {
                    Peca frente = dequeue(&fila);
                    push(&pilha, frente);
                    printf("\nPeça [%s | %d] reservada!\n", frente.nome, frente.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }

            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("\nNenhuma peça reservada!\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("\nVocê usou a peça [%s | %d]\n", usada.nome, usada.id);
                }
                break;
            }

            case 4:
                trocar(&fila, &pilha);
                break;

            case 5:
                desfazerJogada(&fila, &hist);
                break;

            case 6:
                inverter(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

        mostrarTudo(&fila, &pilha);

    } while (op != 0);

    return 0;
}
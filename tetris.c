
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define TAM_FILA 5

// ============================================================================
// STRUCT DA PEÇA
// ============================================================================
typedef struct {
    char nome;   // 'I', 'O', 'T', 'L' etc.
    int id;      // identificador único
} Peca;

// ============================================================================
// FILA CIRCULAR
// ============================================================================
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;   // posição da primeira peça
    int fim;      // posição onde a próxima peça será inserida
} FilaPecas;

// ============================================================================
// GERAR PEÇA AUTOMÁTICA
// ============================================================================
Peca gerarPeca() {
    Peca nova;
    char tipos[] = { 'I', 'O', 'T', 'L', 'J', 'S', 'Z' };

    nova.nome = tipos[rand() % 7];
    nova.id = rand() % 1000;   // id aleatório

    return nova;
}

// ============================================================================
// INICIALIZAR FILA
// ============================================================================
void inicializarFila(FilaPecas *fila) {
    fila->inicio = 0;
    fila->fim = 0;

    // Preencher com 5 peças automáticas
    for (int i = 0; i < TAM_FILA; i++) {
        fila->itens[i] = gerarPeca();
        fila->fim = (fila->fim + 1) % TAM_FILA;
    }
}

// ============================================================================
// EXIBIR A FILA
// ============================================================================
void exibirFila(FilaPecas *fila) {
    printf("\n======= FILA ATUAL DE PEÇAS =======\n");
    for (int i = 0; i < TAM_FILA; i++) {
        printf("Pos %d -> Peça %c (ID %d)\n",
               i, fila->itens[i].nome, fila->itens[i].id);
    }
    printf("===================================\n\n");
}

// ============================================================================
// REMOVER PEÇA (DEQUEUE)
// ============================================================================
Peca jogarPeca(FilaPecas *fila) {
    Peca removida = fila->itens[fila->inicio];

    fila->inicio = (fila->inicio + 1) % TAM_FILA;

    return removida;
}

// ============================================================================
// INSERIR PEÇA (ENQUEUE)
// ============================================================================
void inserirPeca(FilaPecas *fila) {
    Peca nova = gerarPeca();
    fila->itens[fila->fim] = nova;

    fila->fim = (fila->fim + 1) % TAM_FILA;

    printf("\nPeça %c (ID %d) inserida na fila!\n", nova.nome, nova.id);
}

// ============================================================================
// MENU PRINCIPAL
// ============================================================================
int main() {
setlocale(LC_ALL,"Portuguese_Brazil");
    srand(time(NULL));

    FilaPecas fila;
    inicializarFila(&fila);

    int opcao;

    do {
        printf("========== TETRIS STACK – NÍVEL NOVATO ==========\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("3 - Exibir fila de peças\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: {
                Peca jogada = jogarPeca(&fila);
                printf("\nPeça jogada: %c (ID %d)\n\n", jogada.nome, jogada.id);
                // Após jogar, sempre insere uma nova peça automaticamente
                inserirPeca(&fila);
                break;
            }

            case 2:
                inserirPeca(&fila);
                break;

            case 3:
                exibirFila(&fila);
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
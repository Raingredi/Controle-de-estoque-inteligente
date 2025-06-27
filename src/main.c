// Controle de Estoque Inteligente, programa feito para avaliação da disciplina estrutura de dados 1.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo as estruturas
typedef struct Produto {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    char validade[11];
    struct Produto* prox;
} Produto;

typedef struct Saida {
    int codigo;
    int quantidade;
    struct Saida* prox;
} Saida;

typedef struct Fila {
    Produto* produto;
    struct Fila* prox;
} Fila;

//LISTA LIGADA (Estoque)
Produto* criarProduto(int codigo, char nome[], int quantidade, float preco, char validade[]) {
    Produto* novo = (Produto*) malloc(sizeof(Produto));
    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    novo->quantidade = quantidade;
    novo->preco = preco;
    strcpy(novo->validade, validade);
    novo->prox = NULL;
    return novo;
}

Produto* inserirProduto(Produto* lista, Produto* novo) {
    novo->prox = lista;
    return novo;
}

Produto* removerProduto(Produto* lista, int codigo) {
    Produto *atual = lista, *anterior = NULL;
    while (atual != NULL && atual->codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL) return lista;
    if (anterior == NULL) lista = atual->prox;
    else anterior->prox = atual->prox;
    free(atual);
    return lista;
}

void listarProdutos(Produto* lista) {
    Produto* p = lista;
    printf("\n----- Produtos em Estoque -----\n");
    while (p != NULL) {
        printf("Código: %d | Nome: %s | Qtde: %d | Preço: %.2f | Validade: %s\n",
               p->codigo, p->nome, p->quantidade, p->preco, p->validade);
        p = p->prox;
    }
}

Produto* buscarProduto(Produto* lista, int codigo) {
    while (lista != NULL) {
        if (lista->codigo == codigo) return lista;
        lista = lista->prox;
    }
    return NULL;
}

//Histórico de Saídas usando pilha
Saida* registrarSaida(Saida* topo, int codigo, int quantidade) {
    Saida* nova = (Saida*) malloc(sizeof(Saida));
    nova->codigo = codigo;
    nova->quantidade = quantidade;
    nova->prox = topo;
    return nova;
}

void listarSaidas(Saida* topo) {
    printf("\n----- Histórico de Saídas -----\n");
    while (topo != NULL) {
        printf("Produto %d | Qtde retirada: %d\n", topo->codigo, topo->quantidade);
        topo = topo->prox;
    }
}

//Produtos a vencer usando fila
Fila* enfileirar(Fila* fim, Produto* produto) {
    Fila* novo = (Fila*) malloc(sizeof(Fila));
    novo->produto = produto;
    novo->prox = NULL;
    if (fim != NULL) fim->prox = novo;
    return novo;
}

Fila* desenfileirar(Fila** inicio) {
    if (*inicio == NULL) return NULL;
    Fila* temp = *inicio;
    *inicio = (*inicio)->prox;
    return temp;
}

void listarFila(Fila* inicio) {
    printf("\n----- Produtos próximos do vencimento -----\n");
    while (inicio != NULL) {
        printf("Produto: %s | Validade: %s\n", inicio->produto->nome, inicio->produto->validade);
        inicio = inicio->prox;
    }
}

//MENU
void menu() {
    printf("\n1. Adicionar produto\n");
    printf("2. Listar produtos\n");
    printf("3. Remover produto\n");
    printf("4. Registrar saída\n");
    printf("5. Ver saídas\n");
    printf("6. Marcar produto para vencimento\n");
    printf("7. Listar vencimentos\n");
    printf("0. Sair\n");
    printf("Escolha: ");
}

int main() {
    Produto* estoque = NULL;
    Saida* saidas = NULL;
    Fila *inicioFila = NULL, *fimFila = NULL;
    int opcao, codigo, quantidade;
    float preco;
    char nome[50], validade[11];

    do {
        menu();
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                printf("Código: "); scanf("%d", &codigo); getchar();
                printf("Nome: "); fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Quantidade: "); scanf("%d", &quantidade);
                printf("Preço: "); scanf("%f", &preco); getchar();
                printf("Validade (dd/mm/aaaa): "); fgets(validade, 11, stdin);
                validade[strcspn(validade, "\n")] = 0;
                estoque = inserirProduto(estoque, criarProduto(codigo, nome, quantidade, preco, validade));
                break;
            case 2:
                listarProdutos(estoque);
                break;
            case 3:
                printf("Código: "); scanf("%d", &codigo);
                estoque = removerProduto(estoque, codigo);
                break;
            case 4:
                printf("Código do produto: "); scanf("%d", &codigo);
                printf("Quantidade a retirar: "); scanf("%d", &quantidade);
                {
                    Produto* p = buscarProduto(estoque, codigo);
                    if (p && p->quantidade >= quantidade) {
                        p->quantidade -= quantidade;
                        saidas = registrarSaida(saidas, codigo, quantidade);
                        printf("Saída registrada.\n");
                    } else {
                        printf("Produto não encontrado ou quantidade insuficiente.\n");
                    }
                }
                break;
            case 5:
                listarSaidas(saidas);
                break;
            case 6:
                printf("Código do produto: "); scanf("%d", &codigo);
                {
                    Produto* p = buscarProduto(estoque, codigo);
                    if (p) fimFila = enfileirar(fimFila, p);
                    else printf("Produto não encontrado.\n");
                    if (inicioFila == NULL) inicioFila = fimFila;
                }
                break;
            case 7:
                listarFila(inicioFila);
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    // Libera memória
    while (estoque) {
        Produto* temp = estoque;
        estoque = estoque->prox;
        free(temp);
    }
    while (saidas) {
        Saida* temp = saidas;
        saidas = saidas->prox;
        free(temp);
    }
    while (inicioFila) {
        Fila* temp = inicioFila;
        inicioFila = inicioFila->prox;
        free(temp);
    }

    return 0;
}

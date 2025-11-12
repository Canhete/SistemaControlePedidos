// estrutura para produto
struct Produto{
    int codigo;                  // codigo identificador do produto     
    char descricao[200];         // descrição do produto
    float preco;                 // preço do produto
    int estoque;                 //quantidade do respectivo produto no inventario
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produto.h"

#define PRODUTO_H

#define MAX_PRODUTOS 100
#define DESCRICAO_TAMANHO 100

struct Produto {
    int id;
    char descricao[DESCRICAO_TAMANHO];
    double preco;
    int estoque;
};

void inserirProduto(Produto produtos[], int *quantidade) {
    if (*quantidade >= MAX_PRODUTOS) {
        printf("Erro: Limite máximo de produtos atingido!\n");
        return;
    }

    Produto novoProduto;

    // código do produto
    printf("Código do produto: ");
    scanf("%d", &novoProduto.id);
    getchar(); 

    // verifica se código já existe
    if (analisarProduto(produtos, *quantidade, novoProduto.id) != -1) {
        printf("Erro: Código do produto já existe!\n");
        return;
    }

    // solicitar descrição
    printf("Descrição: ");
    fgets(novoProduto.descricao, DESCRICAO_LENGTH, stdin);
    novoProduto.descricao[strcspn(novoProduto.descricao, "\n")] = 0; 

    // solicitar preço
    printf("Preço: ");
    scanf("%lf", &novoProduto.preco);

    // solicitar estoque
    printf("Quantidade em estoque: ");
    scanf("%d", &novoProduto.estoque);

    // adicionar à lista
    produtos[*quantidade] = novoProduto;
    (*quantidade)++;

    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos(Produto produtos[], int quantidade) {
    if (quantidade == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("\n-----------LISTA DE PRODUTOS-----------\n");
    printf("%-6s %-30s %-10s %-8s\n", "Código", "Descrição", "Preço", "Estoque");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < quantidade; i++) {
        printf("%-6d %-30s R$%-8.2f %-8d\n", 
               produtos[i].id, 
               produtos[i].descricao, 
               produtos[i].preco, 
               produtos[i].estoque);
    }
    printf("------------------------------------------------------------\n");
    printf("Total de produtos: %d\n", quantidade);
}

void consultarProduto(Produto produtos[], int quantidade) {
    if (quantidade == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    int id;
    printf("Código do produto a consultar: ");
    scanf("%d", &id);

    int index = analisarProduto(produtos, quantidade, id);
    if (index == -1) {
        printf("Erro: Produto não encontrado!\n");
        return;
    }

    printf("\n-----------DADOS DO PRODUTO-----------\n");
    mostrarProduto(produtos[index]);
}

void removerProduto(Produto produtos[], int *quantidade) {
    if (*quantidade == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    int id;
    printf("Código do produto a remover: ");
    scanf("%d", &id);

    int index = analisarProduto(produtos, *quantidade, id);
    if (index == -1) {
        printf("Erro: Produto não encontrado!\n");
        return;
    }

    // verificar se o produto está em algum pedido
    if (produtoExisteEmPedidos(id)) {
        printf("Erro: Produto não pode ser removido pois está em pedidos!\n");
        return;
    }

    // confirmar remoção
    printf("Produto a ser removido:\n");
    mostrarProduto(produtos[index]);

    char confirmacao;
    printf("Confirmar remoção? (s/N): ");
    scanf(" %c", &confirmacao);

    if (confirmacao == 's' || confirmacao == 'S') {
        // remover produto movendo os elementos
        for (int i = index; i < *quantidade - 1; i++) {
            produtos[i] = produtos[i + 1];
        }
        (*quantidade)--;
        printf("Produto removido com sucesso!\n");
    } else {
        printf("Remoção cancelada.\n");
    }
}

int analisarProduto(Produto produtos[], int quantidade, int id) {
    for (int i = 0; i < quantidade; i++) {
        if (produtos[i].id == id) {
            return i; // retorna o índice do produto encontrado
        }
    }
    return -1; // produto não encontrado
}

void salvarProdutosCSV(Produto produtos[], int quantidade) {
    FILE *arquivo = fopen("data/Produtos.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar produtos!\n");
        return;
    }

    fprintf(arquivo, "id;descricao;preco;estoque\n");

    for (int i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%d;%s;%.2f;%d\n",
                produtos[i].id,
                produtos[i].descricao,
                produtos[i].preco,
                produtos[i].estoque);
    }

    fclose(arquivo);
    printf("Produtos salvos em data/Produtos.csv\n");
}

void carregarProdutosCSV(Produto produtos[], int *quantidade) {
    FILE *arquivo = fopen("data/Produtos.csv", "r");
    if (arquivo == NULL) {
        printf("Arquivo de produtos não encontrado. Iniciando com lista vazia.\n");
        return;
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    *quantidade = 0;
    while (fgets(linha, sizeof(linha), arquivo) && *quantidade < MAX_PRODUTOS) {
        linha[strcspn(linha, "\n")] = 0;

        char *token = strtok(linha, ";");
        if (token == NULL) continue;

        produtos[*quantidade].id = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(produtos[*quantidade].descricao, token, DESCRICAO_LENGTH - 1);
        produtos[*quantidade].descricao[DESCRICAO_LENGTH - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        produtos[*quantidade].preco = atof(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        produtos[*quantidade].estoque = atoi(token);

        (*quantidade)++;
    }

    fclose(arquivo);
    printf("Produtos carregados: %d\n", *quantidade);
}

void mostrarProduto(Produto produto) {
    printf("Código: %d\n", produto.id);
    printf("Descrição: %s\n", produto.descricao);
    printf("Preço: R$ %.2f\n", produto.preco);
    printf("Estoque: %d unidades\n", produto.estoque);
}


#include <stdio.h>
#include <string.h>
#include <pedido.h>
#include "pedido.h"
#include <persistencia.h>
#include <utils.h>

/* Constates e definições */

/* 
Descrição de Cenários

Cadastrar Pedido - Descrição do fluxo principal/alternativo
    1. O usuário informa ao sistema o código identificador do pedido
    2. (Analisa Pedido) O sistema verifica se o respectivo código já existe. Caso exista o sistema retorna
    uma mensagem de erro informando que o código já existe e retorna para tela de cadastro com o
    cursor no campo de código para ser preenchido novamente.
    3. O usuário informa o código do cliente do referido pedido.
    4. (Analisa Cliente) O sistema verifica se o respectivo cliente já existe. Caso não exista, o sistema retorna
    uma mensagem de erro informando que o cliente não existe e retorna para tela de cadastro com o
    cursor no campo de código do cliente para ser preenchido novamente.
    5. (Cadastrar Itens de Pedido) O usuário informa o código do produto a ser cadastrado como item do
    pedido.
    6. (Cadastrar Itens de Pedido) (Analisa Produto) O sistema verifica se o produto referenciado pelo código
    existe na lista de produtos. Caso não exista, o sistema retorna uma mensagem de erro informando
*/

/* Variáveis globais inicializadas */

struct Pedido pedidoGlobal = {};
struct ItemPedido itemPedidoGlobal = {};

void cadastrarPedido(struct Pedido *P){
    const char dataAtual = obterDataAtual();
    
    printf("Id do pedido: ");
    scanf("%d", P->id);
    getchar();

    printf("Id do cliente: ");
    scanf("%d", P->clienteId);
    getchar();

    printf("Data (use o formato %s): ", dataAtual);
    fgets(P->data, sizeof(P->data), stdin);
    P->data[strcspn(P->data, "\n")] = '\0';

    printf("Total: ");
    scanf("%lf", P->total);
    getchar();

    guardarPedido(P);
}

void listarPedidos(){
    FILE *arq = fopen(NOME_ARQUIVO_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return;
    }

    char linha[TAMANHO_BUFFER_LINHA];

    printf("+------------+----------------+-------------+----------+\n");
    printf("| ID         | ID do Cliente  | Data        | Total    |\n");
    printf("+------------+----------------+-------------+----------+\n");

    while(fgets(linha, sizeof(linha), arq)){
        struct Pedido P;

        if(sscanf(linha, "%d,%49[^,],%f", &P.id, &P.clienteId, P.data, &P.total) == 4){
            printf("| %-10d | %-14d | %-11s | %-8.2f |\n", P.id, P.clienteId, P.data, P.total);
        }
    }

    printf("+------------+----------------+-------------+----------+\n");
    
    fclose(arq);
}

int analisarPedido(int idDoPedido){
    FILE *arq = fopen(NOME_ARQUIVO_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return -1;
    }

    char linha[TAMANHO_BUFFER_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct Pedido P;

        if(sscanf(linha, "%d,%49[^,],%f", &P.id, &P.clienteId, P.data, &P.total) == 4 && (P.id == idDoPedido)){
            perror("CÓDIGO DE PRODUTO JÁ EXISTE!");
            return -2;
        }
    }
    
    fclose(arq);
    return 0; // 0 -> sucesso
}

int analisarItemPedido(int idDoItemPedido){
    FILE *arq = fopen(NOME_ARQUIVO_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return -1;
    }

    char linha[TAMANHO_BUFFER_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct Pedido P;

        if(sscanf(linha, "%d,%49[^,],%f", &P.id, &P.clienteId, P.data, &P.total) == 4 && (P.id == idDoItemPedido)){
            perror("CÓDIGO DE PRODUTO JÁ EXISTE!");
            return -2;
        }
    }
    
    fclose(arq);
    return 0; // 0 = sucesso
}

void cadastrarItemPedido(struct ItemPedido *IP){
    // IMPLEMENTAR FUNÇÕES DE VALIDAÇÃO DE CADA CAMPO
    do{
        printf("Id do Pedido: ");
        scanf("%d", IP->pedidoId);
        getchar();
    } while (!analisarItemPedido(IP->pedidoId));

    printf("Id do Produto: ");
    scanf("%d", IP->produtoId);
    getchar();

    printf("Quantidade: ");
    scanf("%d", IP->quantidade);
    getchar();

    printf("Subtotal: ");
    scanf("%lf", IP->subtotal);
    getchar();

    guardarItemPedido(IP);
}


void removerPedido(){
    int idRemover;

    printf("Digite o id do pedido a ser removido: ");
    scanf("%d", &idRemover);

    apagarPedido(idRemover);
}

void removerItemPedido(){
    int idRemover;

    printf("Digite o id do item de pedido a ser removido: ");
    scanf("%d", &idRemover);

    apagarItemPedido(idRemover);
}
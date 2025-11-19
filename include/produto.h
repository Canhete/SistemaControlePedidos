/* Biblioteca dos arquivos */

#ifndef PRODUTO_H
#define PRODUTO_H

/* Bibliotecas */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DEFINIÇÕES & CONSTANTES */

#define MAX_PRODUTOS 100
#define DESCRICAO_TAMANHO 100
#define DESCRICAO_LENGTH 100

typedef struct {
    int id;
    char descricao[DESCRICAO_TAMANHO];
    double preco;
    int estoque;
} Produto;

/* FUNÇÕES */

void inserirProduto(Produto produtos[], int *quantidade);

void listarProdutos(Produto produtos[], int quantidade);

void consultarProduto(Produto produtos[], int quantidade);

void removerProduto(Produto produtos[], int *quantidade);

int analisarProduto(Produto produtos[], int quantidade, int id);

void salvarProdutosCSV(Produto produtos[], int quantidade);

void carregarProdutosCSV(Produto produtos[], int *quantidade);

void mostrarProduto(Produto produto);



#endif
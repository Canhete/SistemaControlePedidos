/* Biblioteca dos arquivos */

#ifndef PRODUTO_H
#define PRODUTO_H

/* Bibliotecas */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

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

extern Produto produtos[MAX_PRODUTOS];
extern int quantidade;

/* FUNÇÕES */

int validarIdProduto(int id, char *mensagem);

void inserirProduto(Produto produtos[], int *quantidade, WINDOW *win);

void listarProdutos(Produto produtos[], int quantidade, WINDOW *win);

void consultarProduto(Produto produtos[], int quantidade, WINDOW *win);

void removerProduto(Produto produtos[], int *quantidade, WINDOW *win);

int analisarProduto(Produto produtos[], int quantidade, int id);

void salvarProdutosCSV(Produto produtos[], int quantidade, WINDOW *win);

int carregarProdutosCSV(Produto produtos[], int *quantidade, char *mensagem);

void mostrarProduto(Produto produto, WINDOW *win);

void menuProduto();

#endif
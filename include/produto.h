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

typedef struct {
    Produto *produtos;
    int quantidade;
    int capacidade;
} GerenciadorProdutos;

extern Produto produtos[MAX_PRODUTOS];
extern int quantidade;
extern GerenciadorProdutos gerenciador;

/* FUNÇÕES */

int validarIdProduto(int id, char *mensagem);

void inserirProduto(GerenciadorProdutos *gerenciador, WINDOW *win);

void listarProdutos(GerenciadorProdutos *gerenciador, WINDOW *win);

void consultarProduto(GerenciadorProdutos *gerenciador, WINDOW *win);

void removerProduto(GerenciadorProdutos *gerenciador, WINDOW *win);

int analisarProduto(Produto *produtos, int quantidade, int id);

void salvarProdutosCSV(Produto *produtos, int quantidade, WINDOW *win);

int carregarProdutosCSV(GerenciadorProdutos *gerenciador, char *mensagem);

void mostrarProduto(Produto produto, WINDOW *win);

void menuProduto();

#endif
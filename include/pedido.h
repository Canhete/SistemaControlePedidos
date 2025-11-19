#ifndef PEDIDO_H
#define PEDIDO_H

/* Bibliotecas */

#include <stdio.h>
#include <stdlib.h>

#include "interface.h" // Importa funções de interface
#include "utils.h" // Importa funções utilitárias
#include "cliente.h" // Importa funções relacionadas a clientes
#include "produto.h" // Importa funções relacionadas a produtos
#include "estados.h" // Importa estados do sistema

/* DEFINIÇÕES */

#define MAX_PEDIDOS 100
#define MAX_ITENS_PEDIDO 500

/* Estrutura de dados do Pedido */

struct Pedido {
    int id;
    int clienteId;
    char data[11];
    double total;
};

struct ItemPedido {
    int pedidoId;
    int produtoId;
    int quantidade;
    double subtotal;
};

extern struct Pedido pedidoGlobal;
extern struct ItemPedido itemPedidoGlobal;

/* PEDIDO */

void cadastrarPedido(WINDOW *win, struct Pedido *P);

void listarPedidos(WINDOW *win);

int analisarPedido(int idDoPedido, int idDoCliente, char *mensagem);

void removerPedido(WINDOW *win);

void detalharPedido(WINDOW *win);


/* ITEM DE PEDIDOS */

void cadastrarItemPedido(WINDOW *win, struct ItemPedido *IP);

void listarItemPedidos(WINDOW *win);

int analisarItemPedido(int idDoItemPedido, char *mensagem);

void removerItemPedido(WINDOW *win, struct ItemPedido *IP);

/* Interface */

void menuPedidos();

#endif
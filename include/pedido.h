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

void cadastrarPedido(WINDOW *win, struct Pedido *P, struct ItemPedido *IP);

void listarPedidos(WINDOW *win);

void removerPedido(WINDOW *win);

void removerTodosPedidos(WINDOW *win);

void consultarPedido(WINDOW *win);

/* ITEM DE PEDIDOS */

void cadastrarItemPedido(WINDOW *win, struct Pedido *P, struct ItemPedido *IP, int primeiraVez);

int analisarItemPedido(int idDoItemPedido, char *mensagem);

void removerItemPedido(WINDOW *win);

void listarItemPedidosDoPedido(WINDOW *win, int idPedido);

void listarTodosItensPedidos(WINDOW *win);

/* Cálculos */

double calcularTotal(struct Pedido *P, char *mensagem);

double calcularSubtotal(struct ItemPedido *IP, char *mensagem);

/* Interface */

void desenhaOpcoesVertical(WINDOW *win, char *labels[], int total, int selecionado, int temFoco);

void menuPedidos();

#endif
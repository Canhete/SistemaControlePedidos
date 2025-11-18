#ifndef PEDIDO_H
#define PEDIDO_H

/* Bibliotecas */

#include "persistencia.h"
#include "utils.h"
#include "estados.h"
#include "interface.h"

/* Constantes e definições */



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

/* PEDIDO */

void cadastrarPedido(struct Pedido *P);

void listarPedidos();

void removerPedido();

int analisarPedido(int idDoPedido);

int analisarItemPedido(int idDoItemPedido);

void detalharPedido();

/* ITEM DE PEDIDOS */

void cadastrarItemPedido(struct ItemPedido *IP);

void removerItemPedido(struct ItemPedido *IP);


#endif
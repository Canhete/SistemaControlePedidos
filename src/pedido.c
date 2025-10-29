#include <stdio.h>
#include <pedido.h>

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

void cadastrarPedido(struct Pedido *P){
    
}

void listarPedidos(struct Pedido *P){

}

void removerPedido(struct Pedido *P){

}
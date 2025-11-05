/* Biblioteca de pedido */

#ifndef pedido
#define pedido

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

void cadastrarPedido(struct Pedido *P);

void listarPedidos();

void removerPedido();

int analisarPedido(int idDoPedido);

int analisarItemPedido(int idDoItemPedido);

/* ITEM DE PEDIDOS */

void cadastrarItemPedido(struct ItemPedido *IP);

void removerItemPedido(struct ItemPedido *IP);


#endif
#include "../include/estados.h"
#include "../include/interface.h"
#include "../include/cliente.h"
#include "../include/produto.h"
#include "../include/pedido.h"

// ===============================================================================================================
//  CADA ESTADO IMPLICA NUMA FUNÇÃO ESPECIFICA, ESSA FUNÇÃO FAZ ISSO E DISTRIBUI CADA ESTADO PARA UMA FUNÇÃO
// ===============================================================================================================

struct Cliente; // Declaração externa do array de clientes
int qtd_cliente;           // Declaração externa da quantidade de clientes

Estado estado_atual; // Declaração externa do estado atual
WINDOW *win;    // Declaração externa da janela global

void processaEstado(Estado st){
    switch(st){
        case ST_ABRINDO:
            inicializaInterface();

        case ST_MENU_PRINCIPAL:
            animacaoAbertura();
            menuPrincipal(win);

        // ================================================
        //               MENU CLIENTE
        // ================================================

        case ST_CLIENTE_PRINCIPAL:
            menuClientes();

        case ST_CLIENTE_CADASTRO:
            cadastrarCliente(clientes, &qtd_clientes);

        case ST_CLIENTE_LISTA:
            listarClientes(clientes, qtd_clientes);

        // ================================================
        //               MENU PRODUTO
        // ================================================

        case ST_PRODUTO_PRINCIPAL:
            //menuProdutos();

        // ================================================
        //               MENU PEDIDO
        // ================================================

        case ST_PEDIDO_PRINCIPAL:
            menuPedidos();

        case ST_PEDIDO_CADASTRO:
            cadastrarPedido(win, &pedidoGlobal);

        case ST_PEDIDO_LISTA:
            listarPedidos(win);

        case ST_PEDIDO_DETALHES:
            detalharPedido(win);

        case ST_PEDIDO_APAGAR:
            removerPedido(win);

        case ST_ITEM_PEDIDO_CADASTRO:
            cadastrarItemPedido(win, &itemPedidoGlobal);

        case ST_ITEM_PEDIDO_LISTA:
            listarItemPedidos(win);

        case ST_ITEM_PEDIDO_DETALHES:
            //detalharItemPedido(win);

        case ST_ITEM_PEDIDO_APAGAR:
            removerItemPedido(win, &itemPedidoGlobal);

        // ================================================
        //               MENU OUTROS
        // ================================================

        case ST_CONFIGURACAO:
            //menuConfiguracao();

        case ST_SAINDO:
            finalizaInterface();
            printf("Programa finalizado com sucesso!\n");
            exit(EXIT_SUCCESS);

        case ST_ERRO:
            finalizaInterface();
            printf("Ocorreu um erro inesperado! O programa será encerrado.\n");
            exit(EXIT_FAILURE);

        default:
            estado_atual = ST_ERRO;
            finalizaInterface();
            printf("Estado desconhecido! O programa será encerrado.\n");
            exit(EXIT_FAILURE);
    }
}

void executaEstado(){
    while(estado_atual != ST_SAINDO || estado_atual != ST_ERRO){
        processaEstado(estado_atual);
    }
}
#include "../include/estados.h"
#include "../include/interface.h"
#include "../include/cliente.h"
#include "../include/produto.h"
#include "../include/pedido.h"

// ===============================================================================================================
//  CADA ESTADO IMPLICA NUMA FUNÇÃO ESPECIFICA, ESSA FUNÇÃO FAZ ISSO E DISTRIBUI CADA ESTADO PARA UMA FUNÇÃO
// ===============================================================================================================

struct Cliente;         // Declaração externa do array de clientes
int qtd_cliente;        // Declaração externa da quantidade de clientes

Estado estado_atual;    // Declaração externa do estado atual
WINDOW *win;            // Declaração externa da janela global

void processaEstado(Estado st){
    switch(st){
        case ST_ABRINDO:
            inicializaInterface();
            estado_atual = ST_MENU_PRINCIPAL;
            break;

        case ST_MENU_PRINCIPAL:
            animacaoAbertura();
            menuPrincipal(win);
            break;

        // ================================================
        //               MENU CLIENTE
        // ================================================

        case ST_CLIENTE_PRINCIPAL:
            menuClientes();
            break;

        case ST_CLIENTE_CADASTRO:
            cadastrarCliente(clientes, &qtd_clientes);
            break;

        case ST_CLIENTE_LISTA:
            listarClientes(clientes, qtd_clientes);
            break;

        // ================================================
        //               MENU PRODUTO
        // ================================================

        case ST_PRODUTO_PRINCIPAL:
            //menuProdutos();
            break;

        // ================================================
        //               MENU PEDIDO
        // ================================================

        case ST_PEDIDO_PRINCIPAL:
            menuPedidos();
            break;

        case ST_PEDIDO_CADASTRO:
            break;

        case ST_PEDIDO_LISTA:
            break;

        case ST_PEDIDO_CONSULTA:
            break;

        case ST_PEDIDO_APAGAR:
            break;

        // ================================================
        //              ITEM DE PEDIDOS
        // ================================================

        case ST_ITEM_PEDIDO_CADASTRO:
            break;

        case ST_ITEM_PEDIDO_LISTA:
            break;

        case ST_ITEM_PEDIDO_CONSULTA:
            break;

        case ST_ITEM_PEDIDO_APAGAR:
            break;

        // ================================================
        //               MENU OUTROS
        // ================================================

        case ST_CONFIGURACAO:
            //menuConfiguracao();
            break;

        case ST_SAINDO:
            finalizaInterface();
            printf("Programa finalizado com sucesso!\n");
            exit(EXIT_SUCCESS);
            break;

        case ST_ERRO:
            finalizaInterface();
            printf("Ocorreu um erro inesperado! O programa será encerrado.\n");
            exit(EXIT_FAILURE);
            break;

        default:
            finalizaInterface();
            printf("Estado desconhecido! O programa será encerrado.\n");
            exit(EXIT_FAILURE);
            break;
    }
}

void executaEstado(){
    while(estado_atual != ST_SAINDO || estado_atual != ST_ERRO){
        processaEstado(estado_atual);
    }
}
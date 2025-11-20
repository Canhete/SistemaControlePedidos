#include "../include/estados.h"
#include "../include/interface.h"
#include "../include/cliente.h"
#include "../include/produto.h"
#include "../include/pedido.h"

// ===============================================================================================================
//  CADA ESTADO IMPLICA NUMA FUNÇÃO ESPECIFICA, ESSA FUNÇÃO FAZ ISSO E DISTRIBUI CADA ESTADO PARA UMA FUNÇÃO
// ===============================================================================================================

Estado estado_atual; // Declaração externa do estado atual

void processaEstado(Estado st){
    switch(st){
        case ST_ABRINDO:
            inicializaInterface();
            estado_atual = ST_MENU_PRINCIPAL;
            break;

        case ST_MENU_PRINCIPAL:
            animacaoAbertura();
            menuPrincipal();
            break;
        
        // =========================================================
        //                  ESTADOS CLIENTE
        // =========================================================

        case ST_CLIENTE_PRINCIPAL:
            menuClientes();
            break;

        case ST_CLIENTE_CADASTRO:
            cadastrarCliente(cliente_global, &qtd_clientes_global);
            break;

        case ST_CLIENTE_LISTA:
            listarClientes(cliente_global, qtd_clientes_global);
            break;

        case ST_CLIENTE_APAGAR:
            //apagarCliente();
            break;

        // =========================================================
        //                  ESTADOS PRODUTO
        // =========================================================

        case ST_PRODUTO_PRINCIPAL:
            //menuProdutos();
            break;

        // =========================================================
        //                  ESTADOS PEDIDO
        // =========================================================

        case ST_PEDIDO_PRINCIPAL:
            menuPedidos();
            break;

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
    }
}

void executaEstado(){
    while(estado_atual != ST_SAINDO || estado_atual != ST_ERRO){
        processaEstado(estado_atual);
    }
}
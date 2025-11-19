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

        case ST_MENU_PRINCIPAL:
            animacaoAbertura();
            menuPrincipal();

        case ST_CLIENTE_PRINCIPAL:
            menuClientes();

        case ST_PRODUTO_PRINCIPAL:
            //menuProdutos();

        case ST_PEDIDO_PRINCIPAL:
            menuPedidos();

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
    }
}

void executaEstado(){
    while(estado_atual != ST_SAINDO || estado_atual != ST_ERRO){
        processaEstado(estado_atual);
    }
}
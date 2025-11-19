#include "../include/estados.h"
#include "../include/interface.h"
#include "../include/cliente.h"
#include "../include/produto.h"
#include "../include/pedido.h"

// ===============================================================================================================
//  CADA ESTADO IMPLICA NUMA FUNÇÃO ESPECIFICA, ESSA FUNÇÃO FAZ ISSO E DISTRIBUI CADA ESTADO PARA UMA FUNÇÃO
// ===============================================================================================================

Estado estado_atual; // Declaração externa do estado atual

Estado processaEstado(Estado st){
    switch(st){
        case ST_ABRINDO:
            inicializaInterface();
            return ST_MENU_PRINCIPAL;

        case ST_MENU_PRINCIPAL:
            animacaoAbertura();
            menuPrincipal();
            return ST_SAINDO;

        case ST_CLIENTE_PRINCIPAL:
            menuClientes();
            return ST_MENU_PRINCIPAL;

        case ST_PRODUTO_PRINCIPAL:
            //menuProdutos();
            return ST_MENU_PRINCIPAL;

        case ST_PEDIDO_PRINCIPAL:
            menuPedidos();
            return ST_MENU_PRINCIPAL;

        case ST_CONFIGURACAO:
            //menuConfiguracao();
            return ST_MENU_PRINCIPAL;

        case ST_SAINDO:
            finalizaInterface();
            printf("Programa finalizado com sucesso!\n");
            exit(EXIT_SUCCESS);
            return ST_SAINDO;

        case ST_ERRO:
            finalizaInterface();
            printf("Ocorreu um erro inesperado! O programa será encerrado.\n");
            exit(EXIT_FAILURE);
            return ST_SAINDO;

        default:
            return ST_ERRO;
    }
}

void executaEstado(){
    while(estado_atual != ST_SAINDO){
        estado_atual = processaEstado(estado_atual);
    }
}
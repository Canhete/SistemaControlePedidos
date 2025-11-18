#include "../include/estados.h"

Estado estado_atual = ST_TELA_PRINCIPAL;

// ===============================================================================================================
//  CADA ESTADO IMPLICA NUMA FUNÇÃO ESPECIFICA, ESSA FUNÇÃO FAZ ISSO E DISTRIBUI CADA ESTADO PARA UMA FUNÇÃO
// ===============================================================================================================

Estado processaEstado(Estado st){
    switch(st){

        case ST_TELA_PRINCIPAL:
            return telaPrincipal();

        case ST_TELA_PRINCIPAL_PEDIDO:
            return telaPrincipalPedido();

        case ST_TELA_LISTA_PEDIDO:
            return telaListaPedido();

        default:
            return ST_SAINDO;
    }
}

void executaEstado(){
    while(estado_atual != ST_SAINDO){
        estado_atual = processaEstado(estado_atual);
    }
}
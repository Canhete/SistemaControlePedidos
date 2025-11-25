#ifndef ESTADOS_H
#define ESTADOS_H

/* Bibliotecas */

#include <stdio.h>
#include <stdlib.h>

#include "interface.h"

/* Todos States do programa, cada estado indica um menu diferente, trabalhar com states é bem superior do que ter que aninhar vários if elses e switches, o que é muito cringe */

/* Estrutura dos States */
typedef enum {
    ST_ABRINDO,
    ST_MENU_PRINCIPAL,

    ST_CLIENTE_PRINCIPAL,
    ST_CLIENTE_CADASTRO,
    ST_CLIENTE_LISTA,
    ST_CLIENTE_DETALHES,
    ST_CLIENTE_APAGAR,

    ST_PRODUTO_PRINCIPAL,
    ST_PRODUTO_CADASTRO,
    ST_PRODUTO_LISTA,
    ST_PRODUTO_DETALHES,
    ST_PRODUTO_APAGAR,

    ST_PEDIDO_PRINCIPAL,
    ST_PEDIDO_CADASTRO,
    ST_PEDIDO_LISTA,
    ST_PEDIDO_CONSULTA,
    ST_PEDIDO_APAGAR,

    ST_ITEM_PEDIDO_CADASTRO,
    ST_ITEM_PEDIDO_LISTA,
    ST_ITEM_PEDIDO_CONSULTA,
    ST_ITEM_PEDIDO_APAGAR,

    ST_TELA_PEQUENA,
    ST_CONFIGURACAO,
    ST_ERRO,

    ST_SAINDO
} Estado;

extern Estado estado_atual;

// FUNÇÕES DE ESTADO

void processaEstado(Estado st);

void executaEstado();

#endif
#ifndef ESTADOS_H
#define ESTADOS_H

/* Bibliotecas */

#include <stdio.h>
#include "cliente.h"
#include "produto.h"
#include "pedido.h"
#include "interface.h"

/* Todos States do programa, cada estado indica um menu diferente, trabalhar com states é bem superior do que ter que aninhar vários if elses e switches, o que é muito cringe */

/* Estrutura dos States */
typedef enum {
    ST_ABRINDO,
    ST_TELA_PRINCIPAL,
    ST_TELA_PRINCIPAL_PEDIDO,
    ST_TELA_CADASTRO_PEDIDO,
    ST_TELA_LISTA_PEDIDO,
    ST_TELA_DETALHA_PEDIDO,
    ST_TELA_APAGA_PEDIDO,
    ST_TELA_CADASTRO_ITEM_PEDIDO,
    ST_TELA_LISTA_ITEM_PEDIDO,
    ST_TELA_DETALHA_ITEM_PEDIDO,
    ST_TELA_APAGA_ITEM_PEDIDO,
    ST_ERRO,
    ST_SAINDO
} Estado;

/* Cria um callback para cada Estado, como não está definido, todos começam com void */
typedef Estado (*MenuCallback)(void);

/* Struct de Item de Menu, cada item do menu, ou seja, cada texto irá realizar uma ação, esse struct faz essa associação */
typedef struct {
    const char* texto;
    MenuCallback acao;
} ItemMenu;

// FUNÇÕES DE ESTADO

Estado processaEstado(Estado st);
void executaEstado();

#endif
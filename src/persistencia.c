/* BIBLIOTECAS */

#include <stdio.h>
#include <stdlib.h>
#include <cliente.h>
#include <interface.h>
#include <pedido.h>
#include <persistencia.h>
#include <produto.h>

/* DEFINIÇÕES & CONSTANTES */
#define NOME_ARQUIVO_PEDIDO "pedido.csv"


void guardarPedido(FILE *F, struct ItemPedido *P){
    *F = fopen(NOME_ARQUIVO_PEDIDO, "r+");

    fscanf()
}

void adicionarPedido(FILE *F, struct ItemPedido *P){

}

void apagarPedido(FILE *F, struct ItemPedido *P){

}

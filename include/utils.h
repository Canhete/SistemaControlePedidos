/* Biblioteca de utilitários */
/* Aqui deve estar as funções que conferem qualidade, isso é, requisitos que não são funcionais, mas que são de
utilidade para o programa */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Pedido;
struct ItemPedido;

/* DEFINIÇÕES E CONSTANTES */



/* FUNÇÕES */

const char* obterDataAtual();

int validarIdPedidoObtido(int id, char *mensagem);

int validarIdItemPedidoObtido(int id, char *mensagem);

int validarIdClienteObtido(int id, char *mensagem);

int validarData(char *data, char *mensagem);

int validarSubtotalObtido(double subtotal, char *mensagem);

int validarTotalObtido(double total, char *mensagem);

int validarQuantidadeObtida(int quantidade, char *mensagem);

#endif
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

/* PEDIDO */
// Como funciona? Você fornece o parâmetro do campo e cada função verifica se existe no banco de dados, se não existe, retorna um número diferente de 1, com uma mensagem de erro.

int validarQuantidade(int quantidade, char *mensagem);

int validarIdPedido(int id, char *mensagem);

int validarIdCliente(int codigoVerificado, char *mensagem);

int validarRapidamenteIdProduto(int id, char *mensagem);

int validarData(const char *dataVerificada, char *mensagem);

int validarTotal(double total, char *mensagem);

int validarIdItemPedido(int id, char *mensagem);

int validarSubTotal(double total, char *mensagem);

int validarQuantidade(int quantidade, char *mensagem);

void formatarDataPadrao(int dia, int mes, int ano, char *out);

#endif
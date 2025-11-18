/* Biblioteca de utilitários */
/* Aqui deve estar as funções que conferem qualidade, isso é, requisitos que não são funcionais, mas que são de
utilidade para o programa */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "persistencia.h"
#include "cliente.h"
#include "produto.h"
#include "pedido.h"

/* DEFINIÇÕES E CONSTANTES */



/* FUNÇÕES */

const char* obterDataAtual();

int validarIdPedido(int id);

int validarIdCliente(int id);

int validarData(char data);

#endif
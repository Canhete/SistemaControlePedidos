/* Biblioteca de utilitários */
/* Aqui deve estar as funções que conferem qualidade, isso é, requisitos que não são funcionais, mas que são de
utilidade para o programa */

#ifndef UTILS_H
#define UTILS_H

/* DEFINIÇÕES E CONSTANTES */



/* FUNÇÕES */

const char* obterDataAtual();

int validarIdPedido(int id);

int validarIdCliente(int id);

int validarData(char data);

#endif
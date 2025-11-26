#ifndef CLIENTE_H
#define CLIENTE_H

/* Bibliotecas */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

/* Estrutura de dados do Cliente */

#define CPF_LENGTH 11
#define CNPJ_LENGTH 14
#define MAX_CLIENTES 100



struct Cliente {
     int codigo;
     char tipo;
     char nome[100];
     char documento[15];
     char telefone[20];
     char endereco[150];
};

// Struct cliente global, para usar em todas funções - Raphael
extern struct Cliente clientes[MAX_CLIENTES];
extern int qtd_clientes;

/* FUNÇÕES DO CLIENTE */

int validarCPF(char cpf[]);

// Validação aceita de CNPJ......
int validarCNPJ(char cnpj[]);

void cadastrarCliente(struct Cliente clientes[], int *qtd);

void listarClientes(struct Cliente clientes[], int qtd);

void menuClientes();

#endif
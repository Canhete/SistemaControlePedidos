#ifndef CLIENTE_H
#define CLIENTE_H

/* Bibliotecas */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Estrutura de dados do Cliente */

struct Cliente {
    int codigo;
    char tipo; 
    char nome[100];
    char documento[15]; 
};

/* FUNÇÕES DO CLIENTE */

int validarCPF(char cpf[]);

// Validação aceita de CNPJ......
int validarCNPJ(char cnpj[]);

void cadastrarCliente(struct Cliente clientes[], int *qtd);

void listarClientes(struct Cliente clientes[], int qtd);

void menuClientes();

#endif
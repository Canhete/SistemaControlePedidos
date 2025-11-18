/* Biblioteca dos arquivos */

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"    // Importa struct de cliente
#include "produto.h"    // Importa struct de produto
#include "pedido.h"     // Importa o struct pedido

/* DEFINIÇÕES & CONSTANTES */

#define NOME_ARQUIVO_CLIENTE "cliente.csv"          // Nome padrão do arquivo de clientes
#define NOME_ARQUIVO_PRODUTO "produto.csv"          // Nome padrão do arquivo de produtos
#define NOME_ARQUIVO_PEDIDO "pedido.csv"            // Nome padrão do arquivo de pedidos
#define NOME_ARQUIVO_ITEM_PEDIDO "item_pedido.csv"  // Nome padrão do arquivo de itens de pedidos
#define NOME_ARQUIVO_TEMP "temp.csv"                // Nome padrão do arquivo temporário (usado para copiar informações de um arquivo para o outro e apagar um dado específico)
#define BUFFER_ARQUIVO_LINHA 512                    // Tamanho máximo de caracteres de uma linha, usado para ler linhas

/* FUNÇÕES */

void guardarPedido(struct Pedido *P);

void guardarItemPedido(struct ItemPedido *IP);

void apagarPedido(int idParaRemover);

void apagarItemPedido(int idParaRemover);

#endif
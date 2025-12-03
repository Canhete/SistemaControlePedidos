/* Biblioteca dos arquivos */

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <stdio.h>
#include <stdlib.h>

#include "cliente.h"    // Importa struct de cliente
#include "produto.h"    // Importa struct de produto

struct Pedido;         // Forward declaration da struct Pedido
struct ItemPedido;     // Forward declaration da struct ItemPedido

/* DEFINIÇÕES, CONSTANTES & DIRETÓRIOS */

#define DIRETORIO_ARQUIVO_CLIENTE "data/cliente.csv"            // Diretório padrão do arquivo de clientes
#define DIRETORIO_ARQUIVO_PRODUTO "data/produto.csv"            // Diretório padrão do arquivo de produtos
#define DIRETORIO_ARQUIVO_PEDIDO "data/pedido.csv"              // Diretório padrão do arquivo de pedidos
#define DIRETORIO_ARQUIVO_ITEM_PEDIDO "data/item_pedido.csv"    // Diretório padrão do arquivo de itens de pedidos
#define DIRETORIO_ARQUIVO_TEMP "data/temp.csv"                  // Diretório padrão do arquivo temporário (usado para copiar informações de um arquivo para o outro e apagar um dado específico)
#define BUFFER_ARQUIVO_LINHA 512                                // Tamanho máximo de caracteres de uma linha, usado para ler linhas

/* FUNÇÕES PEDIDO */

int criarArquivoPedido(char *mensagem);

int guardarPedido(struct Pedido *P, char *mensagem);

int analisarPedido(int idDoPedido, char *mensagem);

int apagarPedido(int idParaRemover, char *mensagem);

int apagarTodosPedidosDoCliente(int idDoCliente, char *mensagem);

int apagarTodosPedidosExistentes(char *mensagem);

int atualizarPedido(struct Pedido *P, char *mensagem);

/* FUNÇÕES ITEM DE PEDIDO */

int criarArquivoItemPedido(char *mensagem);

int guardarItemPedido(struct ItemPedido *IP, char *mensagem);

int apagarItemPedido(int idParaRemover, char *mensagem);

int apagarTodosItensDoPedido(int idDoPedido, char *mensagem);

int apagarTodosItensPedidoExistentes(char *mensagem);

#endif
/* Biblioteca dos arquivos */

#ifndef persistencia
#define persistencia

/* DEFINIÇÕES & CONSTANTES */

#define NOME_ARQUIVO_CLIENTE "cliente.csv"
#define NOME_ARQUIVO_PRODUTO "produto.csv"
#define NOME_ARQUIVO_PEDIDO "pedido.csv"
#define NOME_ARQUIVO_TEMP "temp.csv"
#define TAMANHO_BUFFER_LINHA 216    // Tamanho máximo de caracteres de uma linha, usado para ler linhas

/* FUNÇÕES */

void guardarPedido(struct Pedido *P);

void apagarPedido(int idParaRemover);

#endif
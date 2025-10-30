/* Biblioteca dos arquivos */

#ifndef persistencia
#define persistencia

void guardarPedido(FILE *F, struct ItemPedido *P);

void adicionarPedido(FILE *F, struct ItemPedido *P);

void apagarPedido(FILE *F, struct ItemPedido *P);

#endif
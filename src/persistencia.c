/* BIBLIOTECAS */

#include <stdio.h>
#include <stdlib.h>
#include <cliente.h>
#include <produto.h>
#include <pedido.h>
#include <persistencia.h>

/*==================================== MÓDULO PEDIDO ==============================================================*/
/* PEDIDO */

void guardarPedido(struct Pedido *P){
    FILE *arq = fopen(NOME_ARQUIVO_PEDIDO, "a+");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return;
    }

    fprintf(arq, "%d,%d,%s,%lf\n", P->id, P->clienteId, P->data, P->total);

    fclose(arq);
}

// Como a algoritmo que escreve informações no arquivo é sequencial, deve-se copiar toda informação do arquivo, sobreescrever com essa mesma informação, ignorando no processo o arquivo que não deseja ser gravado, no caso arquivo a ser apagado. No final faz uma cópia do original para o temporário, retirando da cópia o que se deseja remover, apaga-se o arquivo original e renomeia o temporário como original. O resultado é uma cópia identica com a ilusão de que uma informação foi removida.

void apagarPedido(int idParaRemover){
    FILE *arq = fopen(NOME_ARQUIVO_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return;
    }

    FILE *arqtemp = fopen(NOME_ARQUIVO_TEMP, "w");
    if(!arq){
        perror("ERRO AO CRIAR ARQUIVO TEMPORÁRIO!");
        fclose(arq);
        return;
    }

    char linha[TAMANHO_BUFFER_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        int id;
        sscanf(linha, "%d,", &id);
        if(id != idParaRemover){
            fputs(linha, arqtemp);
        }
    }

    fclose(arq);
    fclose(arqtemp);

    remove(arq);
    rename(NOME_ARQUIVO_TEMP, arq);
}

/* ITEM DE PEDIDO */

void guardarItemPedido(struct ItemPedido *IP){
    FILE *arq = fopen(NOME_ARQUIVO_ITEM_PEDIDO, "a");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return;
    }

    fprintf(arq, "%d,%d,%d,%lf\n", IP->pedidoId, IP->produtoId, IP->quantidade, IP->subtotal);

    fclose(arq);
}

void apagarItemPedido(int idParaRemover){
    FILE *arq = fopen(NOME_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return;
    }

    FILE *arqtemp = fopen(NOME_ARQUIVO_TEMP, "w");
    if(!arq){
        perror("ERRO AO CRIAR ARQUIVO TEMPORÁRIO!");
        fclose(arq);
        return;
    }

    char linha[TAMANHO_BUFFER_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        int id;
        sscanf(linha, "%d,", &id);
        if(id != idParaRemover){
            fputs(linha, arqtemp);
        }
    }

    fclose(arq);
    fclose(arqtemp);

    remove(arq);
    rename(NOME_ARQUIVO_TEMP, arq);
}
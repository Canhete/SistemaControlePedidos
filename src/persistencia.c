/* BIBLIOTECAS */

#include "../include/persistencia.h"
#include "../include/pedido.h"
#include "../include/utils.h"

// =====================================================================
//                      FUNÇÕES PEDIDO
// ====================================================================

int guardarPedido(struct Pedido *P, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "a+");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    fprintf(arq, "%d,%d,%s,%lf\n", P->id, P->clienteId, P->data, P->total);

    fclose(arq);
    return 1;
}

// Como a algoritmo que escreve informações no arquivo é sequencial, deve-se copiar toda informação do arquivo, sobreescrever com essa mesma informação, ignorando no processo o arquivo que não deseja ser gravado, no caso arquivo a ser apagado. No final faz uma cópia do original para o temporário, retirando da cópia o que se deseja remover, apaga-se o arquivo original e renomeia o temporário como original. O resultado é uma cópia identica com a ilusão de que uma informação foi removida.

int apagarPedido(int idParaRemover, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    FILE *arqtemp = fopen(DIRETORIO_ARQUIVO_TEMP, "w");
    if(!arq){
        mensagem = "Erro ao criar arquivo temporário!";
        fclose(arq);
        return -2;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        int id;
        sscanf(linha, "%d,", &id);
        if(id != idParaRemover) fputs(linha, arqtemp);
    }

    fclose(arq);
    fclose(arqtemp);

    // Remove o arquivo antigo de cliente e renomeia o temporário para o nome do arquivo de cliente
    remove(DIRETORIO_ARQUIVO_PEDIDO);
    rename(DIRETORIO_ARQUIVO_PEDIDO, DIRETORIO_ARQUIVO_PEDIDO);
    return 1;
}

int apagarTodosPedidosDoCliente(int idDoCliente, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    FILE *arqtemp = fopen(DIRETORIO_ARQUIVO_TEMP, "w");
    if(!arq){
        mensagem = "Erro ao criar arquivo temporário!";
        fclose(arq);
        return -2;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        int clienteId;
        sscanf(linha, "%*d,%d,", &clienteId);
        if(clienteId != idDoCliente) fputs(linha, arqtemp);
    }

    fclose(arq);
    fclose(arqtemp);

    remove(DIRETORIO_ARQUIVO_PEDIDO);
    rename(DIRETORIO_ARQUIVO_TEMP, DIRETORIO_ARQUIVO_PEDIDO);

    return 1;
}

// =====================================================================
//                  FUNÇÕES ITEM DE PEDIDO
// ====================================================================

int guardarItemPedido(struct ItemPedido *IP, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "a");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    fprintf(arq, "%d,%d,%d,%lf\n", IP->pedidoId, IP->produtoId, IP->quantidade, IP->subtotal);

    fclose(arq);
    return 1;
}

int apagarItemPedido(int idParaRemover, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    FILE *arqtemp = fopen(DIRETORIO_ARQUIVO_TEMP, "w");
    if(!arq){
        mensagem = "Erro ao criar arquivo temporário!";
        fclose(arq);
        return -2;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        int id;
        sscanf(linha, "%d,", &id);
        if(id != idParaRemover) fputs(linha, arqtemp);
    }

    fclose(arq);
    fclose(arqtemp);

    remove(DIRETORIO_ARQUIVO_ITEM_PEDIDO);
    rename(DIRETORIO_ARQUIVO_TEMP, DIRETORIO_ARQUIVO_ITEM_PEDIDO);

    return 1;
}

int apagarTodosItensDoPedido(int idDoPedido, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    FILE *arqtemp = fopen(DIRETORIO_ARQUIVO_TEMP, "w");
    if(!arq){
        mensagem = "Erro ao criar arquivo temporário!";
        fclose(arq);
        return -2;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        int pedidoId;
        sscanf(linha, "%d,", &pedidoId);
        if(pedidoId != idDoPedido) fputs(linha, arqtemp);
    }

    fclose(arq);
    fclose(arqtemp);

    remove(DIRETORIO_ARQUIVO_ITEM_PEDIDO);
    rename(DIRETORIO_ARQUIVO_TEMP, DIRETORIO_ARQUIVO_ITEM_PEDIDO);

    return 1;
}
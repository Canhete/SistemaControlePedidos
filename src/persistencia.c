/* BIBLIOTECAS */

#include "../include/persistencia.h"
#include "../include/pedido.h"
#include "../include/utils.h"

// =====================================================================
//                      FUNÇÕES PEDIDO
// ====================================================================

int criarArquivoPedido(char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r+");
    if(!arq){ // Se não existe, cria
        arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "w");
        if(!arq){
            sprintf(mensagem, "Erro ao criar o arquivo de pedidos!");
            return -1;
        }

        fprintf(arq, "pedido_id;cliente_id;data;total\n");
        fclose(arq);
        return 1;
    }
    rewind(arq);

    char linha[BUFFER_ARQUIVO_LINHA];

    // Primeira linha cabeçalho
    if(fgets(linha, sizeof(linha), arq) == NULL){
        rewind(arq);
        fprintf(arq, "pedido_id;cliente_id;data;total\n");
        fclose(arq);
        return 1;
    }

    // Caso o cabeçalho esta escrito de maneira incorreta, sobreescreve
    if(strcmp(linha, "pedido_id;cliente_id;data;total\n") != 0){
        rewind(arq);
        fprintf(arq, "pedido_id;cliente_id;data;total\n");
    }

    fclose(arq);
    return 1; // -> sucesso
}

int guardarPedido(struct Pedido *P, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "a");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo de pedidos!");
        return -1;
    }

    fprintf(arq, "%d;%d;%10s;%.2lf\n", P->id, P->clienteId, P->data, P->total);

    fclose(arq);
    return 1; // -> sucesso
}

int atualizarPedido(struct Pedido *P, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r+");
    if (!arq) {
        sprintf(mensagem, "Erro ao abrir arquivo de pedidos!");
        return 0;
    }

    FILE *tmp = fopen(DIRETORIO_ARQUIVO_TEMP, "w");
    if (!tmp) {
        sprintf(mensagem, "Erro ao criar arquivo temporário!");
        return 0;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    fprintf(tmp, "id;clienteId;data;total\n");

    while (fgets(linha, sizeof(linha), arq)) {
        struct Pedido x;

        if (sscanf(linha, "%d;%d;%10[^;];%lf", &x.id, &x.clienteId, x.data, &x.total) == 4) {
            if (x.id == P->id) { // Se for o mesmo pedido, atualiza
                fprintf(tmp, "%d;%d;%s;%.2lf\n", P->id, P->clienteId, P->data, P->total);
            } else { // Se não copia
                fputs(linha, tmp);
            }
        }
    }

    fclose(arq);
    fclose(tmp);

    remove(DIRETORIO_ARQUIVO_PEDIDO);
    rename(DIRETORIO_ARQUIVO_TEMP, DIRETORIO_ARQUIVO_PEDIDO);

    return 1;
}

int analisarPedido(int idDoPedido, char *mensagem) {
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arq){
        sprintf(mensagem, "Arquivo de pedidos inexistente ou não pode ser aberto.");
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)) {
        struct Pedido P;

        if(sscanf(linha, "%d;%d;%10[^;];%lf", &P.id, &P.clienteId, P.data, &P.total) == 4){
            if(idDoPedido == P.id){
                fclose(arq);
                return 1; // Pedido existe
            }
        }
    }
    fclose(arq);
    return 0; // Pedido não existe
}

// Como a algoritmo que escreve informações no arquivo é sequencial, deve-se copiar toda informação do arquivo, sobreescrever com essa mesma informação, ignorando no processo o arquivo que não deseja ser gravado, no caso arquivo a ser apagado. No final faz uma cópia do original para o temporário, retirando da cópia o que se deseja remover, apaga-se o arquivo original e renomeia o temporário como original. O resultado é uma cópia identica com a ilusão de que uma informação foi removida.

int apagarPedido(int idParaRemover, char *mensagem){
    if(!apagarItemPedido(idParaRemover, mensagem)) return -3; // Apaga primeiro os itens de pedidos

    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo de pedidos!");
        return -1;
    }

    FILE *arqtemp = fopen(DIRETORIO_ARQUIVO_TEMP, "w");
    if(!arqtemp){
        sprintf(mensagem, "Erro ao criar arquivo temporário!");
        fclose(arq);
        return -2;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct Pedido P;
        
        if(sscanf(linha, "%d;%d;%10[^;];%lf", &P.id, &P.clienteId, P.data, &P.total) == 4){
            if(idParaRemover != P.id){
                fputs(linha, arqtemp);
            }
        }
    }

    fclose(arq);
    fclose(arqtemp);

    // Remove o arquivo antigo de cliente e renomeia o temporário para o nome do arquivo de cliente
    remove(DIRETORIO_ARQUIVO_PEDIDO);
    rename(DIRETORIO_ARQUIVO_TEMP, DIRETORIO_ARQUIVO_PEDIDO);
    return 1;
}

int apagarTodosPedidosDoCliente(int idDoCliente, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r+");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo!");
        return -1;
    }

    FILE *arqtemp = fopen(DIRETORIO_ARQUIVO_TEMP, "w+");
    if(!arq){
        sprintf(mensagem, "Erro ao criar arquivo temporário!");
        fclose(arq);
        return -2;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        int clienteId;
        sscanf(linha, "%*d;%d;", &clienteId);
        if(clienteId != idDoCliente) fputs(linha, arqtemp);
    }

    fclose(arq);
    fclose(arqtemp);

    remove(DIRETORIO_ARQUIVO_PEDIDO);
    rename(DIRETORIO_ARQUIVO_TEMP, DIRETORIO_ARQUIVO_PEDIDO);

    return 1;
}

int apagarTodosPedidosExistentes(char *mensagem){
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
        // Não copia nada para o arquivo temporário, apagando assim todo conteúdo
    }

    fclose(arq);
    fclose(arqtemp);

    remove(DIRETORIO_ARQUIVO_PEDIDO);
    rename(DIRETORIO_ARQUIVO_TEMP, DIRETORIO_ARQUIVO_PEDIDO);

    if(apagarTodosItensPedidoExistentes(mensagem) < 0) return -3;

    return 1;
}

// =====================================================================
//                  FUNÇÕES ITEM DE PEDIDO
// ====================================================================

int criarArquivoItemPedido(char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r+");
    if(!arq){ // Se não existe, cria
        arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "w");
        if(!arq){
            sprintf(mensagem, "Erro ao criar o arquivo de item pedidos!");
            return -1;
        }

        fprintf(arq, "pedido_id;produto_id;quantidade;subtotal\n");
        fclose(arq);
        return 1;
    }
    rewind(arq);

    char linha[BUFFER_ARQUIVO_LINHA];

    // Primeira linha cabeçalho
    if(fgets(linha, sizeof(linha), arq) == NULL){
        rewind(arq);
        fprintf(arq, "pedido_id;produto_id;quantidade;subtotal\n");
        fclose(arq);
        return 1;
    }

    // Caso o cabeçalho esta escrito de maneira incorreta, sobreescreve
    if(strcmp(linha, "pedido_id;produto_id;quantidade;subtotal\n") != 0){
        rewind(arq);
        fprintf(arq, "pedido_id;produto_id;quantidade;subtotal\n");
    }

    fclose(arq);
    return 1; // -> sucesso
}

int guardarItemPedido(struct ItemPedido *IP, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "a");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo de itens de pedidos!");
        return -1;
    }

    fprintf(arq, "%d;%d;%d;%.2lf\n", IP->pedidoId, IP->produtoId, IP->quantidade, IP->subtotal);

    fclose(arq);
    return 1;
}

int analisarItemPedido(int idDoItemPedido, char *mensagem) {
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        sprintf(mensagem, "Arquivo de itens de pedidos inexistente ou não pode ser aberto.");
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)) {
        struct ItemPedido IP;

        if(sscanf(linha, "%d;%d;%d;%lf", &IP.pedidoId, &IP.produtoId, &IP.quantidade, &IP.subtotal) == 4 && IP.pedidoId == idDoItemPedido) {
            fclose(arq);
            return 1;
        }
    }
    fclose(arq);
    return 0; // Item de pedido não existe
}

int apagarItemPedido(int idParaRemover, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo de itens de pedidos!");
        return -1;
    }

    FILE *arqtemp = fopen(DIRETORIO_ARQUIVO_TEMP, "a");
    if(!arqtemp){
        sprintf(mensagem, "Erro ao criar arquivo temporário!");
        fclose(arq);
        return -2;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct ItemPedido IP;
        
        if(sscanf(linha, "%d;%d;%d;%lf", &IP.pedidoId, &IP.produtoId, &IP.quantidade, &IP.subtotal) == 4){
            if(idParaRemover != IP.pedidoId){
                fputs(linha, arqtemp);
            }
        }
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
        sprintf(mensagem, "Erro ao abrir arquivo!");
        return -1;
    }

    FILE *arqtemp = fopen(DIRETORIO_ARQUIVO_TEMP, "w");
    if(!arq){
        sprintf(mensagem, "Erro ao criar arquivo temporário!");
        fclose(arq);
        return -2;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        int pedidoId;
        sscanf(linha, "%d;", &pedidoId);
        if(pedidoId != idDoPedido) fputs(linha, arqtemp);
    }

    fclose(arq);
    fclose(arqtemp);

    remove(DIRETORIO_ARQUIVO_ITEM_PEDIDO);
    rename(DIRETORIO_ARQUIVO_TEMP, DIRETORIO_ARQUIVO_ITEM_PEDIDO);

    return 1;
}

int apagarTodosItensPedidoExistentes(char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo!");
        return -1;
    }

    FILE *arqtemp = fopen(DIRETORIO_ARQUIVO_TEMP, "w");
    if(!arq){
        sprintf(mensagem, "Erro ao criar arquivo temporário!");
        fclose(arq);
        return -2;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        // Não copia nada para o arquivo temporário, apagando assim todo conteúdo
    }

    fclose(arq);
    fclose(arqtemp);

    remove(DIRETORIO_ARQUIVO_ITEM_PEDIDO);
    rename(DIRETORIO_ARQUIVO_TEMP, DIRETORIO_ARQUIVO_ITEM_PEDIDO);

    return 1;
}
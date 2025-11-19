#include "../include/utils.h"
#include "../include/cliente.h"
#include "../include/pedido.h"
#include "../include/produto.h"
#include "../include/cliente.h"
#include "../include/persistencia.h"
#include <string.h>
#include <ctype.h>

const char* obterDataAtual(){
    static char dataAtual[11];
    time_t agora;
    struct tm *infoTempo;

    time(&agora);
    infoTempo = localtime(&agora);
    strftime(dataAtual, sizeof(dataAtual), "%d/%m/%Y", infoTempo);

    return dataAtual;
}

int validarIdPedidoObtido(int id, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq) != NULL){
        struct Pedido P;
        fscanf(arq, "%d", &P.id);
        if(P.id == id) return 1;
    }

    fclose(arq);
    mensagem = "ID do pedido não encontrado, digite um ID válido!";
    return 0;
}

int validarIdItemPedidoObtido(int id, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq) != NULL){
        struct ItemPedido IP;
        fscanf(arq, "%d", &IP.pedidoId);
        if(IP.pedidoId == id) return 1;
    }

    fclose(arq);
    mensagem = "ID do pedido não encontrado, digite um ID válido!";
    return 0;
}

int validarIdClienteObtido(int codigoVerificado, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];
    
    while(fgets(linha, sizeof(linha), arq)){
        struct Cliente C;
        fscanf(arq, "%d", &C.codigo);
        if(C.codigo == codigoVerificado) return 1;
    }

    fclose(arq);
    mensagem = "ID do cliente não encontrado, digite um ID válido!";
    return 0;
}

int validarTotalObtido(double total, char *mensagem){
    if(total < 0){
        mensagem = "Valor de total menor que zero!";
        return -2;
    }

    if(total == 0){
        mensagem = "Valor de total não pode ser zero!";
        return -3;
    }

    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];
    
    while(fgets(linha, sizeof(linha), arq)){
        struct Pedido P;
        fscanf(arq, "%lf", &P.total);
        if(P.total == total) return 1;
    }

    fclose(arq);
    mensagem = "Nenhum pedido com esse total encontrado!";
    return 0;
}

int validarSubtotalObtido(double subtotal, char *mensagem){
    if(subtotal < 0){
        mensagem = "Valor de subtotal menor que zero!";
        return -2;
    }

    if(subtotal == 0){
        mensagem = "Valor de subtotal não pode ser zero!";
        return -3;
    }

    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];
    
    while(fgets(linha, sizeof(linha), arq)){
        struct ItemPedido IP;
        fscanf(arq, "%lf", &IP.subtotal);
        if(IP.subtotal == subtotal) return 1;
    }

    fclose(arq);
    mensagem = "Nenhum item de pedido com esse subtotal encontrado!";
    return 0;
}

int validarData(char *dataVerificada, char *mensagem){
    int data = atoi(dataVerificada + 6) * 10000 + atoi(dataVerificada + 3) * 100 + atoi(dataVerificada);

    int dia = data / 1000000;
    int mes = (data / 10000) % 100;
    int ano = data % 10000;

    if ((dia >= 1 && dia <= 31) && (mes >= 1 && mes <= 12) && (ano > 0 && ano <= 9999)){ // Verifica se os numeros sao validos
        if ((dia == 29 && mes == 2) && ((ano % 4) == 0)) return 1;  // Ano bissexto
        else if (dia <= 28 && mes == 2) return 1;    // Fevereiro
        else if ((dia <= 30) && (mes == 4 || mes == 6 || mes == 9 || mes == 11)) return 1; // Mês de 30 dias
        else if ((dia <=31) && (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes ==8 || mes == 10 || mes == 12)) return 1; // Mês de 31 dias
        else return 0;
    } else {
        mensagem = "Número de dia/mês/ano inválido! Use a forma (ddmmaaaa).";
        return 0;
    }
}

int validarQuantidadeObtida(int quantidade, char *mensagem){
    if(quantidade < 0){
        mensagem = "Valor de quantidade menor que zero!";
        return -2;
    }

    if(quantidade == 0){
        mensagem = "Valor de quantidade não pode ser zero!";
        return -3;
    }

    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        mensagem = "Erro ao abrir arquivo!";
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];
    
    while(fgets(linha, sizeof(linha), arq)){
        struct ItemPedido IP;
        fscanf(arq, "%d", &IP.quantidade);
        if(IP.quantidade == quantidade) return 1;
    }

    fclose(arq);
    mensagem = "Nenhum item de pedido com essa quantidade encontrado!";
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pedido.h>
#include <cliente.h>
#include <persistencia.h>

const char* obterDataAtual(){
    static char dataAtual[11];
    time_t agora;
    struct tm *infoTempo;

    time(&agora);
    infoTempo = localtime(&agora);
    strftime(dataAtual, sizeof(dataAtual), "%d/%m/%Y", infoTempo);

    return dataAtual;
}

int validarIdPedido(int id){
    FILE *arq = fopen(NOME_ARQUIVO_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return 1;
    }

    char linha[TAMANHO_BUFFER_LINHA];

    while(fgets(linha, sizeof(linha), arq) != NULL){
        struct Pedido P;
        fscanf(arq, "%d", &P.id);
        if(P.id == id) return 1;
    }

    fclose(arq);
    return 0;
}

int validarIdCliente(int id){ // FAZER ISSO AQUI QUANDO TIVER O STRUCT DE CLIENTE
    FILE *arq = fopen(NOME_ARQUIVO_CLIENTE, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return 1;
    }

    char linha[TAMANHO_BUFFER_LINHA];
    
    /*
    while(fgets(linha, sizeof(linha), arq)){
        struct Cliente C;
        fscanf(arq, "%d", &C.id);
        if(C.id == id) return 1;
    }
    */

    fclose(arq);
    return 0;
}

int validarData(char data){
    int data = atoi(data);

    int dia = data/1000000;
    int mes = (data/10000) % 100;
    int ano = data % 10000;

    if ((dia >= 1 && dia <= 31) && (mes >= 1 && mes <= 12) && (ano > 0 && ano <= 9999)){ // Verifica se os numeros sao validos
        if ((dia == 29 && mes == 2) && ((ano % 4) == 0)) return 1;  // Ano bissexto
        if (dia <= 28 && mes == 2) return 1;    // Fevereiro
        if ((dia <= 30) && (mes == 4 || mes == 6 || mes == 9 || mes == 11)) return 1; // Mês de 30 dias
        if ((dia <=31) && (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes ==8 || mes == 10 || mes == 12)) return 1; // Mês de 31 dias
        else return 0;
    } else return 0;
}


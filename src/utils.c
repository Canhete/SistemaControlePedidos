#include "../include/utils.h"

const char* obterDataAtual(){
    static char dataAtual[11];
    time_t agora;
    struct tm *infoTempo;

    time(&agora);
    infoTempo = localtime(&agora);
    strftime(dataAtual, sizeof(dataAtual), "%d/%m/%Y", infoTempo);

    return dataAtual;
}

int validarIdPedidoObtido(int id){
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

int validarIdClienteObtido(int codigoVerificado){
    FILE *arq = fopen(NOME_ARQUIVO_CLIENTE, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return 1;
    }

    char linha[TAMANHO_BUFFER_LINHA];
    
    while(fgets(linha, sizeof(linha), arq)){
        struct Cliente C;
        fscanf(arq, "%d", &C.codigo);
        if(C.codigo == codigoVerificado) return 1;
    }

    fclose(arq);
    return 0;
}

int validarData(char dataVerificada){
    int data = atoi(&dataVerificada);

    int dia = data/1000000;
    int mes = (data/10000) % 100;
    int ano = data % 10000;

    if ((dia >= 1 && dia <= 31) && (mes >= 1 && mes <= 12) && (ano > 0 && ano <= 9999)){ // Verifica se os numeros sao validos
        if ((dia == 29 && mes == 2) && ((ano % 4) == 0)) return 1;  // Ano bissexto
        else if (dia <= 28 && mes == 2) return 1;    // Fevereiro
        else if ((dia <= 30) && (mes == 4 || mes == 6 || mes == 9 || mes == 11)) return 1; // Mês de 30 dias
        else if ((dia <=31) && (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes ==8 || mes == 10 || mes == 12)) return 1; // Mês de 31 dias
        else return 0;
    } else return 0;
}


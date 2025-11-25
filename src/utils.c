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

// ===========================================================
//              Funções de validação Pedido
// ===========================================================

int validarIdPedido(int id, char *mensagem){
    int index = analisarPedido(id, mensagem);

    if(index == -1) return 0; // Erro ao abrir arquivo

    if(index == 1){
        sprintf(mensagem, "ID do pedido já existe! Digite um ID válido!");
    }

    return 1; // Se passar o teste, tá livre para cadastro
}

int validarIdCliente(int codigoVerificado, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_CLIENTE, "r");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo de clientes!");
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];
    
    while(fgets(linha, sizeof(linha), arq)){
        struct Cliente C;

        if(sscanf(linha, "%d,%c,%100[^,],%15[^\n]", &C.codigo, &C.tipo, C.nome, C.documento) == 4){
            if(codigoVerificado == C.codigo){
                fclose(arq);
                return 1;
            }
        } // Sucesso, encontrou cliente com ID!
    }

    fclose(arq);
    mensagem = "Código de cliente não existe! Digite um código válido!";
    return 0; // Verificou toda lista de cliente, cliente não encontrado.
}

int validarIdProduto(int id, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PRODUTO, "r");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo de produtos!");
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];
    
    while(fgets(linha, sizeof(linha), arq)){
        Produto P;

        if(sscanf(linha, "%d,%100[^,],%lf,%d", &P.id, P.descricao, &P.preco, &P.estoque) == 4){
            if(id == P.id){
                fclose(arq);
                return 1;
            }
        }
    }

    fclose(arq);
    sprintf(mensagem, "Produto com id não encontrado! Digite um id válido!");
    return 0;
}

int validarData(const char *dataVerificada, char *mensagem){
    if(dataVerificada == NULL){
        sprintf(mensagem, "Data nula! Use o formato (dd/mm/aaaa).");
        return 0;
    }
    
    if((int) strlen(dataVerificada) != 10){
        sprintf(mensagem, "Data inválida! Use o formato (dd/mm/aaaa).");
        return 0;
    }

    int dia = 0;
    int mes = 0;
    int ano = 0;

    if(sscanf(dataVerificada, "%2d/%2d/%4d", &dia, &mes, &ano) != 3){
        sprintf(mensagem, "Formato inválido! Use o formato (dd/mm/aaaa).");
        return 0;
    }

    // Verifica se ano e meses estão no limite aceitável
    if(ano < 1 || ano > 9999){
        sprintf(mensagem, "Ano inválido! Insira um ano válido!");
        return 0;
    }

    if(mes < 1 || mes > 12){
        sprintf(mensagem, "Mês inválido! Insira um mês válido!");
        return 0;
    }

    int bissexto = ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0));

    int diasPorMes;
    switch(mes){
        case 1: 
        case 3: 
        case 5: 
        case 7: 
        case 8: 
        case 10: 
        case 12:
            diasPorMes = 31; // Janeiro, Março, Maio, Julho, Setembro, Novembro, Dezembro
            break;

        case 4: 
        case 6: 
        case 9: 
        case 11:
            diasPorMes = 30; // Abril, Junho, Setembro, Novembro
            break;

        case 2: // Fevereiro, se for bissexto assume 29, se não 28;
            diasPorMes = bissexto ? 29 : 28;
            break;

        default:
            diasPorMes = 31; // Apenas para o caso de der errado
    }

    if(dia < 1 || dia > diasPorMes){
        sprintf(mensagem, "Dia inválido para o mês obtido! Insira um dia válido!");
        return 0;
    }

    return 1; // -> SUCESSO PASSOU VALIDAÇÃO
}

int validarTotal(double total, char *mensagem){
    if(total < 0){
        sprintf(mensagem, "Valor de total menor que zero!");
        return -2;
    }

    if(total == 0){
        sprintf(mensagem, "Valor de total não pode ser zero!");
        return -3;
    }

    return 1;
}

// ===========================================================
//              Funções de validação Item Pedido
// ===========================================================

int validarIdItemPedido(int id, char *mensagem){
    int index = analisarItemPedido(id, mensagem);

    if(index == -1) return 0;

    if(index == 0){
        sprintf(mensagem, "Cliente com id não enontrado! Digite um ID válido!");
    }

    return 1;
}

int validarSubTotal(double total, char *mensagem){
    if(total < 0){
        sprintf(mensagem, "Valor de total menor que zero!");
        return -2;
    }

    if(total == 0){
        sprintf(mensagem, "Valor de total não pode ser zero!");
        return -3;
    }

    return 1;
}

int validarQuantidade(int quantidade, char *mensagem){
    if(quantidade < 0){
        sprintf(mensagem, "Valor de quantidade menor que zero!");
        return -2;
    }

    if(quantidade == 0){
        sprintf(mensagem, "Valor de quantidade não pode ser zero!");
        return -3;
    }

    // Verificar se a quantidade não extrapola o estoque
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PRODUTO, "r");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo de produtos!");
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];
    
    while(fgets(linha, sizeof(linha), arq)){
        Produto P;

        if(sscanf(linha, "%d,%100[^,]s,%lf,%d", &P.id, P.descricao, &P.preco, &P.estoque) == 4){
            if(quantidade > P.estoque){
                sprintf(mensagem, "Quantidade superior ao estoque disponível! Digite uma quantidade válida!");
                fclose(arq);
                return 0;
            }
        }
    }

    fclose(arq);
    return 1;
}

// Formata data antes de gravar no csv para garantir compatibilidade
void formatarDataPadrao(int dia, int mes, int ano, char *out){
    snprintf(out, 11, "%02d/%02d/%04d", dia, mes, ano);
}
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define CPF_LENGTH 11
#define CNPJ_LENGTH 14
#define MAX_CLIENTES 100

// Estrutura Cliente
struct Cliente {
    int codigo;
    char tipo; 
    char nome[100];
    char documento[15]; 
};

// --------------------> FUNÇÕES DE VALIDAÇÃO <-------------------- //

// Validação simples de CPF............
int validarCPF(char cpf[]) {
    int i, j, soma, resto, digito1, digito2;

    // Verificação de tamanho.........
    if (strlen(cpf) != CPF_LENGTH) return 0;

    // Verifica se os valores são númericos.....
    for (i = 0; i < CPF_LENGTH; i++) {
        if (!isdigit(cpf[i])) return 0;
    }

    // Verificação de valores iguais............
    for (i = 1; i < CPF_LENGTH; i++) {
        if (cpf[i] != cpf[0]) break;
        if (i == CPF_LENGTH - 1) return 0;
    }

    // Calculo do primeiro digito....
    soma = 0;
    for (i = 0, j = 10; i < 9; i++, j--) soma += (cpf[i] - '0') * j;
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;

    // Calculo do segundo digito..........
    soma = 0;
    for (i = 0, j = 11; i < 10; i++, j--) soma += (cpf[i] - '0') * j;
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;

    // Comparação dos dígitos informados pelo cliente.......
    return (digito1 == (cpf[9] - '0') && digito2 == (cpf[10] - '0'));
}

// Validação aceita de CNPJ......
int validarCNPJ(char cnpj[]) {
    int i, j, soma, resto, digito1, digito2;
    int pesos1[12] = {5,4,3,2,9,8,7,6,5,4,3,2};
    int pesos2[13] = {6,5,4,3,2,9,8,7,6,5,4,3,2};

    if (strlen(cnpj) != CNPJ_LENGTH) return 0;

    for (i = 0; i < CNPJ_LENGTH; i++) {
        if (!isdigit(cnpj[i])) return 0;
    }

    // Verificação de valores iguais............
    for (i = 1; i < CNPJ_LENGTH; i++) {
        if (cnpj[i] != cnpj[0]) break;
        if (i == CNPJ_LENGTH - 1) return 0;
    }

    // Calculo do primeiro digito....
    soma = 0;
    for (i = 0; i < 12; i++) soma += (cnpj[i] - '0') * pesos1[i];
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;

    // Calculo do segundo digito..........
    soma = 0;
    for (i = 0; i < 13; i++) soma += (cnpj[i] - '0') * pesos2[i];
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;

    return (digito1 == (cnpj[12] - '0') && digito2 == (cnpj[13] - '0'));
}

// --------------------> FUNÇÕES DE CADASTRO E LISTAGEM <-------------------- //

void cadastrarCliente(struct Cliente clientes[], int *qtd) {
    struct Cliente novo;

    printf("\n--- Cadastro de Cliente ---\n");
    novo.codigo = *qtd + 1;

    printf("Pessoa Física (F) ou Jurídica (J)? ");
    scanf(" %c", &novo.tipo);
    novo.tipo = toupper(novo.tipo);

    if (novo.tipo == 'F') {
        printf("Nome do cliente: ");
        scanf(" %[^\n]%*c", novo.nome);

        do {
            printf("CPF (somente números): ");
            scanf("%s", novo.documento);
            if (!validarCPF(novo.documento))
                printf("CPF inválido. Tente novamente.\n");
        } while (!validarCPF(novo.documento));

    } else if (novo.tipo == 'J') {
        printf("Razão Social: ");
        scanf(" %[^\n]%*c", novo.nome);

        do {
            printf("CNPJ (somente números): ");
            scanf("%s", novo.documento);
            if (!validarCNPJ(novo.documento))
                printf("CNPJ inválido. Tente novamente.\n");
        } while (!validarCNPJ(novo.documento));

    } else {
        printf("Tipo inválido! Use 'F' ou 'J'.\n");
        return;
    }

    clientes[*qtd] = novo;
    (*qtd)++;

    printf("Cliente cadastrado com sucesso!\n");
}

void listarClientes(struct Cliente clientes[], int qtd) {
    printf("\n--- Lista de Clientes ---\n");
    if (qtd == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    for (int i = 0; i < qtd; i++) {
        printf("\nCódigo: %d\n", clientes[i].codigo);
        printf("Tipo: %s\n", (clientes[i].tipo == 'F') ? "Pessoa Física" : "Pessoa Jurídica");
        printf("%s: %s\n", (clientes[i].tipo == 'F') ? "Nome" : "Razão Social", clientes[i].nome);
        printf("%s: %s\n", (clientes[i].tipo == 'F') ? "CPF" : "CNPJ", clientes[i].documento);
    }
}

// --------------------> FUNÇÃO PRINCIPAL <-------------------- //

void menuClientes() {
    struct Cliente clientes[MAX_CLIENTES];
    int qtd = 0;
    int opcao;

    do {
        printf("\n====> MENU CLIENTES <====\n");
        printf("1 - Cadastrar Cliente\n");
        printf("2 - Listar Clientes\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarCliente(clientes, &qtd);
                break;
            case 2:
                listarClientes(clientes, qtd);
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

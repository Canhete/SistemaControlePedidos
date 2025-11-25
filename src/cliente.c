#include "../include/cliente.h"
#include "../include/interface.h"
#include "../include/estados.h"

// Estrutura Cliente

struct Cliente clientes[MAX_CLIENTES];
int qtd_clientes = 0;

// ---------------- VALIDAÇÃO CPF ----------------
int validarCPF(char cpf[]) {
    int i, j, soma, resto, digito1, digito2;
    if (strlen(cpf) != CPF_LENGTH) return 0;
    for (i = 0; i < CPF_LENGTH; i++)
        if (!isdigit(cpf[i])) return 0;

    for (i = 1; i < CPF_LENGTH; i++)
        if (cpf[i] != cpf[0]) break;
        else if (i == CPF_LENGTH - 1) return 0;

    soma = 0;
    for (i = 0, j = 10; i < 9; i++, j--) soma += (cpf[i] - '0') * j;
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;

    soma = 0;
    for (i = 0, j = 11; i < 10; i++, j--) soma += (cpf[i] - '0') * j;
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;

    return (digito1 == (cpf[9] - '0') && digito2 == (cpf[10] - '0'));
}

// ---------------- VALIDAÇÃO CNPJ ----------------
int validarCNPJ(char cnpj[]) {
    int i, soma, resto, digito1, digito2;
    int pesos1[12] = {5,4,3,2,9,8,7,6,5,4,3,2};
    int pesos2[13] = {6,5,4,3,2,9,8,7,6,5,4,3,2};
    if (strlen(cnpj) != CNPJ_LENGTH) return 0;
    for (i = 0; i < CNPJ_LENGTH; i++)
        if (!isdigit(cnpj[i])) return 0;
    for (i = 1; i < CNPJ_LENGTH; i++)
        if (cnpj[i] != cnpj[0]) break;
        else if (i == CNPJ_LENGTH - 1) return 0;

    soma = 0;
    for (i = 0; i < 12; i++) soma += (cnpj[i] - '0') * pesos1[i];
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;

    soma = 0;
    for (i = 0; i < 13; i++) soma += (cnpj[i] - '0') * pesos2[i];
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;

    return (digito1 == (cnpj[12] - '0') && digito2 == (cnpj[13] - '0'));
}

// ---------------- CADASTRO ----------------
void cadastrarCliente(struct Cliente clientes[], int *qtd) {
    echo(); // habilita digitação visível...
    struct Cliente novo;
    novo.codigo = *qtd + 1;

    clear();
    mvprintw(2, 2, "--- Cadastro de Cliente ---");
    mvprintw(4, 2, "Pessoa Física (F) ou Jurídica (J): ");
    scanw(" %c", &novo.tipo);
    novo.tipo = toupper(novo.tipo);

    if (novo.tipo == 'F') {
        mvprintw(6, 2, "Nome: ");
        getnstr(novo.nome, 99);
        do {
            mvprintw(8, 2, "CPF (somente números): ");
            getnstr(novo.documento, 14);
            if (!validarCPF(novo.documento))
                mvprintw(9, 2, "CPF inválido. Tente novamente.\n");
        } while (!validarCPF(novo.documento));
    } else if (novo.tipo == 'J') {
        mvprintw(6, 2, "Razão Social: ");
        getnstr(novo.nome, 99);
        do {
            mvprintw(8, 2, "CNPJ (somente números): ");
            getnstr(novo.documento, 14);
            if (!validarCNPJ(novo.documento))
                mvprintw(9, 2, "CNPJ inválido. Tente novamente.\n");
        } while (!validarCNPJ(novo.documento));
    } else {
        mvprintw(10, 2, "Tipo inválido!");
        getch();
        return;
    }

    clientes[*qtd] = novo;
    (*qtd)++;

    mvprintw(11, 2, "Cliente cadastrado com sucesso!");
    mvprintw(13, 2, "Pressione qualquer tecla para voltar...");
    getch();
    noecho(); // desabilita eco...
    estado_atual = ST_MENU_PRINCIPAL;
}

// ---------------- LISTAR CLIENTES ----------------
void listarClientes(struct Cliente clientes[], int qtd) {
    clear();
    mvprintw(2, 2, "--- Lista de Clientes ---");
    if (qtd == 0) {
        mvprintw(4, 2, "Nenhum cliente cadastrado.");
        getch();
        return;
    }

    int y = 4;
    for (int i = 0; i < qtd; i++) {
        mvprintw(y++, 2, "Código: %d", clientes[i].codigo);
        mvprintw(y++, 2, "Tipo: %s", (clientes[i].tipo == 'F') ? "Pessoa Física" : "Pessoa Jurídica");
        mvprintw(y++, 2, "%s: %s", (clientes[i].tipo == 'F') ? "Nome" : "Razão Social", clientes[i].nome);
        mvprintw(y++, 2, "%s: %s", (clientes[i].tipo == 'F') ? "CPF" : "CNPJ", clientes[i].documento);
        y++;
    }

    mvprintw(y + 1, 2, "Pressione qualquer tecla para voltar...");
    getch();
    estado_atual = ST_MENU_PRINCIPAL;
}

// ---------------- MENU PRINCIPAL ----------------
void menuClientes() {
    const char *opcoes[] = {
        "Cadastrar Cliente",
        "Listar Clientes",
        "Sair"
    };
    int escolha = 0;
    int n_opcoes = 3;

    while (1) {
        clear();
        refresh();

        mvprintw(1, 2, "==== MENU CLIENTES ====");
        for (int i = 0; i < n_opcoes; i++) {
            if (i == escolha) attron(A_REVERSE);
            mvprintw(3 + i, 4, "%d - %s", i + 1, opcoes[i]);
            if (i == escolha) attroff(A_REVERSE);
        }
        refresh();

        int ch = getch();
        
        if(ch == KEY_UP && escolha > 0) {
            escolha--;
        }
        else if(ch == KEY_DOWN && escolha < n_opcoes - 1) {
            escolha++;
        } else if(ch == '\n' || ch == 10) {
            switch (escolha) {
                case 0:
                    estado_atual = ST_CLIENTE_CADASTRO;
                    break;

                case 1:
                    estado_atual = ST_CLIENTE_LISTA;
                    break;
                    
                case 2:
                    estado_atual = ST_MENU_PRINCIPAL;
                    break;
                }
                break;
        }
    }
}

// ---------------- MAIN ----------------
/*
int main() {
    struct Cliente clientes[MAX_CLIENTES];
    int qtd = 0;

    initscr(); // inicia ncurses
    while (1) {
        int opcao = menuPrincipal();

        if (opcao == 0)
            cadastrarCliente(clientes, &qtd);
        else if (opcao == 1)
            listarClientes(clientes, qtd);
        else if (opcao == 2)
            break;
    }

    endwin(); // encerra ncurses
    printf("\nEncerrando o programa...\n");
    return 0;
}
*/
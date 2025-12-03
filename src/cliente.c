#include "../include/cliente.h"
#include "../include/interface.h"
#include "../include/estados.h"
#include "../include/persistencia.h"
#include <string.h> // Adicionado para strcmp e outras funções
#include <ctype.h>  // Adicionado para toupper

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

// =====================================================
// ============ NOVO: BUSCAR POR DOCUMENTO =============
// =====================================================
// A função deve ser ajustada para usar a variável global qtd_clientes
int buscarClientePorDocumento(const char documento[]) {
    for (int i = 0; i < qtd_clientes; i++) {
        if (strcmp(clientes[i].documento, documento) == 0) {
            return 1; // Documento encontrado
        }
    }
    return 0; // Documento não encontrado
}


// =====================================================
// == NOVO: FUNÇÃO CENTRALIZADA DE VALIDAÇÃO E DUPLICIDADE ===
// =====================================================
int obterDocumentoComValidacao(struct Cliente *novo_cliente, int *qtd) {
    int tentar_novamente = 1;
    char tipo = novo_cliente->tipo;
    char doc_nome[20];
    
    // Define os nomes e funções de validação
    if (tipo == 'F') {
        strcpy(doc_nome, "CPF (somente números): ");
    } else {
        strcpy(doc_nome, "CNPJ (somente números): ");
    }

    do {
        clear();
        // Redesenha o contexto de cadastro
        mvprintw(2, 2, "--- Cadastro de Cliente ---");
        mvprintw(4, 2, "Pessoa Física (F) ou Jurídica (J): %c", tipo);
        mvprintw(6, 2, (tipo == 'F' ? "Nome: %s" : "Razão Social: %s"), novo_cliente->nome); 
        
        mvprintw(8, 2, doc_nome);
        getnstr(novo_cliente->documento, 14);

        int valido = (tipo == 'F') ? validarCPF(novo_cliente->documento) : validarCNPJ(novo_cliente->documento);
        int duplicado = 0;

        // Só verifica duplicidade se o documento for válido
        if (valido) {
            // A busca usa a variável global qtd_clientes carregada em cadastrarCliente
            duplicado = buscarClientePorDocumento(novo_cliente->documento);
        }

        if (!valido) {
            mvprintw(9, 2, "%s inválido!", (tipo == 'F' ? "CPF" : "CNPJ"));
        } else if (duplicado) {
            mvprintw(9, 2, "ERRO: %s já cadastrado!", (tipo == 'F' ? "CPF" : "CNPJ"));
        }

        if (!valido || duplicado) {
            mvprintw(11, 2, "Deseja tentar novamente (S) ou voltar ao menu (V)? ");
            
            char escolha = getch();
            escolha = toupper(escolha);

            if (escolha == 'V') {
                return 0; // Usuário escolheu voltar
            }
        } else {
            tentar_novamente = 0; // Documento válido e único
        }
    } while (tentar_novamente);

    return 1; // Documento obtido com sucesso
}


// =====================================================
// =============== FUNÇÃO REMOVER CLIENTE ==============
// =====================================================
void removerCliente(struct Cliente clientes[], int *qtd) {
    echo();
    curs_set(1);
    clear();

    carregarClientesCSV(clientes, qtd);

    if (*qtd == 0) {
        mvprintw(2, 2, "Nenhum cliente cadastrado!");
        getch();
        return;
    }

    int codigo;
    mvprintw(2, 2, "--- Remover Cliente ---");
    mvprintw(4, 2, "Digite o código do cliente: ");
    scanw("%d", &codigo);

    int index = -1;
    for (int i = 0; i < *qtd; i++) {
        if (clientes[i].codigo == codigo) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        mvprintw(6, 2, "Cliente não encontrado!");
        getch();
        return;
    }

    for (int i = index; i < *qtd - 1; i++) {
        clientes[i] = clientes[i + 1];
    }

    (*qtd)--;

    // Ajustar IDs após remoção
    for (int i = 0; i < *qtd; i++) {
        clientes[i].codigo = i + 1;
    }

    salvarClientesCSV(clientes, *qtd);

    mvprintw(8, 2, "Cliente removido com sucesso!");
    mvprintw(10, 2, "Pressione qualquer tecla para voltar...");
    getch();
}


// =====================================================
// =============== SALVAR EM CSV =======================
// =====================================================
void salvarClientesCSV(struct Cliente clientes[], int qtd) {
    FILE *fp = fopen(DIRETORIO_ARQUIVO_CLIENTE, "w");
    if (!fp) return;

    fprintf(fp, "codigo;tipo;nome;documento;telefone;endereco\n");

    for (int i = 0; i < qtd; i++) {
        fprintf(fp, "%d;%c;%s;%s;%s;%s\n",
            clientes[i].codigo,
            clientes[i].tipo,
            clientes[i].nome,
            clientes[i].documento,
            clientes[i].telefone,
            clientes[i].endereco
        );
    }

    fclose(fp);
}

// =====================================================
// =============== CARREGAR DADOS ======================
// =====================================================
void carregarClientesCSV(struct Cliente clientes[], int *qtd) {
    FILE *fp = fopen(DIRETORIO_ARQUIVO_CLIENTE, "r");
    if (!fp) return;

    char linha[512];
    fgets(linha, sizeof(linha), fp); // ignora cabeçalho

    *qtd = 0;

    while (fgets(linha, sizeof(linha), fp) != NULL && *qtd < MAX_CLIENTES) {
        char *token;
        struct Cliente c;

        // 1. Código
        token = strtok(linha, ";");
        if (token == NULL) continue;
        c.codigo = atoi(token); // Converte string para int

        // 2. Tipo
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        c.tipo = token[0];

        // 3. Nome/Razão
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(c.nome, token, 99);
        c.nome[99] = '\0';

        // 4. Documento
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(c.documento, token, 14);
        c.documento[14] = '\0';

        // 5. Telefone
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(c.telefone, token, 19);
        c.telefone[19] = '\0';

        // 6. Endereço (o último token deve ter o \n no final, precisa remover)
        token = strtok(NULL, "\n"); 
        if (token == NULL) continue;

        size_t len = strlen(token);
        if (len > 0 && token[len-1] == '\r') {
            token[len-1] = '\0';
        }

        strncpy(c.endereco, token, 149);
        c.endereco[149] = '\0';

        clientes[*qtd] = c;
        (*qtd)++;
    }

    fclose(fp);
}

// =====================================================
// =============== CADASTRO CLIENTE (Refatorado) =======
// =====================================================
void cadastrarCliente(struct Cliente clientes[], int *qtd) {
    echo();
    curs_set(1);

    struct Cliente novo;
    novo.codigo = *qtd + 1;

    clear();
    mvprintw(2, 2, "--- Cadastro de Cliente ---");
    mvprintw(4, 2, "Pessoa Física (F) ou Jurídica (J): ");
    scanw(" %c", &novo.tipo);
    novo.tipo = toupper(novo.tipo);

    mvprintw(6, 2, (novo.tipo == 'F') ? "Nome: " : "Razão Social: ");
    getnstr(novo.nome, 99);

    // 1. Carregar clientes para verificar duplicidade
    carregarClientesCSV(clientes, qtd);
    
    // 2. USO DA NOVA FUNÇÃO PARA OBTER E VALIDAR O DOCUMENTO
    if (novo.tipo == 'F' || novo.tipo == 'J') {
        if (obterDocumentoComValidacao(&novo, qtd) == 0) {
            // Se retornar 0, o usuário escolheu voltar
            estado_atual = ST_CLIENTE_PRINCIPAL;
            return;
        }
    } else {
        mvprintw(10, 2, "Tipo inválido!");
        getch();
        estado_atual = ST_CLIENTE_PRINCIPAL;
        return;
    }

    // TELEFONE ================================
    do {
        mvprintw(11, 2, "Telefone: ");
        getnstr(novo.telefone, 19);
        if (strlen(novo.telefone) < 3)
            mvprintw(12, 2, "Telefone inválido! Digite novamente.");
    } while (strlen(novo.telefone) < 3);

    // ENDEREÇO ================================
    do {
        mvprintw(14, 2, "Endereço: ");
        getnstr(novo.endereco, 149);
        if (strlen(novo.endereco) < 3)
            mvprintw(15, 2, "Endereço inválido! Digite novamente.");
    } while (strlen(novo.endereco) < 3);

    clientes[*qtd] = novo;
    (*qtd)++;

    salvarClientesCSV(clientes, *qtd);
    
    mvprintw(17, 2, "Cliente cadastrado com sucesso!");
    mvprintw(18, 2, "Pressione qualquer tecla para voltar...");
    getch();
    estado_atual = ST_CLIENTE_PRINCIPAL;

}

// =====================================================
// =============== LISTAR CLIENTES (Corrigido) =========
// =====================================================
void listarClientes(struct Cliente clientes[], int qtd) {
    clear();
    carregarClientesCSV(clientes, &qtd);

    mvprintw(2, 2, "--- Lista de Clientes ---");

    if (qtd == 0) {
        mvprintw(4, 2, "Nenhum cliente cadastrado.");
        mvprintw(6, 2, "Pressione qualquer tecla para voltar...");
        getch();
        
        estado_atual = ST_CLIENTE_PRINCIPAL; 
        return;
    }

    int y = 4;
    for (int i = 0; i < qtd; i++) {
        mvprintw(y++, 2, "Código: %d", clientes[i].codigo);
        mvprintw(y++, 2, "Tipo: %s", (clientes[i].tipo == 'F') ? "Pessoa Física" : "Pessoa Jurídica");
        mvprintw(y++, 2, "Nome/Razão: %s", clientes[i].nome);
        mvprintw(y++, 2, "Documento: %s", clientes[i].documento);
        mvprintw(y++, 2, "Telefone: %s", clientes[i].telefone);
        mvprintw(y++, 2, "Endereço: %s", clientes[i].endereco);
        y++; // linha em branco entre clientes
    }

    mvprintw(y + 2, 2, "Pressione qualquer tecla para voltar...");
    getch();

    estado_atual = ST_CLIENTE_PRINCIPAL;
}


// =====================================================
// =============== MENU CLIENTES =======================
// =====================================================
void menuClientes() {
    echo();
    curs_set(0);

    const char *opcoes[] = {
        "Cadastrar Cliente",
        "Listar Clientes",
        "Remover Cliente",
        "Sair"
    };

    int escolha = 0;
    int n_opcoes = 4;

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

        if(ch == KEY_UP && escolha > 0) escolha--;
        else if(ch == KEY_DOWN && escolha < n_opcoes - 1) escolha++;
        else if(ch == '\n') {
            switch (escolha) {
                case 0: estado_atual = ST_CLIENTE_CADASTRO; return;
                case 1: estado_atual = ST_CLIENTE_LISTA; return;
                case 2:
                    removerCliente(clientes, &qtd_clientes);
                    salvarClientesCSV(clientes, qtd_clientes);
                    break;
                case 3:
                    estado_atual = ST_MENU_PRINCIPAL; return;
            }
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
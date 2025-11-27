#include "../include/produto.h"
#include "../include/persistencia.h"
#include "../include/interface.h"
#include "../include/estados.h"
#include "../include/utils.h"
#include "../include/pedido.h"
#include <string.h>
#include <stdlib.h>

// estrutura para gerenciar a lista de produtos
typedef struct {
    Produto *produtos;
    int quantidade;
    int capacidade;
} GerenciadorProdutos;

// protótipos das funções internas
static int garantirCapacidade(GerenciadorProdutos *gerenciador, int capacidade_minima);
static int contarProdutosNoArquivo(FILE *arquivo);

int validarIdProduto(int id, char *mensagem) {
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PRODUTO, "r");
    if(!arq) {
        sprintf(mensagem, "Erro ao abrir arquivo de produto!");
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];
    //pPula o cabeçalho
    if (fgets(linha, sizeof(linha), arq) == NULL) {
        fclose(arq);
        return 0; // arquivo vazio
    }

    while(fgets(linha, sizeof(linha), arq)) {
        Produto P;
        char *token = strtok(linha, ";");
        if(token == NULL) continue;
        
        P.id = atoi(token);
        
        token = strtok(NULL, ";");
        if(token == NULL) continue;
        strncpy(P.descricao, token, DESCRICAO_LENGTH - 1);
        P.descricao[DESCRICAO_LENGTH - 1] = '\0';
        
        token = strtok(NULL, ";");
        if(token == NULL) continue;
        P.preco = atof(token);
        
        token = strtok(NULL, ";");
        if(token == NULL) continue;
        P.estoque = atoi(token);
        
        if(P.id == id) {
            sprintf(mensagem, "Produto já existe!");
            fclose(arq);
            return -2;
        }
    }

    fclose(arq);
    return 0;
}

GerenciadorProdutos* criarGerenciadorProdutos() {
    GerenciadorProdutos *gerenciador = malloc(sizeof(GerenciadorProdutos));
    if (gerenciador == NULL) {
        return NULL;
    }
    
    gerenciador->capacidade = 10;
    gerenciador->quantidade = 0;
    gerenciador->produtos = malloc(gerenciador->capacidade * sizeof(Produto));
    
    if (gerenciador->produtos == NULL) {
        free(gerenciador);
        return NULL;
    }
    
    return gerenciador;
}

void destruirGerenciadorProdutos(GerenciadorProdutos *gerenciador) {
    if (gerenciador != NULL) {
        if (gerenciador->produtos != NULL) {
            free(gerenciador->produtos);
        }
        free(gerenciador);
    }
}

static int garantirCapacidade(GerenciadorProdutos *gerenciador, int capacidade_minima) {
    if (gerenciador == NULL) return -1;
    
    if (gerenciador->capacidade >= capacidade_minima) {
        return 0; // tem capacidade suficiente
    }
    
    // calcula nova capacidade 
    int nova_capacidade = gerenciador->capacidade;
    while (nova_capacidade < capacidade_minima) {
        nova_capacidade *= 2;
        if (nova_capacidade <= 0) { 
            nova_capacidade = capacidade_minima;
            break;
        }
    }
    
    Produto *novo_array = realloc(gerenciador->produtos, nova_capacidade * sizeof(Produto));
    if (novo_array == NULL) {
        return -1;
    }
    
    gerenciador->produtos = novo_array;
    gerenciador->capacidade = nova_capacidade;
    return 0;
}

static int contarProdutosNoArquivo(FILE *arquivo) {
    if (arquivo == NULL) return 0;
    
    long posicao_atual = ftell(arquivo);
    int contador = 0;
    char linha[256];
    
    // pula a primeira linha (cabeçalho)
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        return 0;
    }
    
    // conta as linhas de dados
    while (fgets(linha, sizeof(linha), arquivo)) {
        // verifica se é uma linha válida 
        int tem_dados = 0;
        for (int i = 0; linha[i] != '\0'; i++) {
            if (linha[i] != ' ' && linha[i] != '\n' && linha[i] != '\r' && linha[i] != '\t') {
                tem_dados = 1;
                break;
            }
        }
        if (tem_dados) {
            contador++;
        }
    }
    
    // retorna ao ponto inicial
    fseek(arquivo, posicao_atual, SEEK_SET);
    return contador;
}

void inserirProduto(GerenciadorProdutos *gerenciador, WINDOW *win) {
    if (gerenciador == NULL) return;
    
    echo();
    curs_set(1);

    char mensagem[BUFFER_ARQUIVO_LINHA];

    // limpa a área de mensagens
    for (int i = 2; i < 12; i++) {
        mvwprintw(win, i, 2, "%100s", "");
    }
    wrefresh(win);

    // verifica e garante capacidade ANTES de inserir
    if (garantirCapacidade(gerenciador, gerenciador->quantidade + 1) != 0) {
        sprintf(mensagem, "Erro: Não foi possível alocar memória!");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        noecho();
        curs_set(0);
        return;
    }
    
    Produto novoProduto;
    
    // código do produto
    mvwprintw(win, 2, 2, "Código do produto: ");
    wrefresh(win);

    int validacao;

    do {
        novoProduto.id = input_int(win, 2, 22);
        if (novoProduto.id <= 0) {
            sprintf(mensagem, "Erro: Código deve ser positivo!");
            mvwprintw(win, 4, 2, "%s", mensagem);
            wrefresh(win);
            napms(2000);
            mvwprintw(win, 4, 2, "%100s", "");
            mvwprintw(win, 2, 22, "%10s", "");
            wrefresh(win);
            validacao = -1;
            continue;
        }
        
        validacao = validarIdProduto(novoProduto.id, mensagem);
        if(validacao != 0) {
            mvwprintw(win, 4, 2, "%s", mensagem);
            wrefresh(win);
            napms(2000);
            mvwprintw(win, 4, 2, "%100s", "");
            mvwprintw(win, 2, 22, "%10s", "");
            wrefresh(win);
        }
    } while(validacao != 0);
    
    // verifica se código já existe na memória
    if (analisarProduto(gerenciador->produtos, gerenciador->quantidade, novoProduto.id) != -1) {
        sprintf(mensagem, "Erro: Código do produto já existe!");
        mvwprintw(win, 4, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        noecho();
        curs_set(0);
        return;
    }
    
    // solicitar descrição
    mvwprintw(win, 4, 2, "Descrição: "); 
    wrefresh(win);
    echo();
    char descricao_temp[DESCRICAO_LENGTH];
    wgetnstr(win, descricao_temp, DESCRICAO_LENGTH - 1);
    noecho();
    
    // valida descrição não vazia
    if (strlen(descricao_temp) == 0) {
        sprintf(mensagem, "Erro: Descrição não pode ser vazia!");
        mvwprintw(win, 6, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        noecho();
        curs_set(0);
        return;
    }
    
    strncpy(novoProduto.descricao, descricao_temp, DESCRICAO_LENGTH - 1);
    novoProduto.descricao[DESCRICAO_LENGTH - 1] = '\0';
    
    // solicitar preço
    mvwprintw(win, 6, 2, "Preço: R$ ");
    wrefresh(win);
    novoProduto.preco = input_double(win, 6, 12);
    
    // valida preço positivo
    if (novoProduto.preco <= 0) {
        sprintf(mensagem, "Erro: Preço deve ser positivo!");
        mvwprintw(win, 8, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        noecho();
        curs_set(0);
        return;
    }
    
    // solicitar estoque
    mvwprintw(win, 8, 2, "Quantidade em estoque: ");
    wrefresh(win);
    novoProduto.estoque = input_int(win, 8, 25);
    
    // valida estoque não negativo
    if (novoProduto.estoque < 0) {
        sprintf(mensagem, "Erro: Estoque não pode ser negativo!");
        mvwprintw(win, 10, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        noecho();
        curs_set(0);
        return;
    }
    
    // adicionar à lista (CAPACIDADE GARANTIDA)
    gerenciador->produtos[gerenciador->quantidade] = novoProduto;
    gerenciador->quantidade++;
    
    salvarProdutosCSV(gerenciador->produtos, gerenciador->quantidade, win);

    mvwprintw(win, 10, 2, "Produto cadastrado com sucesso!");
    wrefresh(win);
    napms(2000);

    noecho();
    curs_set(0);
}

void listarProdutos(GerenciadorProdutos *gerenciador, WINDOW *win) {
    if (gerenciador == NULL) return;
    
    noecho();
    curs_set(0);

    char mensagem[BUFFER_ARQUIVO_LINHA];

    // limpa a janela
    werase(win);
    box(win, 0, 0);
    wrefresh(win);
    
    if (gerenciador->quantidade == 0) {
        sprintf(mensagem, "Nenhum produto cadastrado.");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        return;
    }
    
    int linha = 2;
    int max_y = getmaxy(win);
    int max_x = getmaxx(win);

    // cabeçalho
    mvwprintw(win, linha++, (max_x - 36) / 2, "----------- LISTA DE PRODUTOS -----------");
    linha++;
    
    // cabeçalho da tabela
    mvwprintw(win, linha++, 2, "%-6s %-30s %-12s %-8s", 
              "Código", "Descrição", "Preço", "Estoque");
    
    // linha separadora
    char separador[100];
    memset(separador, '-', sizeof(separador) - 1);
    separador[sizeof(separador) - 1] = '\0';
    mvwprintw(win, linha++, 2, "%.*s", max_x - 4, separador);
    
    // lista de produtos
    for (int i = 0; i < gerenciador->quantidade && linha < max_y - 3; i++) {
        mvwprintw(win, linha++, 2, "%-6d %-30s R$ %-9.2f %-8d", 
               gerenciador->produtos[i].id, 
               gerenciador->produtos[i].descricao, 
               gerenciador->produtos[i].preco, 
               gerenciador->produtos[i].estoque);
    }
    
    // nota abaixo da tela
    if (linha < max_y - 2) {
        mvwprintw(win, linha++, 2, "%.*s", max_x - 4, separador);
        mvwprintw(win, linha++, 2, "Total de produtos: %d", gerenciador->quantidade);
        mvwprintw(win, linha, 2, "Pressione qualquer tecla para continuar...");
    } else {
        mvwprintw(win, max_y - 2, 2, "Pressione qualquer tecla para continuar...");
    }
    
    wrefresh(win);
    wgetch(win);
}

void consultarProduto(GerenciadorProdutos *gerenciador, WINDOW *win) {
    if (gerenciador == NULL) return;
    
    echo();
    curs_set(1);

    char mensagem[BUFFER_ARQUIVO_LINHA];

    // limpa a área de mensagens
    for (int i = 2; i < 12; i++) {
        mvwprintw(win, i, 2, "%100s", "");
    }
    wrefresh(win);

    if (gerenciador->quantidade == 0) {
        sprintf(mensagem, "Nenhum produto cadastrado.");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        noecho();
        curs_set(0);
        return;
    }
    
    int id;
    mvwprintw(win, 2, 2, "Código do produto a consultar: ");
    wrefresh(win);
    id = input_int(win, 2, 34);
    
    int index = analisarProduto(gerenciador->produtos, gerenciador->quantidade, id);
    if (index == -1) {
        sprintf(mensagem, "Erro: Produto não encontrado!");
        mvwprintw(win, 4, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        noecho();
        curs_set(0);
        return;
    }
    
    // limpa e mostra os dados do produto
    for (int i = 4; i < 12; i++) {
        mvwprintw(win, i, 2, "%100s", "");
    }
    
    mvwprintw(win, 4, 2, "----------- DADOS DO PRODUTO -----------");
    mostrarProduto(gerenciador->produtos[index], win);
    mvwprintw(win, 10, 2, "Pressione qualquer tecla para continuar...");
    wrefresh(win);
    wgetch(win);

    noecho();
    curs_set(0);
}

void removerProduto(GerenciadorProdutos *gerenciador, WINDOW *win) {
    if (gerenciador == NULL) return;
    
    echo();
    curs_set(1);

    char mensagem[BUFFER_ARQUIVO_LINHA];
    
    // Limpa a área de mensagens
    for (int i = 2; i < 15; i++) {
        mvwprintw(win, i, 2, "%100s", "");
    }
    wrefresh(win);

    if (gerenciador->quantidade == 0) {
        sprintf(mensagem, "Nenhum produto cadastrado.");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        noecho();
        curs_set(0);
        return;
    }
    
    int id;
    mvwprintw(win, 2, 2, "Código do produto a remover: ");
    wrefresh(win);
    id = input_int(win, 2, 32);
    
    int index = analisarProduto(gerenciador->produtos, gerenciador->quantidade, id);
    if (index == -1) {
        sprintf(mensagem, "Erro: Produto não encontrado!");
        mvwprintw(win, 4, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        noecho();
        curs_set(0);
        return;
    }
    
    // confirmar remoção
    mvwprintw(win, 4, 2, "Produto a ser removido:");
    mostrarProduto(gerenciador->produtos[index], win);
    
    mvwprintw(win, 10, 2, "Confirmar remoção? (s/N): ");
    wrefresh(win);
    
    char confirmacao = wgetch(win);
    
    if (confirmacao == 's' || confirmacao == 'S') {
        // remover produto movendo os elementos
        for (int i = index; i < gerenciador->quantidade - 1; i++) {
            gerenciador->produtos[i] = gerenciador->produtos[i + 1];
        }
        gerenciador->quantidade--;
        
        // reduz capacidade se necessário (mantém mínimo de 10)
        if (gerenciador->capacidade > 10 && gerenciador->quantidade <= gerenciador->capacidade / 4) {
            int nova_capacidade = gerenciador->capacidade / 2;
            if (nova_capacidade < 10) nova_capacidade = 10;
            
            Produto *novo_array = realloc(gerenciador->produtos, nova_capacidade * sizeof(Produto));
            if (novo_array != NULL) {
                gerenciador->produtos = novo_array;
                gerenciador->capacidade = nova_capacidade;
            }
        }
        
        salvarProdutosCSV(gerenciador->produtos, gerenciador->quantidade, win);
        mvwprintw(win, 12, 2, "Produto removido com sucesso!");
    } else {
        mvwprintw(win, 12, 2, "Remoção cancelada.");
    }
    
    wrefresh(win);
    napms(2000);
    noecho();
    curs_set(0);
}

int analisarProduto(Produto *produtos, int quantidade, int id) {
    for (int i = 0; i < quantidade; i++) {
        if (produtos[i].id == id) {
            return i;
        }
    }
    return -1;
}

void salvarProdutosCSV(Produto *produtos, int quantidade, WINDOW *win) {
    noecho();
    curs_set(0);

    char mensagem[BUFFER_ARQUIVO_LINHA];
    FILE *arquivo = fopen(DIRETORIO_ARQUIVO_PRODUTO, "w");
    if (arquivo == NULL) {
        sprintf(mensagem, "Erro ao abrir arquivo para salvar produtos!");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        return;
    }
    
    fprintf(arquivo, "id;descricao;preco;estoque\n");
    
    for (int i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%d;%s;%.2f;%d\n",
                produtos[i].id,
                produtos[i].descricao,
                produtos[i].preco,
                produtos[i].estoque);
    }
    
    fclose(arquivo);
    
    // mensagem de sucesso
    sprintf(mensagem, "Dados salvos com sucesso!");
    mvwprintw(win, 2, 2, "%s", mensagem);
    wrefresh(win);
    napms(1000);
}

int carregarProdutosCSV(GerenciadorProdutos *gerenciador, char *mensagem) {
    if (gerenciador == NULL) {
        sprintf(mensagem, "Gerenciador de produtos não inicializado!");
        return -1;
    }
    
    FILE *arquivo = fopen(DIRETORIO_ARQUIVO_PRODUTO, "r");
    if (arquivo == NULL) {
        sprintf(mensagem, "Arquivo de produtos não encontrado. Iniciando com lista vazia.");
        return -1;
    }
    
    // conta produtos primeiro para alocar memória uma vez
    int total_produtos = contarProdutosNoArquivo(arquivo);
    
    if (total_produtos == 0) {
        fclose(arquivo);
        sprintf(mensagem, "Arquivo de produtos vazio.");
        gerenciador->quantidade = 0;
        return 0;
    }
    
    // capacidade para todos os produtos de uma vez
    if (garantirCapacidade(gerenciador, total_produtos) != 0) {
        fclose(arquivo);
        sprintf(mensagem, "Erro ao alocar memória para produtos!");
        return -1;
    }
    
    // pula cabeçalho
    char linha[256];
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        sprintf(mensagem, "Erro ao ler cabeçalho do arquivo!");
        return -1;
    }
    
    gerenciador->quantidade = 0;
    int produtos_carregados = 0;
    
    // carrega os produtos (SEM VERIFICAÇÃO DE CAPACIDADE NO LOOP)
    while (fgets(linha, sizeof(linha), arquivo) && produtos_carregados < total_produtos) {
        linha[strcspn(linha, "\n")] = 0;
        
        // pula linhas vazias
        int linha_vazia = 1;
        for (int i = 0; linha[i] != '\0'; i++) {
            if (linha[i] != ' ' && linha[i] != '\t') {
                linha_vazia = 0;
                break;
            }
        }
        if (linha_vazia) continue;

        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        
        gerenciador->produtos[gerenciador->quantidade].id = atoi(token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(gerenciador->produtos[gerenciador->quantidade].descricao, token, DESCRICAO_LENGTH - 1);
        gerenciador->produtos[gerenciador->quantidade].descricao[DESCRICAO_LENGTH - 1] = '\0';
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        gerenciador->produtos[gerenciador->quantidade].preco = atof(token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        gerenciador->produtos[gerenciador->quantidade].estoque = atoi(token);
        
        gerenciador->quantidade++;
        produtos_carregados++;
    }
    
    fclose(arquivo);
    sprintf(mensagem, "Produtos carregados: %d", gerenciador->quantidade);
    return 1;
}

void mostrarProduto(Produto produto, WINDOW *win) {
    noecho();
    curs_set(0);

    mvwprintw(win, 5, 4, "Código:    %d", produto.id);
    mvwprintw(win, 6, 4, "Descrição: %s", produto.descricao);
    mvwprintw(win, 7, 4, "Preço:     R$ %.2f", produto.preco);
    mvwprintw(win, 8, 4, "Estoque:   %d unidades", produto.estoque);

    echo();
    curs_set(1);
}

void menuProduto(GerenciadorProdutos *gerenciador) {
    if (gerenciador == NULL) return;
    
    noecho();
    curs_set(0);

    char *botoesMenu[] = {
        "Inserir produto",
        "Listar produtos", 
        "Consultar produto",
        "Remover produto",
        "Voltar"
    };
    const int totalBotoes = 5;
    int selecionado = 0;
    int ch;

    // carrega produtos do arquivo ao entrar no menu
    char mensagem[BUFFER_ARQUIVO_LINHA];
    carregarProdutosCSV(gerenciador, mensagem);

    while(1) {
        erase();
        refresh();

        // obtenção do tamanho da tela
        int altura, largura;
        getmaxyx(stdscr, altura, largura);

        // imprime dimensões da tela no canto da tela
        char texto_do_tamanho[32];
        snprintf(texto_do_tamanho, sizeof(texto_do_tamanho), "%dx%d", altura, largura);

        // dimensões da janela
        int altura_janela = altura - UI_MARGIN;
        int largura_janela = largura - UI_MARGIN;
        int posX_janela = UI_PADDING;
        int posY_janela = UI_PADDING;

        wrefresh(win);

        // impede que o terminal fique muito pequeno, por padrão as dimensões minimas são (20x60)
        if(ehTerminalPequeno(altura, largura)) continue;

        WINDOW *win = newwin(altura_janela, largura_janela, posX_janela, posY_janela);
        box(win, 0, 0);

        desenhaOpcoesVertical(win, botoesMenu, totalBotoes, selecionado, 1);

        mvwprintw(win, altura_janela - UI_MARGIN, largura_janela - (int)strlen(texto_do_tamanho) - UI_MARGIN, "%s", texto_do_tamanho);

        wrefresh(win);

        ch = getch();
        
        // titulo
        mvwprintw(win, 1, (largura_janela - 20) / 2, "MENU DE PRODUTOS");
        
        desenhaOpcoesVertical(win, botoesMenu, totalBotoes, selecionado, 3);

        // informação sobre produtos carregados
        mvwprintw(win, altura_janela - 3, 2, "Produtos na memória: %d", gerenciador->quantidade);
        mvwprintw(win, altura_janela - 2, largura_janela - (int)strlen(texto_do_tamanho) - 2, "%s", texto_do_tamanho);

        // controle dos botões, ENTER confirma e sai do loop
        if(ch == KEY_UP && selecionado > 0) selecionado--;
        else if(ch == KEY_DOWN && selecionado < totalBotoes - 1) selecionado++;
        else if(ch == 'q' || ch == 'Q' || ch == 27) { 
            estado_atual = ST_MENU_PRINCIPAL;
            delwin(win);
            break;
        } else if(ch == '\n' || ch == 10){ 
            werase(win);
            box(win, 0, 0);
            
            switch(selecionado){
                case 0:
                    inserirProduto(produtos, &quantidade, win);
                    break;

                case 1:
                    listarProdutos(produtos, quantidade, win);
                    break;

                case 2:
                    consultarProduto(produtos, quantidade, win);
                    break;

                case 3:
                    removerProduto(produtos, &quantidade, win);
                    break;

                case 4:
                    estado_atual = ST_MENU_PRINCIPAL;
                    delwin(win);
                    return;
            }
            
            // limpa a janela após a operação
            werase(win);
            box(win, 0, 0);
            wrefresh(win);
        }
    }
}


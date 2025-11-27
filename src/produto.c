#include "../include/produto.h"
#include "../include/persistencia.h"
#include "../include/interface.h"
#include "../include/estados.h"
#include <ncurses.h>
#include <string.h>

int validarIdProduto(int id, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_PRODUTO, "r");
    if(!arq) {
        sprintf(mensagem, "Erro ao abrir arquivo de produto!");
        return -1;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        Produto P;
        
        if(sscanf(linha, "%d,%[^100],%lf,%d", &P.id, P.descricao, &P.preco, &P.estoque) == 4){
            if(P.id == id){
                sprintf(mensagem, "Produto já existe!");
                fclose(arq);
                return -2;
            }
        }
    }

    fclose(arq);
    return 0;
}

void inserirProduto(Produto produtos[], int *quantidade, WINDOW *win) {
    char mensagem[BUFFER_ARQUIVO_LINHA];

    if (*quantidade >= MAX_PRODUTOS) {
        sprintf(mensagem, "Erro: Limite máximo de produtos atingido!");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
        return;
    }
    
    Produto novoProduto;
    
    // código do produto
    mvwprintw(win, 2, 2, "Código do produto: ");
    wrefresh(win);

    do{
        novoProduto.id = input_int(win, 2, 22);
        int validacao = validarIdProduto(novoProduto.id, mensagem);
        if(validacao != 0){
            mvwprintw(win, 4, 2, "%s", mensagem);
            wrefresh(win);
            napms(2000); // aguarda 2 segundos
            mvwprintw(win, 4, 2, "%100s", "");
            mvwprintw(win, 2, 22, "%10s", "");
            wrefresh(win);
        }
    } while(validarIdProduto(novoProduto.id, mensagem) != 0);
    
    // verifica se código já existe
    if (analisarProduto(produtos, *quantidade, novoProduto.id) != -1) {
        sprintf(mensagem, "Erro: Código do produto já existe!");
        mvwprintw(win, 4, 2, "%s", mensagem);
        wrefresh(win);
        return;
    }
    
    // solicitar descrição
    mvwprintw(win, 4, 2, "Descrição: "); 
    wrefresh(win);
    echo(); // habilita eco para entrada de string
    char descricao_temp[DESCRICAO_LENGTH];
    wgetnstr(win, descricao_temp, DESCRICAO_LENGTH - 1);
    noecho(); // desabilita eco
    strncpy(novoProduto.descricao, descricao_temp, DESCRICAO_LENGTH - 1);
    novoProduto.descricao[DESCRICAO_LENGTH - 1] = '\0';
    
    // solicitar preço
    mvwprintw(win, 6, 2, "Preço: ");
    wrefresh(win);
    novoProduto.preco = input_double(win, 6, 10);
    
    // solicitar estoque
    mvwprintw(win, 8, 2, "Quantidade em estoque: ");
    wrefresh(win);
    novoProduto.estoque = input_int(win, 8, 25);
    
    // adicionar à lista
    produtos[*quantidade] = novoProduto;
    (*quantidade)++;
    
    mvwprintw(win, 10, 2, "Produto cadastrado com sucesso!");
    wrefresh(win);
    napms(2000); // aguarda 2 segundos
}

void listarProdutos(Produto produtos[], int quantidade, WINDOW *win) {
    char mensagem[BUFFER_ARQUIVO_LINHA];
    
    if (quantidade == 0) {
        sprintf(mensagem, "Nenhum produto cadastrado.");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
        return;
    }
    
    int linha = 2;
    int max_y, max_x;
    getmaxyx(win, max_y, max_x);

    mvwprintw(win, linha++, 2, "-----------LISTA DE PRODUTOS-----------");
    mvwprintw(win, linha++, 2, "%-6s %-30s %-10s %-8s", "Código", "Descrição", "Preço", "Estoque");
    mvwprintw(win, linha++, 2, "------------------------------------------------------------");
    
    for (int i = 0; i < quantidade && linha < max_y - 2; i++) {
        mvwprintw(win, linha++, 2, "%-6d %-30s R$%-8.2f %-8d", 
               produtos[i].id, 
               produtos[i].descricao, 
               produtos[i].preco, 
               produtos[i].estoque);
    }
    
    mvwprintw(win, linha++, 2, "------------------------------------------------------------");
    mvwprintw(win, linha, 2, "Total de produtos: %d", quantidade);
    mvwprintw(win, linha + 1, 2, "Pressione qualquer tecla para continuar...");
    wrefresh(win);
    wgetch(win);
}

void consultarProduto(Produto produtos[], int quantidade, WINDOW *win) {
    char mensagem[BUFFER_ARQUIVO_LINHA];

    if (quantidade == 0) {
        sprintf(mensagem, "Nenhum produto cadastrado.");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        return;
    }
    
    int id;
    mvwprintw(win, 2, 2, "Código do produto a consultar: ");
    wrefresh(win);
    id = input_int(win, 2, 34);
    
    int index = analisarProduto(produtos, quantidade, id);
    if (index == -1) {
        sprintf(mensagem, "Erro: Produto não encontrado!");
        mvwprintw(win, 4, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        return;
    }
    
    mvwprintw(win, 4, 2, "-----------DADOS DO PRODUTO-----------");
    mostrarProduto(produtos[index], win);
    mvwprintw(win, 10, 2, "Pressione qualquer tecla para continuar...");
    wrefresh(win);
    wgetch(win);
}

void removerProduto(Produto produtos[], int *quantidade, WINDOW *win) {
    char mensagem[BUFFER_ARQUIVO_LINHA];
    
    if (*quantidade == 0) {
        sprintf(mensagem, "Nenhum produto cadastrado.");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        return;
    }
    
    int id;
    mvwprintw(win, 2, 2, "Código do produto a remover: ");
    wrefresh(win);
    id = input_int(win, 2, 32);
    
    int index = analisarProduto(produtos, *quantidade, id);
    if (index == -1) {
        sprintf(mensagem, "Erro: Produto não encontrado!");
        mvwprintw(win, 4, 2, "%s", mensagem);
        wrefresh(win);
        napms(2000);
        return;
    }
    
    // confirmar remoção
    mvwprintw(win, 4, 2, "Produto a ser removido:");
    mostrarProduto(produtos[index], win);
    
    mvwprintw(win, 10, 2, "Confirmar remoção? (s/N): ");
    wrefresh(win);
    
    char confirmacao = wgetch(win);
    
    if (confirmacao == 's' || confirmacao == 'S') {
        // remover produto movendo os elementos
        for (int i = index; i < *quantidade - 1; i++) {
            produtos[i] = produtos[i + 1];
        }
        (*quantidade)--;
        mvwprintw(win, 12, 2, "Produto removido com sucesso!");
    } else {
        mvwprintw(win, 12, 2, "Remoção cancelada.");
    }
    
    wrefresh(win);
    napms(2000);
}

int analisarProduto(Produto produtos[], int quantidade, int id) {
    for (int i = 0; i < quantidade; i++) {
        if (produtos[i].id == id) {
            return i; // retorna o índice do produto encontrado
        }
    }
    return -1; // produto não encontrado
}

void salvarProdutosCSV(Produto produtos[], int quantidade, WINDOW *win) {
    char mensagem[BUFFER_ARQUIVO_LINHA];
    FILE *arquivo = fopen("data/Produtos.csv", "w");
    if (arquivo == NULL) {
        sprintf(mensagem, "Erro ao abrir arquivo para salvar produtos!");
        mvwprintw(win, 2, 2, "%s", mensagem);
        wrefresh(win);
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
    sprintf(mensagem, "Produtos salvos em data/Produtos.csv");
    mvwprintw(win, 2, 2, "%s", mensagem);
    wrefresh(win);
    napms(2000);
}

void carregarProdutosCSV(Produto produtos[], int *quantidade, char *mensagem) {
    FILE *arquivo = fopen("data/produto.csv", "r");
    if (arquivo == NULL) {
        sprintf(mensagem, "Arquivo de produtos não encontrado. Iniciando com lista vazia.");
        return;
    }
    
    char linha[256];
    fgets(linha, sizeof(linha), arquivo);
    
    *quantidade = 0;
    while (fgets(linha, sizeof(linha), arquivo) && *quantidade < MAX_PRODUTOS) {
        linha[strcspn(linha, "\n")] = 0;

        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        
        produtos[*quantidade].id = atoi(token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(produtos[*quantidade].descricao, token, DESCRICAO_LENGTH - 1);
        produtos[*quantidade].descricao[DESCRICAO_LENGTH - 1] = '\0';
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        produtos[*quantidade].preco = atof(token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        produtos[*quantidade].estoque = atoi(token);
        
        (*quantidade)++;
    }
    
    fclose(arquivo);
    sprintf(mensagem, "Produtos carregados: %d", *quantidade);
}

void mostrarProduto(Produto produto, WINDOW *win) {
    mvwprintw(win, 5, 2, "Código: %d", produto.id);
    mvwprintw(win, 6, 2, "Descrição: %s", produto.descricao);
    mvwprintw(win, 7, 2, "Preço: R$ %.2f", produto.preco);
    mvwprintw(win, 8, 2, "Estoque: %d unidades", produto.estoque);
}

void menuProduto(Produto produtos[], int *quantidade){
    char *botoesMenu[] = {
        "Inserir produto",
        "Listar produto",
        "Consultar produto",
        "Remover produto",
        "Voltar"
    };
    const int totalBotoes = 5;
    int selecionado = 0;
    int ch;

    erase();
    refresh();

    while(1){
        // obtenção do tamanho da tela e das coordenadas de inicio
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

        // impede que o terminal fique muito pequeno, por padrão as dimensões minimas são (20x60)
        if(ehTerminalPequeno(stdscr, altura, largura)) continue;

        WINDOW *win = newwin(altura_janela, largura_janela, posX_janela, posY_janela);
        box(win, 0, 0);

        desenhaBotoes(win, botoesMenu, totalBotoes, selecionado);

        mvwprintw(win, altura_janela - UI_MARGIN, largura_janela - (int)strlen(texto_do_tamanho) - UI_MARGIN, "%s", texto_do_tamanho);

        wrefresh(win);

        ch = getch();

        // controle dos botões, ENTER confirma e sai do loop
        if(ch == KEY_LEFT && selecionado > 0) selecionado--;
        else if(ch == KEY_RIGHT && selecionado < totalBotoes - 1) selecionado++;
        else if(ch == 'q' || ch == 'Q' || ch == 27) { 
            estado_atual = ST_MENU_PRINCIPAL;
            delwin(win);
            break;
        } else if(ch == '\n' || ch == 10){ 
            werase(win);
            box(win, 0, 0);
            
            switch(selecionado){
                case 0:
                    inserirProduto(produtos, quantidade, win);
                    break;

                case 1:
                    listarProdutos(produtos, *quantidade, win);
                    break;

                case 2:
                    consultarProduto(produtos, *quantidade, win);
                    break;

                case 3:
                    removerProduto(produtos, quantidade, win);
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
        
        delwin(win);
    }
}

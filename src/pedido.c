#include "../include/pedido.h"

/* Constates e definições */

/* 
Descrição de Cenários

Cadastrar Pedido - Descrição do fluxo principal/alternativo
    1. O usuário informa ao sistema o código identificador do pedido
    2. (Analisa Pedido) O sistema verifica se o respectivo código já existe. Caso exista o sistema retorna
    uma mensagem de erro informando que o código já existe e retorna para tela de cadastro com o
    cursor no campo de código para ser preenchido novamente.
    3. O usuário informa o código do cliente do referido pedido.
    4. (Analisa Cliente) O sistema verifica se o respectivo cliente já existe. Caso não exista, o sistema retorna
    uma mensagem de erro informando que o cliente não existe e retorna para tela de cadastro com o
    cursor no campo de código do cliente para ser preenchido novamente.
    5. (Cadastrar Itens de Pedido) O usuário informa o código do produto a ser cadastrado como item do
    pedido.
    6. (Cadastrar Itens de Pedido) (Analisa Produto) O sistema verifica se o produto referenciado pelo código
    existe na lista de produtos. Caso não exista, o sistema retorna uma mensagem de erro informando
*/

/* Variáveis globais inicializadas */

void cadastrarPedido(struct Pedido *P){
    const char *dataAtual = obterDataAtual();
    
    do{
        printf("Id do pedido: ");
        scanf("%d", P->id);
        getchar();
    } while(!validarIdPedido(P->id));

    do{
        printf("Id do cliente: ");
        scanf("%d", P->clienteId);
        getchar();
    } while(!validarIdCliente(P->clienteId));

    do{
        printf("Data (use o formato %s): ", dataAtual);
        fgets(P->data, sizeof(P->data), stdin);
        P->data[strcspn(P->data, "\n")] = '\0';
    } while(!validarData(P->data));

    printf("Total: ");
    scanf("%lf", P->total);
    getchar();

    guardarPedido(P);
}

void listarPedidos(){
    FILE *arq = fopen(NOME_ARQUIVO_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return;
    }

    char linha[TAMANHO_BUFFER_LINHA];

    printf("+------------+----------------+-------------+----------+\n");
    printf("| ID         | ID do Cliente  | Data        | Total    |\n");
    printf("+------------+----------------+-------------+----------+\n");

    while(fgets(linha, sizeof(linha), arq) != NULL){
        struct Pedido P;

        if(sscanf(linha, "%d,%49[^,],%f", &P.id, &P.clienteId, P.data, &P.total) == 4){
            printf("| %-10d | %-14d | %-11s | %-8.2f |\n", P.id, P.clienteId, P.data, P.total);
        }
    }

    printf("+------------+----------------+-------------+----------+\n");
    
    fclose(arq);
}

int analisarPedido(int idDoPedido){
    FILE *arq = fopen(NOME_ARQUIVO_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return -1;
    }

    char linha[TAMANHO_BUFFER_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct Pedido P;

        if(sscanf(linha, "%d,%49[^,],%f", &P.id, &P.clienteId, P.data, &P.total) == 4 && (P.id == idDoPedido)){
            perror("CÓDIGO DE PRODUTO JÁ EXISTE!");
            return -2;
        }
    }
    
    fclose(arq);
    return 0; // 0 -> sucesso
}

int analisarItemPedido(int idDoItemPedido){
    FILE *arq = fopen(NOME_ARQUIVO_PEDIDO, "r");
    if(!arq){
        perror("ERRO AO ABRIR O ARQUIVO!");
        return -1;
    }

    char linha[TAMANHO_BUFFER_LINHA];

    while(fgets(linha, sizeof(linha), arq) != NULL){
        struct Pedido P;

        if(sscanf(linha, "%d,%49[^,],%f", &P.id, &P.clienteId, P.data, &P.total) == 4 && (P.id == idDoItemPedido)){
            perror("CÓDIGO DE PRODUTO JÁ EXISTE!");
            return -2;
        }
    }
    
    fclose(arq);
    return 0; // 0 = sucesso
}

void cadastrarItemPedido(struct ItemPedido *IP){
    // IMPLEMENTAR FUNÇÕES DE VALIDAÇÃO DE CADA CAMPO
    do{
        printf("Id do Pedido: ");
        scanf("%d", IP->pedidoId);
        getchar();
    } while (!analisarItemPedido(IP->pedidoId));

    printf("Id do Produto: ");
    scanf("%d", IP->produtoId);
    getchar();

    printf("Quantidade: ");
    scanf("%d", IP->quantidade);
    getchar();

    printf("Subtotal: ");
    scanf("%lf", IP->subtotal);
    getchar();

    guardarItemPedido(IP);
}


void removerPedido(){
    int idRemover;

    printf("Digite o id do pedido a ser removido: ");
    scanf("%d", &idRemover);

    apagarPedido(idRemover);
}

void removerItemPedido(struct ItemPedido *IP){
    int idRemover;

    printf("Digite o id do item de pedido a ser removido: ");
    scanf("%d", &idRemover);

    apagarItemPedido(idRemover);
}

void desenhaMenu(WINDOW *menu_win, int destacado, char *opcoes[], int n_opcoes){
    werase(menu_win);
    box(menu_win, 0, 0);

    for(int i=0; i<n_opcoes; i++){
        if (i == destacado) wattron(menu_win, A_REVERSE);

        mvprintw(menu_win, i+1, 2, "%s", opcoes[i]); // Desenha cada uma das opções

        if(i == destacado) wattroff(menu_win, A_REVERSE);
    }

    wrefresh(menu_win); // Atualiza a janela
}

void telaPedidos(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    // Obtenção do tamanho da tela e das cooredenadas de inicio
    int altura, largura; 
    getmaxyx(stdscr, altura, largura);
    int inicio_y = (LINES - altura) / 2;
    int inicio_x = (COLS - largura) / 2;

    // Janelas
    WINDOW *JanelaMenu = newwin(altura, largura, inicio_y, inicio_x);
    WINDOW *TituloPedidos = newwin(10, 40, 0, 0);
    WINDOW *JanelaOpcoes = newwin(50, 30, 0, 0);
    WINDOW *JanelaEscolhas = newwin(altura - 5, largura - 10, inicio_y + 5, inicio_x + 5);

    void desenhaMenu(WINDOW *menu_win, int destacado, char *opcoes[], int n_opcoes); 

    int escolha = 0;
    int destacado = 0;
    int ch;
    const int NUM_OPCOES_PEDIDO = 5;
    const int NUM_OPCOES_ITEM_PEDIDO = 4;

    char *opcoesPedido[] = {
        "Cadastrar Pedido",
        "Listar Pedidos",
        "Detalhar Pedido",
        "Apagar Pedido",
        "Sair"
    };

    char *opcoesItemPedido[] = {
        "Cadastrar Item de Pedido",
        "Listar Item de Pedidos",
        "Apagar Item de Pedido",
        "Sair"
    };
    
    // Escreve em cada janela 
    box(TituloPedidos, 0, 0);
    mvwprintw(TituloPedidos, (altura - 5)/2, (largura - 2)/2, "Pedidos");
    
    while(1){
        desenhaMenu(JanelaEscolhas, destacado, opcoesPedido, NUM_OPCOES_PEDIDO);
        ch = wgetch(JanelaEscolhas);

        switch(ch){
            case KEY_UP:
                if (destacado > 0) destacado--;
                break;

            case KEY_DOWN:
                if (destacado < NUM_OPCOES_PEDIDO - 1) destacado++;
                break;

            case 10: // ENTER não funciona por algum motivo lol
                escolha = destacado;
                break;

            case 'q':
            case 'Q':
                escolha = NUM_OPCOES_PEDIDO - 1;
                break;
        }
        if (ch == 10 || ch == 'q' || ch == 'Q' && escolha == NUM_OPCOES_PEDIDO - 1) break;  // Sair 
        else { 
            clear(); 
            attron(COLOR_PAIR(1) | A_BOLD); 
            mvprintw(LINES/2, (COLS - 30)/2, "Você escolheu: %s", opcoesPedido[escolha]); 
            attroff(COLOR_PAIR(1) | A_BOLD); 
            mvprintw(LINES/2 + 2, (COLS - 30)/2, "Pressione qualquer tecla..."); 
            refresh(); 
        }

        // Exemplo de animação simples 
        for (int i = 0; i < 5; i++) { 
            wclear(JanelaOpcoes); 
            box(JanelaOpcoes, 0, 0); 
            mvwprintw(JanelaOpcoes, 0, 2, "[ Janela de Status ]"); 
            mvwprintw(JanelaOpcoes, 3, 2, "Atualizando: %d/5", i + 1); 
            wrefresh(JanelaOpcoes); 
            napms(500);  // Pausa de 500ms 
        }
        
        mvwprintw(JanelaOpcoes, 5, 2, "Concluído! Pressione qualquer tecla..."); 
        wrefresh(JanelaOpcoes); 

        // Libera memória das janelas 
        delwin(JanelaMenu); 
        delwin(JanelaEscolhas); 
        delwin(JanelaOpcoes); 
        endwin();
    }
}

int main(){
    telaPedidos();
}
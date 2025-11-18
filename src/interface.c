#include "../include/interface.h"

// ===========================================================
//   FUNÇÕES UTILITARIAS, PODEM SER USADAS EM OUTROS PROGRAMAS
// ===========================================================

void inicializaInterface(){
    initscr();              // Inicia a tela
    cbreak();               // Sai do loop se tiver algum erro no código
    noecho();               // Remove o cursor do terminal
    keypad(stdscr, TRUE);   // Permite usar o teclado
    curs_set(0);            // Oculta o cursor
}

void finalizaInterface(){
    endwin();
}

//  =========================================================
//   FUNÇÕES ESTÁTICAS, USADAS APENAS DENTRO DESSE ESCOPO
//  =========================================================

// Função que carrega a logo, como isso acontece só uma vez, usa-se static ponteiro ponteiro char (passagem de parâmentro para um vetor de char ponteiro);
static char **carregaLogo(const char *caminho, int *linhas){
    FILE *arq = fopen(caminho, "r");
    if(!arq){
        perror("ERRO AO ABRIR ARQUIVO DA LOGO");
        return NULL;
    }

    char linha[BUFFER_LINHA_LOGO];
    char **logo = NULL;
    int countLinhas = 0;

    while(fgets(linha, sizeof(linha), arq)){
        linha[strcspn(linha, "\r\n")] = '\0';

        char *copia = strdup(linha);
        if(!copia){
            break;
        }

        char **temp = realloc(logo, sizeof(char*) * (countLinhas + 1));
        if(!temp){
            free(copia);
            break;
        }

        logo = temp;
        logo[countLinhas++] = copia;
    }

    fclose(arq);
    *linhas = countLinhas;
    return logo;
}

// Liberação da alocação dinâmica da logo
static void liberaLogo(char **logo, int linhas){
    if(!logo) return; // Já liberado
    for(int i=0; i<linhas; i++) free(logo[i]);
    free(logo);
}

static void desenhaLogo(WINDOW *win, char **logo, int linhas){
    int altura, largura;
    getmaxyx(win, altura, largura);

    // Linha de maior tamanho da logo, para definir o tamanho minimo que a logo deve ter para aparecer
    int tamanhoMax = 0;
    for(int i=0; i<linhas; i++){
        int len = (int)strlen(logo[i]);
        if(len > tamanhoMax) tamanhoMax = len;
    }

    // Redimensionamento da logo
    int espaco_para_logo = altura - 6;
    if(espaco_para_logo < 1) return; // Não renderiza logo se for pequeno

    int desenha_linhas = linhas;
    if(desenha_linhas > espaco_para_logo) desenha_linhas = espaco_para_logo;    // Altura minima

    int inicio_x = (largura - tamanhoMax) / 2;
    if(inicio_x < 1) inicio_x = 1;  // Largura minima

    int inicio_y = 2;

    for(int i=0; i<desenha_linhas; i++){
        int maxPrint = largura - 4;
        if(maxPrint <= 0) break;

        mvwprintw(win, inicio_y + i, inicio_x, "%.*s", maxPrint, logo[i]);
    }
}

// Desenha botões do menu com destaque
static void desenhaBotoes(WINDOW *win, char *labels[], int total, int selecionado){
    if(!win || total <= 0) return; // Não renderiza se não tiver botões ou janela

    int altura, largura;
    getmaxyx(win, altura, largura);

    // Layout dos botões
    int espacamento = 4;

    int largura_total = 0;
    for(int i=0; i<total; i++){
        int len_botao = (int)strlen(labels[i] + 4);
        largura_total += len_botao;
    }
    largura_total += espacamento * (total - 1);

    // Horizontal
    int inicio_x = (largura - largura_total) / 2;
    if(inicio_x < 1) inicio_x = 1;

    // Vertical
    int inicio_y = altura - 3;
    if(inicio_y < 1) inicio_y = 1;

    int x = inicio_x;
    int y = inicio_y;

    for(int i=0; i<total; i++){
        char labelFormatado[BUFFER_LINHA_CARACTERES];
        snprintf(labelFormatado, sizeof(labelFormatado), "[     %s     ]", labels[i]);

        if(i == selecionado){
            wattron(win, A_REVERSE | A_BOLD);
            mvwprintw(win, y, x, "%s", labelFormatado);
            wattroff(win, A_REVERSE | A_BOLD);
        } else {
            mvwprintw(win, y, x, "%s", labelFormatado);
        }

        x += (int)strlen(labelFormatado) + espacamento;
    }
}

//  =========================================================
//                      MENU PRINCIPAL
//  =========================================================

void menuPrincipal(){
    int linhasLogo = 0;
    char **logo = carregaLogo("public/logo.txt", &linhasLogo);

    char *botoesMenu[] = {
        "Clientes",
        "Produtos",
        "Pedidos",
        "Sair"
    };
    const int totalBotoes = 4;
    int selecionado = 0;

    erase();
    refresh();

    // LOOP PRINCIPAL
    while(1){
        // Obtenção do tamanho da tela e das cooredenadas de inicio
        int altura, largura;
        getmaxyx(stdscr, altura, largura);

        // Imprime dimensões da tela no canto da tela
        char texto_do_tamanho[32];
        snprintf(texto_do_tamanho, sizeof(texto_do_tamanho), "%dx%d", altura, largura);

        // Dimensões da janela
        int altura_janela = altura - UI_MARGIN;
        int largura_janela = largura - UI_MARGIN;
        int posX_janela = UI_PADDING;
        int posY_janela = UI_PADDING;

        // Impede que o terminal fique muito pequeno, por padrão as dimensões minimas são (10x30)
        if(altura_janela < SCREEN_MIN_HEIGHT || largura_janela < SCREEN_MIN_WIDTH){
            mvprintw(0, 0, "Terminal muito pequeno %s", texto_do_tamanho);
            refresh();
            getch();
            clear();
            continue;
        }

        WINDOW *janelaPrincipal = newwin(altura_janela, largura_janela, posX_janela, posY_janela);
        box(janelaPrincipal, 0, 0);

        // Desenho da UI
        desenhaLogo(janelaPrincipal, logo, linhasLogo);
        desenhaBotoes(janelaPrincipal, botoesMenu, totalBotoes, selecionado);

        mvwprintw(janelaPrincipal, altura_janela - UI_MARGIN, largura_janela - (int)strlen(texto_do_tamanho) - UI_MARGIN, "%s", texto_do_tamanho);

        // A cada caracter obtido, a janela é atualizada
        wrefresh(janelaPrincipal);

        int ch = getch();

        delwin(janelaPrincipal);

        // Controle dos botões, ENTER confirma e sai do loop
        if(ch == KEY_LEFT && selecionado > 0) selecionado--;
        else if(ch == KEY_RIGHT && selecionado < totalBotoes - 1) selecionado++;
        else if(ch == '\n') break;
        else if(ch == 'q' || ch == 'Q' || ch == 27) { // ESC
            selecionado = totalBotoes - 1; // Seleciona sair
            break;
        }
    }

    if (logo) liberaLogo(logo, linhasLogo);
}

void telaClientes();

void telaProdutos();

//  =========================================================
//                      MENU PRINCIPAL
//  =========================================================

void telaPedidos(){
    // Obtenção do tamanho da tela e das cooredenadas de inicio
    int altura, largura; 
    getmaxyx(stdscr, altura, largura);
    int inicio_y = (LINES - altura) / 2;
    int inicio_x = (COLS - largura) / 2;

    // Janelas
    WINDOW *JanelaMenu = newwin(altura, largura, inicio_y, inicio_x);
    WINDOW *TituloPedidos = newwin(altura - 20, largura - 20, inicio_y, inicio_x);
    WINDOW *JanelaOpcoes = newwin(altura - 5, largura - 5, inicio_y + 10, inicio_x);
    WINDOW *JanelaEscolhas = newwin(altura - 5, largura - 10, inicio_y + 5, inicio_x + 5);

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
        //desenha(JanelaEscolhas, destacado, opcoesPedido, NUM_OPCOES_PEDIDO);
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
        getchar();

        // Libera memória das janelas 
        delwin(JanelaMenu); 
        delwin(JanelaEscolhas); 
        delwin(JanelaOpcoes); 
        endwin();
    }
}

// TESTES

int main(){
    inicializaInterface();

    menuPrincipal();

    finalizaInterface();

    return 0;
}
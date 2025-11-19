#include "../include/interface.h"
#include "../include/estados.h"
#include "../include/cliente.h"
#include "../include/produto.h"
#include "../include/pedido.h"
#include "../include/utils.h"

// ===========================================================
// FUNÇÕES UTILITARIAS DE UI , PODEM SER USADAS EM OUTROS PROGRAMAS
// ===========================================================

void inicializaInterface(){
    initscr();              // Inicia a tela
    cbreak();               // Sai do loop se tiver algum erro no código
    noecho();               // Remove o cursor do terminal
    keypad(stdscr, true);   // Permite usar o teclado
    curs_set(0);            // Oculta o cursor
}

void finalizaInterface(){
    touchwin(stdscr);
    clear();
    refresh();
    endwin();
}

int ehTerminalPequeno(WINDOW *win, int altura_janela, int largura_janela){
    // Obtenção do tamanho da tela e das cooredenadas de inicio
    int altura, largura;
    getmaxyx(stdscr, altura, largura);

    // Obtém as dimensões da tela
    char texto_do_tamanho[32];
    snprintf(texto_do_tamanho, sizeof(texto_do_tamanho), "%dx%d", altura, largura);

    // Impede que o terminal fique muito pequeno, por padrão as dimensões minimas são (20x60)
    if(altura_janela < SCREEN_MIN_HEIGHT || largura_janela < SCREEN_MIN_WIDTH){
        mvprintw(0, 0, "Terminal muito pequeno! %s", texto_do_tamanho);
        refresh();
        getch();
        clear();
        return 1;
    }
    return 0;
}

int input_int(WINDOW *win, int y, int x){
    char buffer[BUFFER_LINHA_CARACTERES];
    mvwgetnstr(win, y, x, buffer, BUFFER_LINHA_CARACTERES - 1);
    return atoi(buffer);
}

double input_double(WINDOW *win, int y, int x){
    char buffer[BUFFER_LINHA_CARACTERES];
    mvwgetnstr(win, y, x, buffer, BUFFER_LINHA_CARACTERES - 1);
    return atof(buffer);
}

void input_string(WINDOW *win, int y, int x, char *output, int maxlen){
    mvwgetnstr(win, y, x, output, maxlen - 1);
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

    int inicio_y = 5;

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
        int len_botao = (int)strlen(labels[i]) + 12;
        largura_total += len_botao;
    }
    largura_total += espacamento * (total - 1);

    // Horizontal
    int inicio_x = (largura - largura_total) / 2;
    if(inicio_x < 1) inicio_x = 1;

    // Vertical
    int inicio_y = altura - 6;
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
//                      ABERTURA DO PROGRAMA
//  =========================================================

static int animaLogo(WINDOW *win, char **logo, int linhas, int delay_secs){
    keypad(win, true);
    nodelay(win, true);

    int altura, largura;
    getmaxyx(win, altura, largura);

    char *botoesMenu[] = {
        "Clientes",
        "Produtos",
        "Pedidos",
        "Sair"
    };

    const int totalBotoes = 4;
    int selecionado = 0;

    // Linha de maior tamanho da logo, para definir o tamanho minimo que a logo deve ter para aparecer
    int tamanhoMax = 0;
    for(int i=0; i<linhas; i++){
        int len = (int)strlen(logo[i]);
        if(len > tamanhoMax) tamanhoMax = len;
    }

    // Redimensionamento da logo
    int espaco_para_logo = altura - 6;
    if(espaco_para_logo < 1) return 1; // Pula renderização se for pequeno

    int desenha_linhas = linhas;
    if(desenha_linhas > espaco_para_logo) desenha_linhas = espaco_para_logo;    // Altura minima

    int inicio_x = (largura - tamanhoMax) / 2;
    if(inicio_x < 1) inicio_x = 1;  // Largura minima

    int inicio_y = 5;

    int ch;
    int interrompido = 0;

    for(int i=0; i<desenha_linhas; i++){
        ch = wgetch(win);
        if(ch != ERR){
            interrompido = 1;
            break;
        }

        int maxPrint = largura - 4;
        if(maxPrint <= 0) break;

        mvwprintw(win, inicio_y + i, inicio_x, "%.*s", maxPrint, logo[i]);
        wrefresh(win);

        napms(delay_secs);
    }

    desenhaBotoes(win, botoesMenu, totalBotoes, selecionado);
    napms(delay_secs);

    nodelay(win, false);
    return interrompido;
}

void animacaoAbertura(){
    int linhasLogo = 0;
    char **logo = carregaLogo("public/logo.txt", &linhasLogo);

    erase();
    refresh();

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

        // Impede que o terminal fique muito pequeno, por padrão as dimensões minimas são (20x60)
        if(ehTerminalPequeno(stdscr, altura, largura)) continue;

        WINDOW *janelaPrincipal = newwin(altura_janela, largura_janela, posX_janela, posY_janela);
        keypad(stdscr, true);
        box(janelaPrincipal, 0, 0);

        // Desenho da UI
        int pulou = animaLogo(janelaPrincipal, logo, linhasLogo, 250);

        if(!pulou){
            wrefresh(janelaPrincipal);
            desenhaLogo(janelaPrincipal, logo, linhasLogo);
        } else {
            break;
        }

        mvwprintw(janelaPrincipal, altura_janela - UI_MARGIN, largura_janela - (int)strlen(texto_do_tamanho) - UI_MARGIN, "%s", texto_do_tamanho);
        
        wrefresh(janelaPrincipal);
        delwin(janelaPrincipal);

        // Sai automaticamente do loop afinal é uma animação
        napms(500);
        break;
    }

    if (logo) liberaLogo(logo, linhasLogo);
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
    int ch;

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

        // Impede que o terminal fique muito pequeno, por padrão as dimensões minimas são (20x60)
        if(ehTerminalPequeno(stdscr, altura, largura)) continue;

        WINDOW *janelaPrincipal = newwin(altura_janela, largura_janela, posX_janela, posY_janela);
        box(janelaPrincipal, 0, 0);

        // Desenho da UI
        desenhaLogo(janelaPrincipal, logo, linhasLogo);
        desenhaBotoes(janelaPrincipal, botoesMenu, totalBotoes, selecionado);

        mvwprintw(janelaPrincipal, altura_janela - UI_MARGIN, largura_janela - (int)strlen(texto_do_tamanho) - UI_MARGIN, "%s", texto_do_tamanho);

        // A cada caracter obtido, a janela é atualizada
        wrefresh(janelaPrincipal);

        ch = getch();

        delwin(janelaPrincipal);

        // Controle dos botões, ENTER confirma e sai do loop
        if(ch == KEY_LEFT && selecionado > 0) selecionado--;
        else if(ch == KEY_RIGHT && selecionado < totalBotoes - 1) selecionado++;
        else if(ch == '\n') break;
        else if(ch == 'q' || ch == 'Q' || ch == 27) { // ESC
            selecionado = totalBotoes - 1; // Seleciona sair
            break;
        } else if(ch == '\n' || ch == 10){ // ENTER
            switch(selecionado){
                case 0:
                    //estado_atual = ST_CLIENTE_PRINCIPAL;
                    menuClientes();
                    break;

                case 1:
                    //estado_atual = ST_PRODUTO_PRINCIPAL;
                    //menuProdutos();
                    break;

                case 2:
                    //estado_atual = ST_PEDIDO_PRINCIPAL;
                    menuPedidos();
                    break;

                case 3:
                    estado_atual = ST_SAINDO;
                    break;
            }
        }
    }

    if (logo) liberaLogo(logo, linhasLogo);
}

void menuConfiguracao(){
    // Implementação futura
}
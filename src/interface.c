#include <ncurses.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

void menuPrincipal();

void telaClientes();

void telaProdutos();

void telaPedidos(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    // Obtenção do tamanho da tela
    int altura, largura; 
    getmaxyx(stdscr, altura, largura);
    int inicio_y = (LINES - altura) / 2;
    int inicio_x = (COLS - largura) / 2;

    // Janelas
    WINDOW *JanelaMenu = newwin(altura, largura, inicio_y, inicio_x);
    WINDOW *TituloPedidos = newwin(10, 40, 0, 0);
    WINDOW *JanelaOpcoes = newwin(50, 30, 0, 0);
    WINDOW *JanelaEscolhas;

    void desenha_menu(WINDOW *menu_win, int destacado, char *opcoes[], int n_opcoes); 

    int escolha = 0;
    int destacado = 0;
    int ch;
    const int NUM_OPCOES_PEDIDO = 4;
    const int NUM_OPCOES_ITEM_PEDIDO = 4;

    char *opcoesPedido[] = {
        "Cadastrar Pedido",
        "Listar Pedidos",
        "Detalhar Pedido"
        "Apagar Pedido"
        "Sair"
    };

    char *opcoesItemPedido[] = {
        "Cadastrar Item de Pedido",
        "Listar Item de Pedidos",
        "Apagar Item de Pedido"
        "Sair"
    };
    
    // Escreve em cada janela 
    box(TituloPedidos, 0, 0);
    mvwprintw(TituloPedidos, (altura - 5)/2, (largura - 2)/2, "Pedidos");
    
    while(1){
        wrefresh(JanelaEscolhas);
        desenha_menu(JanelaEscolhas, destacado, opcoesPedido, NUM_OPCOES_PEDIDO);
        ch = wgetch(JanelaEscolhas);
        switch(ch){
            case KEY_UP:
                if (destacado > 0) destacado--;
                break;
            case KEY_DOWN:
                if (destacado < NUM_OPCOES_PEDIDO) destacado++;
                break;
            case KEY_ENTER:
                escolha = destacado;
                break;
            case 'q':
            case 'Q':
                escolha = NUM_OPCOES_PEDIDO - 1;
                break;
        }
            if (ch == 10 || ch == 'q' || ch == 'Q') { 
                if (escolha == NUM_OPCOES_PEDIDO - 1){
                    break;  // Sair 
            } else { 
            clear(); 
            attron(COLOR_PAIR(1) | A_BOLD); 
            mvprintw(LINES/2, (COLS - 30)/2, "Você escolheu: %s", opcoesPedido[escolha]); 
            attroff(COLOR_PAIR(1) | A_BOLD); 
            mvprintw(LINES/2 + 2, (COLS - 30)/2, "Pressione qualquer tecla..."); 
            refresh(); 
            getch(); 
        }
    }

    // Atualiza tela principal e janelas 
    getch(); 

    // Exemplo de animação simples 
    for (int i = 0; i < 5; i++) { 
        wclear(JanelaOpcoes); 
        box(JanelaOpcoes, 0, 0); 
        mvwprintw(JanelaOpcoes, 0, 2, "[ Janela de Status ]"); 
        mvwprintw(JanelaOpcoes, 3, 2, "Atualizando: %d/5", i + 1); 
        wrefresh(JanelaOpcoes); 
        napms(500);  
    // Pausa de 500ms 
    } 
    mvwprintw(JanelaOpcoes, 5, 2, "Concluído! Pressione qualquer tecla..."); 
    wrefresh(JanelaOpcoes); 
    getch();

    // Libera memória das janelas 
    delwin(JanelaMenu); 
    delwin(JanelaEscolhas); 
    delwin(JanelaOpcoes); 
    endwin(); 
}

int main(){
    telaPedidos();
}
/* Biblioteca da interface */

#ifndef interface
#define interface

/* DEFINIÇÕES & CONSTANTES */

#define SCREEN_WIDTH   80   // Largura da tela
#define SCREEN_HEIGHT  25   // Altura da tela

#define UI_MARGIN      2    // Margem
#define UI_PADDING     1    // Padding

#define WINDOW_MAIN_W  60   // Tela principal largura
#define WINDOW_MAIN_H  20   // Tela principal altura

#define WINDOW_INFO_W  18   
#define WINDOW_INFO_H  10   

#define COLOR_PAIR_DEFAULT   1
#define COLOR_PAIR_WARNING   2
#define COLOR_PAIR_SUCCESS   3
#define COLOR_PAIR_ERROR     4
#define COLOR_PAIR_MENU      5
#define COLOR_PAIR_HIGHLIGHT 6

/* FUNÇÕES DAS INTERFACE (cada uma é uma tela e/ou elemento gráfico) */

void menuPrincipal();

void telaClientes();

void telaProdutos();

void telaPedidos();

#endif
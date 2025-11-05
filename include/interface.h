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

#define WINDOW_INFO_W  18   // Tela de informação largura
#define WINDOW_INFO_H  10   // Tela de informação altura

#define COLOR_PAIR_DEFAULT   1  // Cor padrão
#define COLOR_PAIR_WARNING   2  // Cor de aviso
#define COLOR_PAIR_SUCCESS   3  // Cor de sucesso
#define COLOR_PAIR_ERROR     4  // Cor de erro
#define COLOR_PAIR_MENU      5  // Cor do menu
#define COLOR_PAIR_HIGHLIGHT 6  // Cor de highlight

/* FUNÇÕES DAS INTERFACE (cada uma é uma tela e/ou elemento gráfico) */

void menuPrincipal();

void telaClientes();

void telaProdutos();

void telaPedidos();

#endif
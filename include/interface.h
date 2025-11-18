/* Biblioteca da interface */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "persistencia.h"
#include "estados.h"

/* DEFINIÇÕES & CONSTANTES */

#define SCREEN_MIN_WIDTH   30   // Largura da tela
#define SCREEN_MIN_HEIGHT  10   // Altura da tela

#define UI_INDENT      4    // Identação
#define UI_RECUO       3    // Récuo
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

#define BUFFER_LINHA_CARACTERES 512   // Tamanho máximo do buffer para quando o usuário for digitar algo na tela
#define BUFFER_LINHA_LOGO 100         // Tamanho do buffer da logo
#define QUANT_LINHAS_LOGO 10          // Quantidade de linhas da logo

/* FUNÇÕES DAS INTERFACE (cada uma é uma tela e/ou elemento gráfico) */

void inicializaInterface();
void finalizaInterface();

/* TODOS STATES DE INTERFACE */

void menuPrincipal();

/*
Estado telaClientes();

Estado telaProdutos();

Estado telaPedidos();
*/

#endif
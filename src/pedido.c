#include "../include/pedido.h"
#include "../include/interface.h"
#include "../include/cliente.h"
#include "../include/persistencia.h"
#include "../include/utils.h"
#include "../include/estados.h"

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

// ==========================================================================================
//          FUNÇÕES DE CASO DE USO DOS PEDIDOS
// ==========================================================================================

struct Pedido pedidoGlobal;
struct ItemPedido itemPedidoGlobal;

void cadastrarPedido(WINDOW *win, struct Pedido *P){
    werase(win);
    box(win, 0, 0);
    keypad(win, true);

    const char *dataAtual = obterDataAtual();
    char mensagem[BUFFER_LINHA_CARACTERES];
    
    mvwprintw(win, 1, 2, "Cadastrar Pedido");
    mvwprintw(win, 3, 2, "ID do pedido: ");
    wrefresh(win);

    do{
        P->id = input_int(win, 5, 18);
        if(!validarIdPedidoObtido(P->id, mensagem)){
            mvwprintw(win, 5, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedidoObtido(P->id, mensagem));

    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 7, 2, "ID do cliente: ");
    wrefresh(win);

    do{
        P->clienteId = input_int(win, 9, 18);
        if(!validarIdClienteObtido(P->clienteId, mensagem)){
            mvwprintw(win, 9, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedidoObtido(P->id, mensagem));

    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 11, 2, "Data (%s): ", dataAtual);
    wrefresh(win);

    input_string(win, 13, 18, P->data, sizeof(P->data));
    while(!validarData(P->data, mensagem)){
        mvwprintw(win, 13, 2, "%s", mensagem);
        wrefresh(win);
        input_string(win, 13, 18, P->data, sizeof(P->data));
    }

    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 15, 2, "Total: ");
    wrefresh(win);

    do{
        P->total = input_int(win, 17, 18);
        if(!validarTotalObtido(P->total, mensagem)){
            mvwprintw(win, 9, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarTotalObtido(P->total, mensagem));

    if(guardarPedido(P, mensagem)){
        mvwprintw(win, 6, 2, "Pedido salvo com sucesso!");
    } else {
        mvwprintw(win, 6, 2, "%s", mensagem);
    }
    mvwprintw(win, 7, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

void listarPedidos(WINDOW *win){
    werase(win);
    box(win, 0, 0);

    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arq){
        mvwprintw(win, 1, 2, "Erro ao abrir arquivo!");
        wrefresh(win);
        wgetch(win);
        return;
    }

    mvwprintw(win, 1, 2, "Lista de Pedidos: ");

    int linhaY = 3;
    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct Pedido P;

        if(sscanf(linha, "%d,%d,%11[^,],%lf", &P.id, &P.clienteId, P.data, &P.total) == 4){
            mvwprintw(win, linhaY++, 2, "ID:%d   Cliente:%d   Data:%s   Total:%.2f", P.id, P.clienteId, P.data, P.total);
        }
    }
    
    fclose(arq);

    mvwprintw(win, linhaY+1, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

int analisarPedido(int idDoPedido, int idDoCliente, char *mensagem){ // TAMBEM ARRUMAR ISSO AQUI!
    FILE *arqPedido = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arqPedido){
        return -1; // -1 -> ERRO NO ARQUIVO
        mensagem = "Erro ao abrir arquivo!";
    }

    FILE *arqCliente = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arqCliente){
        return -1; // -1 -> ERRO NO ARQUIVO
        mensagem = "Erro ao abrir arquivo!";
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arqPedido)){
        struct Pedido P;

        // Verifica se existe algum pedido com mesmo ID
        if((sscanf(linha, "%d,%d,%10[^,],%lf", &P.id, &P.clienteId, P.data, &P.total) == 4)){
            if(P.id == idDoPedido){
                fclose(arqPedido);
                return 1; // 1 -> existe pedido com esse ID
            }
        }
    }
    
    fclose(arqPedido);
    fclose(arqCliente);
    return 0; // 0 -> não existe pedido com esse ID (SUCESSO)
}

void removerPedido(WINDOW *win){
    werase(win);
    box(win, 0, 0);
    keypad(win, TRUE);

    mvwprintw(win, 1, 2, "Remover Pedido");
    mvwprintw(win, 3, 2, "Digite o ID do pedido a ser removido: ");
    wrefresh(win);

    char mensagem[BUFFER_LINHA_CARACTERES];
    int idRemover;

    do{
        idRemover = input_int(win, 3, 47);
        if(!validarIdPedidoObtido(idRemover, mensagem)){
            mvwprintw(win, 5, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedidoObtido(idRemover, mensagem));

    if(apagarPedido(idRemover, mensagem)){
         mvwprintw(win, 7, 2, "Item de pedido removido com sucesso!");
    } else {
        mvwprintw(win, 7, 2, "%s", mensagem);
    }

    mvwprintw(win, 8, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

void detalharPedido(WINDOW *win){
    werase(win);
    box(win, 0, 0);
    keypad(win, TRUE);

    mvwprintw(win, 1, 2, "Detalhar Pedido");
    mvwprintw(win, 3, 2, "Digite o ID do pedido para ver detalhes: ");
    wrefresh(win);

    char mensagem[BUFFER_LINHA_CARACTERES];
    int idDetalhar;

    do{
        idDetalhar = input_int(win, 3, 47);
        if(!validarIdPedidoObtido(idDetalhar, mensagem)){
            mvwprintw(win, 5, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedidoObtido(idDetalhar, mensagem));

    // Lógica para detalhar o pedido (exibir informações detalhadas)
    // IMPLEMENTAR

    mvwprintw(win, 7, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

// =========================================================================================
//         FUNÇÕES DE CASO DE USO DOS ITENS DE PEDIDOS
// ========================================================================================

void cadastrarItemPedido(WINDOW *win, struct ItemPedido *IP){
    // IMPLEMENTAR FUNÇÕES DE VALIDAÇÃO DE CADA CAMPO
    werase(win);
    box(win, 0, 0);
    keypad(win, true);

    char mensagem[BUFFER_LINHA_CARACTERES];
    
    mvwprintw(win, 1, 2, "Cadastrar Item de Pedido");
    mvwprintw(win, 3, 2, "ID do pedido (deve existir): ");
    wrefresh(win);

    do{
        IP->pedidoId = input_int(win, 3, 18);
        if(!validarIdItemPedidoObtido(IP->pedidoId, mensagem)){
            mvwprintw(win, 5, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedidoObtido(IP->pedidoId, mensagem));

    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 7, 2, "ID do produto: ");
    wrefresh(win);

    do{
        IP->produtoId = input_int(win, 7, 18);
        if(!validarIdPedidoObtido(IP->produtoId, mensagem)){
            mvwprintw(win, 9, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedidoObtido(IP->produtoId, mensagem));

    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 11, 2, "Quantidade: ");
    wrefresh(win);

    do{
        IP->quantidade = input_int(win, 9, 18);
        if(!validarQuantidadeObtida(IP->quantidade, mensagem)){
            mvwprintw(win, 13, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarQuantidadeObtida(IP->quantidade, mensagem));

    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 15, 2, "Subtotal: ");
    wrefresh(win);

    do{
        IP->subtotal = input_double(win, 17, 18);
        if(!validarSubtotalObtido(IP->subtotal, mensagem)){
            mvwprintw(win, 17, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarSubtotalObtido(IP->quantidade, mensagem));

    if(guardarItemPedido(IP, mensagem)){
        mvwprintw(win, 19, 2, "Item salvo com sucesso!");
    } else {
        mvwprintw(win, 19, 2, "%s", mensagem);
    }
    mvwprintw(win, 20, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

void listarItemPedidos(WINDOW *win){
    werase(win);
    box(win, 0, 0);

    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        mvwprintw(win, 1, 2, "Erro ao abrir arquivo!");
        wrefresh(win);
        wgetch(win);
        return;
    }

    mvwprintw(win, 1, 2, "Lista de Itens de Pedidos: ");

    int linhaY = 3;
    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct ItemPedido IP;

        if(sscanf(linha, "%d,%d,%d,%lf", &IP.pedidoId, &IP.produtoId, &IP.quantidade, &IP.subtotal) == 4){
            mvwprintw(win, linhaY++, 2, "PedidoID:%d   ProdutoID:%d   Quantidade:%d   Subtotal:%.2f", IP.pedidoId, IP.produtoId, IP.quantidade, IP.subtotal);
        }
    }
    
    fclose(arq);

    mvwprintw(win, linhaY+1, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

int analisarItemPedido(int idDoItemPedido, char *mensagem){ // ARRUMAR ISSO AQUI
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        return -1; // ERRO AO ABRIR ARQUIVO
        mensagem = "Erro ao abrir arquivo!";
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq) != NULL){
        struct ItemPedido IP;

        if(sscanf(linha, "%d,%d,%d,%lf", &IP.pedidoId, &IP.produtoId, &IP.quantidade, &IP.subtotal) == 4){
            if(IP.pedidoId == idDoItemPedido){
                mensagem = "Id do Item de Pedido já existe!";
                return 1;
            }
        }
    }
    
    fclose(arq);
    return 0; // 0 = sucesso
}

void removerItemPedido(WINDOW *win, struct ItemPedido *IP){
    werase(win);
    box(win, 0, 0);
    keypad(win, TRUE);

    mvwprintw(win, 1, 2, "Remover Item de Pedido");
    mvwprintw(win, 3, 2, "Digite o ID do item de pedido a ser removido: ");
    wrefresh(win);

    char mensagem[BUFFER_LINHA_CARACTERES];
    int idRemover;

    do{
        idRemover = input_int(win, 3, 47);
        if(!validarIdItemPedidoObtido(idRemover, mensagem)){
            mvwprintw(win, 5, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdItemPedidoObtido(idRemover, mensagem));

    if(apagarPedido(idRemover, mensagem)){
         mvwprintw(win, 7, 2, "Pedido removido com sucesso!");
    } else {
        mvwprintw(win, 7, 2, "%s", mensagem);
    }

    mvwprintw(win, 8, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

//  =========================================================
//                  Tela Pedidos INTERFACE
//  =========================================================

static void desenhaOpcoesVertical(WINDOW *win, char *labels[], int total, int selecionado, int temFoco){
    if(!win || total <= 0) return;

    int altura, largura;
    getmaxyx(win, altura, largura);

    // Layout dos botões
    int espacamento = 2;
    int altura_total = espacamento * (total - 1);

    // Horizontal
    int inicio_x = altura - 6;
    if(inicio_x < 1) inicio_x = 1;

    // Vertical
    int inicio_y = (altura - altura_total) / 2;
    if(inicio_y < 1) inicio_y = 1;

    int x = inicio_x;
    int y = inicio_y;

    for(int i=0; i<total; i++){
        char labelFormatado[BUFFER_LINHA_CARACTERES];
        snprintf(labelFormatado, sizeof(labelFormatado), "> %s", labels[i]);

        if(i == selecionado){
            wattron(win, A_REVERSE | A_BOLD);
            mvwprintw(win, y, x, "%s", labelFormatado);
            wattroff(win, A_REVERSE | A_BOLD);
        } else {
            mvwprintw(win, y, x, "%s", labelFormatado);
        }

        y += espacamento;
    }

    if (temFoco) wattron(win, A_BOLD);
    box(win, 0, 0);
    if (temFoco) wattroff(win, A_BOLD);

    wrefresh(win);
}

void menuPedidos(){
    // trocar dps para o struct itemmenu
    /*
    ItemMenu opcoesPedido = [
        {"Cadastrar Pedido", cadastrarPedido()},
        {"Listar Pedidos", listarPedidos()},
        {"Detalhar Pedido", detalharPedido()},
        {"Apagar Pedido", apagarPedido()}
    ];
    */
    char *opcoesPedido[] = {
        "Cadastrar Pedido",
        "Listar Pedidos",
        "Detalhar Pedido",
        "Apagar Pedido"
    };
    const int totalBotoesPedido = 4;

    char *opcoesItemPedido[] = {
        "Cadastrar Item de Pedido",
        "Listar Item de Pedidos",
        "Apagar Item de Pedido",
        "Voltar"
    };
    const int totalBotoesItemPedido = 4;

    int select_left = 0;
    int select_right = 0;
    int ativo = 0;
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
        int altura_janela = altura - UI_MARGIN * 2;
        int largura_janela = largura - UI_MARGIN * 2;

        int largura_esquerda = (largura_janela - 3) / 2;
        int largura_direita = (largura_janela - largura_esquerda - 3);

        int topo = UI_MARGIN;
        int esquerda_x = UI_MARGIN;
        int direita_x = UI_MARGIN + esquerda_x + 3;

        // Impede que o terminal fique muito pequeno, por padrão as dimensões minimas são (20x60)
        if(ehTerminalPequeno(stdscr, altura, largura)) continue;

        WINDOW *janelaPrincipal = newwin(altura_janela, largura_janela, topo, topo);
        WINDOW *janelaEsquerda = newwin(altura_janela - UI_MARGIN, largura_esquerda, topo + UI_PADDING, esquerda_x + UI_PADDING);
        WINDOW *janelaDireita = newwin(altura_janela - UI_MARGIN, largura_direita, topo + UI_PADDING, direita_x + UI_PADDING);
        box(janelaPrincipal, 0, 0);
        mvwprintw(janelaPrincipal, 0, 2, " PEDIDOS ");
        mvwprintw(janelaPrincipal, 0, largura_janela, " ITENS DE PEDIDOS ");

        // Desenho da UI
        desenhaOpcoesVertical(janelaEsquerda, opcoesPedido, totalBotoesPedido, select_left, ativo == 0);
        desenhaOpcoesVertical(janelaDireita, opcoesItemPedido, totalBotoesItemPedido, select_right, ativo == 1);

        mvwprintw(janelaPrincipal, altura_janela - UI_MARGIN, largura_janela - (int)strlen(texto_do_tamanho) - UI_MARGIN, "%s", texto_do_tamanho);

        // A cada caracter obtido, a janela é atualizada
        wrefresh(janelaPrincipal);

        ch = getch();

        // Controle dos botões, ENTER confirma e sai do loop

        if(ch == 'q' || ch == 'Q'){
            delwin(janelaPrincipal);
            delwin(janelaDireita);
            delwin(janelaEsquerda);
            break;
        } else if(ch == KEY_LEFT) ativo = 0;
        else if(ch == KEY_RIGHT) ativo = 1;
        else if (ch == KEY_UP){
            if(!ativo && select_left > 0) select_left--;
            if(ativo && select_right > 0) select_right--;
        } else if(ch == KEY_DOWN){
            if(!ativo && select_left < totalBotoesPedido - 1) select_left++;
            if(ativo && select_right < totalBotoesItemPedido - 1) select_right++;
        } else if(ch == 10 || ch == '\n'){
            // PEDIDO
            if(!ativo){
                WINDOW *popup = newwin(15, 60, (altura - 15)/2, (largura-60)/2);
                keypad(popup, true);
                
                switch(select_left){
                    case 0:
                        estado_atual = ST_PEDIDO_CADASTRO;
                        cadastrarPedido(popup, &pedidoGlobal);
                        break;

                    case 1:
                        estado_atual = ST_PEDIDO_LISTA;
                        listarPedidos(popup);
                        break;

                    case 2:
                        estado_atual = ST_PEDIDO_DETALHES;
                        detalharPedido(popup);
                        break;

                    case 3:
                        estado_atual = ST_PEDIDO_APAGAR;
                        removerPedido(popup);
                        break;
                }
                delwin(popup);
            } else { // ITEM PEDIDO
                WINDOW *popup = newwin(15, 60, (altura - 15)/2, (largura-60)/2);
                keypad(popup, true);
                
                switch(select_right){
                    case 0:
                        estado_atual = ST_ITEM_PEDIDO_CADASTRO;
                        cadastrarItemPedido(popup, &itemPedidoGlobal);
                        break;

                    case 1:
                        estado_atual = ST_ITEM_PEDIDO_LISTA;
                        listarItemPedidos(popup);
                        break;

                    case 2:
                        estado_atual = ST_ITEM_PEDIDO_APAGAR;
                        removerItemPedido(popup, &itemPedidoGlobal);
                        break;

                    case 3: 
                        estado_atual = ST_PEDIDO_PRINCIPAL;    
                        break; // Voltar
                }
                delwin(popup);
            }
        }
       
        delwin(janelaPrincipal);
        delwin(janelaDireita);
        delwin(janelaEsquerda);
    }
}
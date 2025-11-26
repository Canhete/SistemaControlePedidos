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
//         UTILITÁRIOS
// ==========================================================================================

static void desenhaBotoesHorizontal(WINDOW *win, char *labels[], int total, int selecionado){
    if(!win || total <= 0) return; // Não renderiza se não tiver botões ou janela

    int altura, largura;
    getmaxyx(win, altura, largura);

    // Layout dos botões
    int espacamento = 6;

    int largura_total = 0;
    for(int i=0; i<total; i++){
        int len_botao = (int)strlen(labels[i]) + 4;
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
        snprintf(labelFormatado, sizeof(labelFormatado), "[ %s ]", labels[i]);

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

static int criaPopupMensagem(WINDOW *win, char *titulo, char *mensagem){
    int altura, largura;
    getmaxyx(stdscr, altura, largura);

    WINDOW *popup = newwin(9, largura/2, (altura - 9)/2, (largura - (largura/2))/2);
    box(popup, 0, 0);

    keypad(popup, true);
    noecho();
    curs_set(0);

    char *labels[] = {
        "Não",
        "Sim"
    };
    int total = 2;
    int selecionado = 0;

    while(1){
        werase(popup);
        box(popup, 0, 0);

        mvwprintw(popup, 2, 2, "%s", titulo);
        mvwprintw(popup, 3, 2, "%s", mensagem);
        desenhaBotoesHorizontal(popup, labels, total, selecionado);
        wrefresh(popup);

        int ch;
        ch = wgetch(popup);

        if(ch == KEY_LEFT) selecionado = 0;
        else if(ch == KEY_RIGHT) selecionado = 1;
        else if(ch == 10 || ch == KEY_ENTER) break;
    }
    werase(popup);
    wrefresh(popup);
    delwin(popup);

    keypad(win, false);
    echo();
    curs_set(1);

    return selecionado;
}

// ==========================================================================================
//          FUNÇÕES DE CASO DE USO DOS PEDIDOS
// ==========================================================================================

struct Pedido;
struct ItemPedido;

struct Pedido pedidoGlobal;
struct ItemPedido itemPedidoGlobal;

// A função principal para cadastro, a ideia e usar structs globais, por isso existem esses parâmetros
void cadastrarPedido(WINDOW *win, struct Pedido *P, struct ItemPedido *IP){
    keypad(win, true);
    echo();
    curs_set(1);
    werase(win);
    box(win, 0, 0);

    const char *dataAtual = obterDataAtual();
    char mensagem[BUFFER_LINHA_CARACTERES];
    
    mvwprintw(win, 0, 4, "[ Cadastrar Pedido ]");

    if(!criarArquivoPedido(mensagem)){
        mvwprintw(win, 2, 2, "%s", mensagem);
        estado_atual = ST_MENU_PRINCIPAL;
        return;
    }

    mvwprintw(win, 3, 2, "%100s", "");
    mvwprintw(win, 3, 2, "ID do pedido: ");
    wrefresh(win);

    // A função de analisarPedido é equivalente a de validarIdPedido
    // CADASTRO DO ID DO PEDIDO
    do{
        P->id = input_int(win, 3, 16);
        if(!validarIdPedido(P->id, mensagem)){
            mvwprintw(win, 5, 2, "%s", mensagem);
            mvwprintw(win, 3, 16, "%100s", "");
            wrefresh(win);
        }
        mvwprintw(win, 5, 2, "%100s", "");
    } while(!validarIdPedido(P->id, mensagem));

    box(win, 0, 0);
    mvwprintw(win, 5, 2, "Código do cliente: ");
    wrefresh(win);

    // A função de analisarCliente é equivalente a função validarIdCliente
    // CADASTRO DO ID DO CLIENTE
    do{
        P->clienteId = input_int(win, 5, 22);
        if(!validarIdCliente(P->clienteId, mensagem)){
            mvwprintw(win, 7, 2, "%s", mensagem);
            mvwprintw(win, 5, 22, "%100s", "");
            wrefresh(win);
        }
        mvwprintw(win, 7, 2, "%100s", "");
    } while(!validarIdCliente(P->clienteId, mensagem));

    box(win, 0, 0);
    mvwprintw(win, 7, 2, "Data do pedido (dd/mm/aaaa) [%s]: ", dataAtual);
    wrefresh(win);

    // Funções extras de validação, no caso essa valida se digitou uma data válida
    // CADASTRO DA DATA DE PEDIDO

    input_string(win, 7, 44, P->data, sizeof(P->data));
    while(!validarData(P->data, mensagem)){
        mvwprintw(win, 9, 2, "%200s", "");
        mvwprintw(win, 9, 2, "%s", mensagem);
        mvwprintw(win, 7, 44, "%10s", "");
        wrefresh(win);

        input_string(win, 7, 44, P->data, sizeof(P->data));
    }

    // Cadastro de Item de pedido; Pelo menos 1 item de pedido deve estar cadastrado
    cadastrarItemPedido(win, P, IP, 1);
    box(win, 0, 0);
    wrefresh(win);

    // TOTAL (SOMA DO SUBTOTAIS DE TODOS OS ITENS)
    // Calculo prévio do total
    double totalCalculado = calcularTotal(P, mensagem);

    if(totalCalculado >= 0){
        P->total = totalCalculado;
        mvwprintw(win, 11, 2, "Total: %.2lf", totalCalculado);
    } else {
        mvwprintw(win, 11, 2, "%s", mensagem);
    }

    if(guardarPedido(P, mensagem)){
        mvwprintw(win, 11, 2, "Pedido salvo com sucesso!");
    } else {
        mvwprintw(win, 11, 2, "%s", mensagem);
    }

    mvwprintw(win, 13, 2, "Pressione qualquer tecla...");
    box(win, 0, 0);
    wrefresh(win);

    wgetch(win);
    
    noecho();
    curs_set(0);
    estado_atual = ST_PEDIDO_CADASTRO;
}

static void desenhaSetinhas(WINDOW *win, char *labels[], int total, int selecionado){
    if(!win || total <= 0) return; // Não renderiza se não tiver botões ou janela

    int altura, largura;
    getmaxyx(win, altura, largura);

    // Layout dos botões
    int espacamento = 8;

    int largura_total = 0;
    for(int i=0; i<total; i++){
        int len_botao = (int)strlen(labels[i]) + 3;
        largura_total += len_botao;
    }
    largura_total += espacamento * (total - 1);

    // Horizontal
    int inicio_x = (largura - largura_total) / 2;
    if(inicio_x < 1) inicio_x = 1;

    // Vertical
    int inicio_y = altura - 2;
    if(inicio_y < 1) inicio_y = 1;

    int x = inicio_x;
    int y = inicio_y;

    for(int i=0; i<total; i++){
        if(i == selecionado){
            wattron(win, A_REVERSE | A_BOLD);
            mvwprintw(win, y, x, "%s", labels[i]);
            wattroff(win, A_REVERSE | A_BOLD);
        } else {
            mvwprintw(win, y, x, "%s", labels[i]);
        }

        x += (int)strlen(labels[i]) + espacamento;
    }
}

void listarPedidos(WINDOW *win){
    int altura, largura;
    getmaxyx(win, altura, largura);

    keypad(win, true);
    echo();
    curs_set(0);
    werase(win);
    box(win, 0, 0);

    FILE *arq = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arq){
        mvwprintw(win, 2, 2, "Erro ao abrir arquivo!");
        mvwprintw(win, 3, 2, "Pressione qualquer tecla...");
        wrefresh(win);
        wgetch(win);
        return;
    }

    char *setinhas[] = {
        "<-- Página anterior",
        "Voltar (q)",
        "Próxima página -->"
    };
    const int quantidade = 3;
    int escolhido = 0;

    struct Pedido *vetor = NULL;
    int count = 0;
    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct Pedido P;

        if(sscanf(linha, "%d,%d,%10[^,],%lf", &P.id, &P.clienteId, P.data, &P.total) == 4){
            struct Pedido *temp = realloc(vetor, (count + 1) * sizeof(*vetor));
            if(!temp){
                free(vetor);
                fclose(arq);
                mvwprintw(win, 2, 2, "Erro de memória ao carregar pedidos.");
                mvwprintw(win, 3, 2, "Pressione qualquer tecla...");
                wrefresh(win);
                wgetch(win);
                return;
            }

            vetor = temp;
            vetor[count++] = P;
        }
    }
    fclose(arq);
    
    if(count == 0){
        mvwprintw(win, 2, 2, "Nenhum pedido encontrado.");
        mvwprintw(win, 4, 2, "Pressione qualquer tecla...");
        wrefresh(win);
        wgetch(win);
        free(vetor);
        return;
    }

    const int cabecalho_linhas = 4;
    const int rodape_linhas = 3;

    int linhas_por_pagina = altura - cabecalho_linhas - rodape_linhas;
    if(linhas_por_pagina < 1) linhas_por_pagina = 1;

    int paginas_total = (count + linhas_por_pagina - 1) / linhas_por_pagina;
    int pagina_atual = 0;

    int ch;

    while(1){
        int linhaY = 2; 

        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 3, "[ Lista de Pedidos ]");

        // Cabeçalho da tabela
        char header[128];
        snprintf(header, sizeof(header), "| %-15s | %-15s | %-15s | %-15s |", "ID", "Cliente ID", "Data", "Total");
        int header_len = (int)strlen(header); // Tamanho do cabeçalho
        int header_x = (largura - header_len) / 2; // Posição X para centralizar
        mvwprintw(win, 2, header_x, "%s", header); // Centralizado

        // Índices dos itens a exibir nessa página
        int index_inicial = pagina_atual * linhas_por_pagina;
        int index_final = index_inicial + linhas_por_pagina;
        if(index_final > count) index_final = count;

        // Desenha as linhas da página
        for(int i = index_inicial; i < index_final; ++i){
            int row = linhaY + (i - index_inicial) + 1;
            mvwprintw(win, row, header_x, "| %-15d | %-15d | %-15s | %-15.2f |", vetor[i].id, vetor[i].clienteId, vetor[i].data, vetor[i].total);
        }

        // Rodapé com paginação e instruções
        mvwprintw(win, altura - 3, 2, "Página %d / %d", pagina_atual + 1, paginas_total);
        desenhaSetinhas(win, setinhas, quantidade, escolhido);
        wrefresh(win);

        ch = wgetch(win);

        if(ch == KEY_LEFT && pagina_atual > 0){
            escolhido = 0;
            pagina_atual--;
        }
        else if(ch == KEY_RIGHT && pagina_atual < paginas_total - 1){
            escolhido = 2;
            pagina_atual++;
        }
        else if(ch == 'q' || ch == 'Q' || ch == 27){ // ESC VOLTA
            escolhido = 1;
            break;
        }
        else if(ch == KEY_NPAGE){ // PageDown
            pagina_atual = (pagina_atual < paginas_total - 1) ? pagina_atual + 1 : pagina_atual;
        } else if(ch == KEY_PPAGE){ // PageUp
            pagina_atual = (pagina_atual > 0) ? pagina_atual - 1 : pagina_atual;
        }
    }

    free(vetor);
}

void listarItemPedidosDoPedido(WINDOW *win, int idPedido){
    int altura, largura;
    getmaxyx(win, altura, largura);

    keypad(win, true);
    noecho();
    curs_set(0);
    werase(win);
    box(win, 0, 0);

    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        mvwprintw(win, 2, 2, "Erro ao abrir arquivo de item de pedidos!");
        mvwprintw(win, 3, 2, "Pressione qualquer tecla...");
        wrefresh(win);
        wgetch(win);
        return;
    }

    char *setinhas[] = {
        "<-- Página anterior",
        "Voltar (q)",
        "Próxima página -->"
    };
    const int quantidade = 3;
    int escolhido = 0;

    struct ItemPedido *vetor = NULL;
    int count = 0;
    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct ItemPedido IP;

        if(sscanf(linha, "%d,%d,%d,%lf", &IP.pedidoId, &IP.produtoId, &IP.quantidade, &IP.subtotal) == 4 && IP.pedidoId == idPedido){ // Pega apeanas a informação do pedido solicitado
            struct ItemPedido *temp = realloc(vetor, (count + 1) * sizeof(*vetor));
            if(!temp){
                free(vetor);
                fclose(arq);
                mvwprintw(win, 2, 2, "Erro de memória ao carregar itens de pedidos.");
                mvwprintw(win, 3, 2, "Pressione qualquer tecla...");
                wrefresh(win);
                wgetch(win);
                return;
            }

            vetor = temp;
            vetor[count++] = IP;
        }
    }
    fclose(arq);
    
    // Obtenção das informações detalhadas
    FILE *arqPed = fopen(DIRETORIO_ARQUIVO_PEDIDO, "r");
    if(!arqPed){
        mvwprintw(win, 5, 2, "Erro ao abrir arquivo de pedidos!");
        return;
    }

    char linhaId[BUFFER_ARQUIVO_LINHA];
    char linhaCliente[BUFFER_ARQUIVO_LINHA];
    char linhaData[BUFFER_ARQUIVO_LINHA];
    char linhaTotal[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arqPed)){
        struct Pedido P;
        
        if(sscanf(linha, "%d,%d,%10[^,],%lf", &P.id, &P.clienteId, P.data, &P.total) == 4 && P.id == idPedido){
            sprintf(linhaId, "Pedido ID: %d", P.id);
            sprintf(linhaCliente, "Cliente ID: %d", P.clienteId);
            sprintf(linhaData, "Data: %s", P.data);
            sprintf(linhaTotal, "Total: %.2lf", P.total);
            break;
        }
    }
    fclose(arqPed);

    if(count == 0){ // Não é esperado chegar nessa tela, pois todo pedido que existe deve ter pelo menos 1 item
        mvwprintw(win, 2, 2, "Nenhum item de pedido encontrado.");
        mvwprintw(win, 4, 2, "Pressione qualquer tecla...");
        wrefresh(win);
        wgetch(win);
        free(vetor);
        return;
    }

    const int cabecalho_linhas = 5;
    const int rodape_linhas = 3;

    int linhas_por_pagina = altura - cabecalho_linhas - rodape_linhas;
    if(linhas_por_pagina < 1) linhas_por_pagina = 1;

    int paginas_total = (count + linhas_por_pagina - 1) / linhas_por_pagina;
    int pagina_atual = 0;

    int ch;

    while(1){
        int linhaY = 2; 

        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 3, "[ Lista de Itens de Pedidos ]");

        // Cabeçalho da tabela
        char header[128];
        snprintf(header, sizeof(header), "| %-15s | %-15s | %-15s | %-15s |", "Pedido ID", "Produto ID", "Quantidade", "Subtotal");
        int header_len = (int)strlen(header); // Tamanho do cabeçalho
        int header_x = (largura - header_len) / 2; // Posição X para centralizar
        mvwprintw(win, 7, header_x, "%s", header); // Centralizado

        // Informações do pedido
        mvwprintw(win, 2, 4, "%s", linhaId);
        mvwprintw(win, 3, 4, "%s", linhaCliente);
        mvwprintw(win, 4, 4, "%s", linhaData);
        mvwprintw(win, 5, 4, "%s", linhaTotal);

        // Índices dos itens a exibir nessa página
        int index_inicial = pagina_atual * linhas_por_pagina;
        int index_final = index_inicial + linhas_por_pagina;
        if(index_final > count) index_final = count;

        // Desenha as linhas da página
        for(int i = index_inicial; i < index_final; ++i){
            int row = linhaY + (i - index_inicial) + 7;
            mvwprintw(win, row, header_x, "| %-15d | %-15d | %-15d | %15.2f |", vetor[i].pedidoId, vetor[i].produtoId, vetor[i].quantidade, vetor[i].subtotal);
        }

        // Rodapé com paginação e instruções
        mvwprintw(win, altura - 3, 2, "Página %d / %d", pagina_atual + 1, paginas_total);
        desenhaSetinhas(win, setinhas, quantidade, escolhido);
        wrefresh(win);

        ch = wgetch(win);

        if(ch == KEY_LEFT && pagina_atual > 0){
            escolhido = 0;
            pagina_atual--;
        }
        else if(ch == KEY_RIGHT && pagina_atual < paginas_total - 1){
            escolhido = 2;
            pagina_atual++;
        }
        else if(ch == 'q' || ch == 'Q' || ch == 27){ // ESC VOLTA
            escolhido = 1;
            break;
        }
        else if(ch == KEY_NPAGE){ // PageDown
            pagina_atual = (pagina_atual < paginas_total - 1) ? pagina_atual + 1 : pagina_atual;
        } else if(ch == KEY_PPAGE){ // PageUp
            pagina_atual = (pagina_atual > 0) ? pagina_atual - 1 : pagina_atual;
        }
    }

    free(vetor);
}

void removerPedido(WINDOW *win){
    keypad(win, true);
    echo();
    curs_set(1);
    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 1, 2, "Remover Pedido");
    mvwprintw(win, 3, 2, "Digite o ID do pedido a ser removido: ");
    wrefresh(win);

    char mensagem[BUFFER_LINHA_CARACTERES];
    int idRemover;

    do{
        idRemover = input_int(win, 3, 40);
        if(!validarIdPedido(idRemover, mensagem)){
            mvwprintw(win, 5, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedido(idRemover, mensagem));

    // Confirmação de remoção
    if(!criaPopupMensagem(win, "Tem certeza que deseja apagar este pedido?", "Esta ação não pode ser desfeita!")) return;

    if(apagarPedido(idRemover, mensagem)){
        mvwprintw(win, 7, 2, "Item de pedido removido com sucesso!");
    } else {
        mvwprintw(win, 7, 2, "%s", mensagem);
    }

    mvwprintw(win, 8, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

void removerTodosPedidos(WINDOW *win){
    keypad(win, true);
    echo();
    curs_set(1);
    werase(win);
    box(win, 0, 0);

    char mensagem[BUFFER_LINHA_CARACTERES];

    if(!criaPopupMensagem(win, "Tem certeza que deseja apagar TODOS os pedidos?", "Esta ação não pode ser desfeita!")) return;

    if(apagarTodosPedidosExistentes(mensagem)){
        mvwprintw(win, 2, 2, "Todos os pedidos foram removidos com sucesso!");
    } else {
        mvwprintw(win, 2, 2, "%s", mensagem);
    }

    mvwprintw(win, 4, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

void consultarPedido(WINDOW *win){
    keypad(win, true);
    echo();
    curs_set(1);
    werase(win);
    box(win, 0, 0);

    char mensagem[BUFFER_LINHA_CARACTERES];
    int idDetalhar;

    mvwprintw(win, 0, 2, "[ Consultar Pedido ]");
    mvwprintw(win, 2, 2, "Consulta informações detalhadas de um pedido específico.");
    mvwprintw(win, 3, 2, "Digite o ID do pedido a ser consultado: ");
    wrefresh(win);

    do{
        idDetalhar = input_int(win, 3, 42);
        if(!validarIdPedido(idDetalhar, mensagem)){
            mvwprintw(win, 5, 2, "%100s", "");
            mvwprintw(win, 5, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedido(idDetalhar, mensagem));

    listarItemPedidosDoPedido(win, idDetalhar);
}

// =========================================================================================
//         FUNÇÕES DE CASO DE USO DOS ITENS DE PEDIDOS
// ========================================================================================

void cadastrarItemPedido(WINDOW *win, struct Pedido *P, struct ItemPedido *IP, int primeiraVez){
    char mensagem[BUFFER_LINHA_CARACTERES];
    int count = 1;

    while(1){
        keypad(win, true);
        echo();
        curs_set(1);
        werase(win);
        box(win, 0, 0);
        
        mvwprintw(win, 0, 2, "[ Cadastrar Item de Pedido ]");

        if(!criarArquivoItemPedido(mensagem)){
            mvwprintw(win, 2, 2, "%s", mensagem);
            estado_atual = ST_MENU_PRINCIPAL;
            return;
        }

        if(!primeiraVez){
            mvwprintw(win, 3, 2, "ID do pedido: ");
            wrefresh(win);
            do{
                P->id = input_int(win, 3, 17);
                if(!validarIdPedido(P->id, mensagem)){
                    mvwprintw(win, 5, 2, "%100s", "");
                    mvwprintw(win, 5, 2, "%s", mensagem);
                    wrefresh(win);
                }
            } while(!validarIdProduto(P->id, mensagem));
        }

        werase(win);
        box(win, 0, 0);

        if (!primeiraVez) {
            mvwprintw(win, 2, 2, "Cadastro de mais um item para o pedido ID %d", P->id);
        } else {
            mvwprintw(win, 2, 2, "Cadastro do primeiro item para o pedido ID %d", P->id);
        }

        mvwprintw(win, 3, 2, "ITEM DE PEDIDO %d", count);

        // Definindo ID do Item de pedido = ID do pedido
        IP->pedidoId = P->id;

        mvwprintw(win, 5, 2, "ID do produto: ");
        wrefresh(win);

        // A função validarIdProduto é equivalente à função analisaProduto
        // OBTENÇÃO DO ID DO PRODUTO
        do{
            IP->produtoId = input_int(win, 5, 17);
            if(!validarIdProduto(IP->produtoId, mensagem)){
                mvwprintw(win, 7, 2, "%100s", "");
                mvwprintw(win, 7, 2, "%s", mensagem);
                wrefresh(win);
            }
            mvwprintw(win, 5, 17, "%100s", "");
        } while(!validarIdProduto(IP->produtoId, mensagem));

        mvwprintw(win, 7, 2, "Quantidade: ");
        wrefresh(win);
        
        // OBTENÇÃO DA QUANTIDADE
        do{
            IP->quantidade = input_int(win, 7, 14);
            if(!validarQuantidade(IP->quantidade, mensagem)){
                mvwprintw(win, 9, 2, "%100s", "");
                mvwprintw(win, 9, 2, "%s", mensagem);
                wrefresh(win);
            }
            mvwprintw(win, 7, 14, "%100s", "");
        } while(!validarQuantidade(IP->quantidade, mensagem));

        double subtotalCalculado = calcularSubtotal(IP, mensagem);
        // Cálculo prévio do subtotal
        if(subtotalCalculado > 0){
            IP->subtotal = subtotalCalculado;
            mvwprintw(win, 9, 2, "Subtotal: %.2lf", subtotalCalculado);
        } else {
            mvwprintw(win, 9, 2, "%s", mensagem);
        }
        wrefresh(win);

        if(guardarItemPedido(IP, mensagem)){
            mvwprintw(win, 11, 2, "Item salvo com sucesso!");
        } else {
            mvwprintw(win, 11, 2, "%s", mensagem);
        }

        wrefresh(win);

        // Janela perguntando se quer cadastrar mais itens
        if(criaPopupMensagem(win, "Deseja cadastrar outro item para este pedido?", "") == 0){
            break;
        } else {
            count++;
        }
    }

    box(win, 0, 0);
    wrefresh(win);

    if(primeiraVez){
        werase(win);
        estado_atual = ST_PEDIDO_PRINCIPAL;
    }
}

void listarTodosItensPedidos(WINDOW *win){
    int altura, largura;
    getmaxyx(win, altura, largura);

    keypad(win, true);
    noecho();
    curs_set(0);
    werase(win);
    box(win, 0, 0);

    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        mvwprintw(win, 2, 2, "Erro ao abrir arquivo de item de pedidos!");
        mvwprintw(win, 3, 2, "Pressione qualquer tecla...");
        wrefresh(win);
        wgetch(win);
        return;
    }

    char linha[BUFFER_ARQUIVO_LINHA];

    char *setinhas[] = {
        "<-- Página anterior",
        "Voltar (q)",
        "Próxima página -->"
    };
    const int quantidade = 3;
    int escolhido = 0;

    struct ItemPedido *vetor = NULL;
    int count = 0;

    while(fgets(linha, sizeof(linha), arq)){
        struct ItemPedido IP;

        if(sscanf(linha, "%d,%d,%d,%lf", &IP.pedidoId, &IP.produtoId, &IP.quantidade, &IP.subtotal) == 4){
            struct ItemPedido *temp = realloc(vetor, (count + 1) * sizeof(*vetor));
            if(!temp){
                free(vetor);
                fclose(arq);
                mvwprintw(win, 2, 2, "Erro de memória ao carregar itens de pedidos.");
                mvwprintw(win, 3, 2, "Pressione qualquer tecla...");
                wrefresh(win);
                wgetch(win);
                return;
            }

            vetor = temp;
            vetor[count++] = IP;
        }
    }
    fclose(arq);

    if(count == 0){
        mvwprintw(win, 2, 2, "Nenhum item de pedido encontrado.");
        mvwprintw(win, 4, 2, "Pressione qualquer tecla...");
        wrefresh(win);
        wgetch(win);
        free(vetor);
        return;
    }

    const int cabecalho_linhas = 4;
    const int rodape_linhas = 3;

    int linhas_por_pagina = altura - cabecalho_linhas - rodape_linhas;
    if(linhas_por_pagina < 1) linhas_por_pagina = 1;

    int paginas_total = (count + linhas_por_pagina - 1) / linhas_por_pagina;
    int pagina_atual = 0;

    int ch;

    while(1){
        int linhaY = 2; 

        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 3, "[ Lista de Itens de Pedidos ]");

        // Cabeçalho da tabela
        char header[128];
        snprintf(header, sizeof(header), "| %-15s | %-15s | %-15s | %-15s |", "Pedido ID", "Produto ID", "Quantidade", "Subtotal");
        int header_len = (int)strlen(header); // Tamanho do cabeçalho
        int header_x = (largura - header_len) / 2; // Posição X para centralizar
        mvwprintw(win, 2, header_x, "%s", header); // Cabeçalho centralizado

        // Índices dos itens a exibir nessa página
        int index_inicial = pagina_atual * linhas_por_pagina;
        int index_final = index_inicial + linhas_por_pagina;
        if(index_final > count) index_final = count;

        // Desenha as linhas da página
        for(int i = index_inicial; i < index_final; ++i){
            int row = linhaY + (i - index_inicial) + 1;
            mvwprintw(win, row, header_x, "| %-15d | %-15d | %-15d | %-15.2f |", vetor[i].pedidoId, vetor[i].produtoId, vetor[i].quantidade, vetor[i].subtotal);
        }

        // Rodapé com paginação e instruções
        mvwprintw(win, altura - 3, 2, "Página %d / %d", pagina_atual + 1, paginas_total);
        desenhaSetinhas(win, setinhas, quantidade, escolhido);
        wrefresh(win);

        ch = wgetch(win);

        if(ch == KEY_LEFT && pagina_atual > 0){
            escolhido = 0;
            pagina_atual--;
        }
        else if(ch == KEY_RIGHT && pagina_atual < paginas_total - 1){
            escolhido = 2;
            pagina_atual++;
        }
        else if(ch == 'q' || ch == 'Q' || ch == 27){ // ESC VOLTA
            escolhido = 1;
            break;
        }
        else if(ch == KEY_NPAGE){ // PageDown
            pagina_atual = (pagina_atual < paginas_total - 1) ? pagina_atual + 1 : pagina_atual;
        } else if(ch == KEY_PPAGE){ // PageUp
            pagina_atual = (pagina_atual > 0) ? pagina_atual - 1 : pagina_atual;
        }
    }

    free(vetor);
}

void removerItemPedido(WINDOW *win){
    keypad(win, true);
    echo();
    curs_set(1);
    werase(win);
    box(win, 0, 0);

    char mensagem[BUFFER_LINHA_CARACTERES];
    int idPedido;
    int idRemover;

    mvwprintw(win, 1, 2, "Remover Item de Pedido");
    mvwprintw(win, 3, 2, "Digite o ID do pedido ao qual o item pertence: ");
    wrefresh(win);

    do{
        idPedido = input_int(win, 3, 49);
        if(!validarIdPedido(idPedido, mensagem)){
            mvwprintw(win, 5, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedido(idPedido, mensagem));

    mvwprintw(win, 5, 2, "Digite o ID do item de pedido a ser removido: ");
    wrefresh(win);

    do{
        idRemover = input_int(win, 5, 48);
        if(!validarIdItemPedido(idRemover, mensagem)){
            mvwprintw(win, 7, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdItemPedido(idRemover, mensagem));

    // Confirmação de remoção
    if(!criaPopupMensagem(win, "Tem certeza que deseja apagar este item de pedido?", "Esta ação não pode ser desfeita!")) return;

    if(apagarItemPedido(idRemover, mensagem)){
        mvwprintw(win, 7, 2, "Pedido removido com sucesso!");
    } else {
        mvwprintw(win, 7, 2, "%s", mensagem);
    }

    mvwprintw(win, 8, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

void removerTodosItensPedido(WINDOW *win){
    keypad(win, true);
    echo();
    curs_set(1);
    werase(win);
    box(win, 0, 0);

    char mensagem[BUFFER_LINHA_CARACTERES];
    int idDoPedido;

    if(!criaPopupMensagem(win, "Tem certeza que deseja apagar TODOS os itens deste pedido?", "Esta ação não pode ser desfeita!")) return;

    mvwprintw(win, 0, 2, "[ Remover Todos Itens do Pedido ]");
    mvwprintw(win, 2, 2, "Digite o ID do pedido cujos itens serão removidos: ");
    wrefresh(win);

    do{
        idDoPedido = input_int(win, 2, 54);
        if(!validarIdPedido(idDoPedido, mensagem)){
            mvwprintw(win, 4, 2, "%100s", "");
            mvwprintw(win, 4, 2, "%s", mensagem);
            wrefresh(win);
        }
    } while(!validarIdPedido(idDoPedido, mensagem));

    if(apagarTodosItensDoPedido(idDoPedido, mensagem)){
        mvwprintw(win, 2, 2, "Todos os itens do pedido foram removidos com sucesso!");
    } else {
        mvwprintw(win, 2, 2, "%s", mensagem);
    }

    mvwprintw(win, 4, 2, "Pressione qualquer tecla...");
    wrefresh(win);
    wgetch(win);
}

double calcularTotal(struct Pedido *P, char *mensagem){
    FILE *arq = fopen(DIRETORIO_ARQUIVO_ITEM_PEDIDO, "r");
    if(!arq){
        sprintf(mensagem, "Erro ao abrir arquivo de itens de pedidos!");
        return -1.0;
    }

    double total = 0.0;
    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arq)){
        struct ItemPedido IP;

        if(sscanf(linha, "%d,%d,%d,%lf", &IP.pedidoId, &IP.produtoId, &IP.quantidade, &IP.subtotal) == 4){
            if(IP.pedidoId == P->id){
                total += IP.subtotal;
            }
        }
    }

    fclose(arq);

    return total;
}

double calcularSubtotal(struct ItemPedido *IP, char *mensagem){
    FILE *arqPrd = fopen(DIRETORIO_ARQUIVO_PRODUTO, "r");
    if(!arqPrd){
        sprintf(mensagem, "Erro ao abrir arquivo de produtos!");
        return -1.0;
    }

    double subtotal = -1.0;
    char linha[BUFFER_ARQUIVO_LINHA];

    while(fgets(linha, sizeof(linha), arqPrd)){
        Produto Pd;

        if(sscanf(linha, "%d,%99[^,],%lf,%d", &Pd.id, Pd.descricao, &Pd.preco, &Pd.estoque) == 4){
            if(IP->produtoId == Pd.id){
                subtotal = Pd.preco * IP->quantidade;
                break;
            }
        }
    }

    IP->subtotal = subtotal;

    fclose(arqPrd);

    if(subtotal < 0.0) sprintf(mensagem, "Produto não encontrado para cálculo do subtotal.");

    return subtotal;
}

//  =========================================================
//                  Tela Pedidos INTERFACE
//  =========================================================

static void desenhaOpcoesVertical(WINDOW *win, char *labels[], int total, int selecionado, int temFoco){
    if(!win || total <= 0) return; // Não renderiza se não tiver opções nem interface

    int altura, largura;
    getmaxyx(win, altura, largura);

    // Layout dos botões
    int espacamento = 3;
    int altura_total = espacamento * (total - 1);

    // Horizontal
    int inicio_x = (largura - 36) / 2;
    if(inicio_x < 1) inicio_x = 1;

    // Vertical
    int inicio_y = (altura - altura_total) / 2;
    if(inicio_y < 1) inicio_y = 1;

    int x = inicio_x;
    int y = inicio_y;

    // Como a janela aparace na frente é necessário limpar seu interior
    for(int i=1; i<altura - 1; ++i) mvwhline(win, i, 1, ' ', largura-2);

    for(int i=0; i<total; i++){
        char labelEscolhido[BUFFER_LINHA_CARACTERES];
        char labelNaoEscolhido[BUFFER_LINHA_CARACTERES];

        snprintf(labelEscolhido, sizeof(labelEscolhido), "> %-34s", labels[i]);
        snprintf(labelNaoEscolhido, sizeof(labelNaoEscolhido), "%-36s  ", labels[i]);

        if(i == selecionado){
            if (temFoco) wattron(win, A_REVERSE | A_BOLD);
            mvwprintw(win, y, x, "%s", labelEscolhido);
            if (temFoco) wattroff(win, A_REVERSE | A_BOLD);
        } else {
            mvwprintw(win, y, x, "%s", labelNaoEscolhido);
        }

        y += espacamento;
    }

    if (temFoco) wattron(win, A_BOLD);
    box(win, 0, 0);
    if (temFoco) wattroff(win, A_BOLD);

    wrefresh(win);
}

static void tutorialLateral(WINDOW *win){
    int altura, largura;
    getmaxyx(win, altura, largura);

    // Limpa a janela, evitando glitches visuais
    werase(win);
    for(int i=1; i<altura - 1; ++i) mvwhline(win, i, 1, ' ', largura-2);
    box(win, 0, 0);
    wrefresh(win);

    mvwprintw(win, 0, 2, "[ TUTORIAL ]");
    mvwprintw(win, 2, 3, "Bem-vindo ao menu de Pedidos!");
    mvwprintw(win, 4, 3, "Use as setas do teclado para selecionar uma opção.");
    mvwprintw(win, 5, 3, "Pressione '%c' para mudar para a opção de cima.", ACS_UARROW);
    mvwprintw(win, 6, 3, "Pressione '%c' para mudar para a opção de baixo.", ACS_DARROW);
    mvwprintw(win, 7, 3, "Pressione ENTER para executar a ação selecionada.");
    mvwprintw(win, 8, 3, "Pressione '<--' ou '-->' para navegar entre páginas quando disponível.");
    mvwprintw(win, 9, 3, "Pressione 'TAB' + '<--' ou '-->' para exibir informações sobre opções.");
    mvwprintw(win, 10, 3, "Pressione 'q' para voltar ao menu principal.");

    wrefresh(win);
}

void menuPedidos(){
    noecho();
    curs_set(0);

    char *opcoesPedido[] = {
        "Cadastrar Pedido",
        "Listar Pedidos",
        "Consultar Pedido",
        "Remover Pedido",
        "Cadastrar Item de Pedido",
        "Listar Itens de Pedido",
        "Remover Item de Pedido",
        "Apagar todos os Pedidos",
        "Apagar todos os Itens de Pedidos",
        "Voltar"
    };
    const int totalBotoesPedido = 10;

    int selecionado = 0;
    int focus = 0;
    int ch;

    erase();
    refresh();

    int altura, largura;
    getmaxyx(stdscr, altura, largura);

    // LOOP PRINCIPAL
    while(1){
        // Obtenção do tamanho da tela e das cooredenadas de inicio
        getmaxyx(stdscr, altura, largura);

        // Imprime dimensões da tela no canto da tela
        char texto_do_tamanho[32];
        snprintf(texto_do_tamanho, sizeof(texto_do_tamanho), "%dx%d", altura, largura);

        // Dimensões da janela
        int altura_janela = altura - UI_MARGIN;
        int largura_janela = largura - UI_MARGIN;

        int posX_janela = UI_PADDING;
        //int posY_janela = UI_PADDING;

        int largura_esquerda = (largura_janela) / 3;
        int largura_direita = (largura - largura_esquerda - 1);

        int topo = 1;
        int esquerda_x = posX_janela;
        int direita_x = largura_esquerda + 1;

        // Impede que o terminal fique muito pequeno, por padrão as dimensões minimas são (20x60)
        if(ehTerminalPequeno(altura, largura)) continue;

        // Cria as janelas
        WINDOW *janelaEsquerda = newwin(altura_janela, largura_esquerda, topo, esquerda_x);
        WINDOW *janelaDireita = newwin(altura_janela, largura_direita, topo, direita_x);

        // Limpa tudo
        werase(janelaEsquerda);
        box(janelaEsquerda, 0, 0);

        // Desenho da UI
        desenhaOpcoesVertical(janelaEsquerda, opcoesPedido, totalBotoesPedido, selecionado, focus == 0);
        
        // Parte de cima
        mvwprintw(janelaEsquerda, 0, 2, "[ Menu de Pedidos ]");

        // Texto das Dimensões
        mvwprintw(stdscr, altura_janela - UI_MARGIN, largura_janela - (int)strlen(texto_do_tamanho) - UI_MARGIN, "%s", texto_do_tamanho);

        // Quando não tem nada, a janela direita vira um tutorial
        if(!focus) tutorialLateral(janelaDireita);

        refresh();
        wrefresh(janelaEsquerda);

        keypad(janelaEsquerda, true);

        ch = wgetch(janelaEsquerda);

        // Controle dos botões, ENTER confirma e sai do loop
        if(ch == 'q' || ch == 'Q'){
            delwin(janelaDireita);
            delwin(janelaEsquerda);
            estado_atual = ST_MENU_PRINCIPAL;
            return;
        } else if(ch == KEY_UP && selecionado > 0) selecionado--;
        else if(ch == KEY_DOWN && selecionado < totalBotoesPedido - 1) selecionado++;

        // Foco na janela esquerda
        else if(ch == '\t' || ch == KEY_BTAB) focus = (focus + 1) % 2;

        // Exibe informações na janela direita conforme a opção selecionada
        // Cada opção tem sua própria função chamada ao pressionar ENTER
        else if(selecionado == 0){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);
        
            mvwprintw(janelaDireita, 0, 2, "[ Cadastrar Pedido ]");
            mvwprintw(janelaDireita, 2, 2, "Permite cadastrar um novo pedido no sistema.");
            mvwprintw(janelaDireita, 3, 2, "Você poderá adicionar itens ao pedido após o cadastro.");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                // PEDIDO
                werase(janelaDireita);
                box(janelaDireita, 0, 0);
                wrefresh(janelaDireita);

                cadastrarPedido(janelaDireita, &pedidoGlobal, &itemPedidoGlobal);
            }
        }

        else if(selecionado == 1){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);
            
            mvwprintw(janelaDireita, 0, 2, "[ Listar Pedidos ]");
            mvwprintw(janelaDireita, 2, 2, "Exibe uma lista paginada de todos os pedidos cadastrados.");
            mvwprintw(janelaDireita, 3, 2, "Use as setas para navegar entre as páginas.");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                // PEDIDO
                werase(janelaDireita);
                box(janelaDireita, 0, 0);
                wrefresh(janelaDireita);

                listarPedidos(janelaDireita);
            }
        }

        else if(selecionado == 2){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);

            mvwprintw(janelaDireita, 0, 2, "[ Consultar Pedido ]");
            mvwprintw(janelaDireita, 2, 2, "Permite consultar detalhes de um pedido específico.");
            mvwprintw(janelaDireita, 3, 2, "Você precisará informar o ID do pedido para consulta.");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                // PEDIDO
                werase(janelaDireita);
                box(janelaDireita, 0, 0);
                wrefresh(janelaDireita);

                consultarPedido(janelaDireita);
            }
        }

        else if(selecionado == 3){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);

            mvwprintw(janelaDireita, 0, 2, "[ Remover Pedido ]");
            mvwprintw(janelaDireita, 2, 2, "Permite remover um pedido do sistema.");
            mvwprintw(janelaDireita, 3, 2, "Você precisará informar o ID do pedido para remoção.");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                // PEDIDO
                werase(janelaDireita);
                box(janelaDireita, 0, 0);
                wrefresh(janelaDireita);

                removerPedido(janelaDireita);
            }
        }

        else if(selecionado == 4){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);

            mvwprintw(janelaDireita, 0, 2, "[ Cadastrar Item de Pedido ]");
            mvwprintw(janelaDireita, 2, 2, "Permite adicionar itens a um pedido existente.");
            mvwprintw(janelaDireita, 3, 2, "Você precisará informar o ID do pedido ao qual deseja adicionar itens.");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                // PEDIDO
                werase(janelaDireita);
                box(janelaDireita, 0, 0);
                wrefresh(janelaDireita);

                cadastrarItemPedido(janelaDireita, &pedidoGlobal, &itemPedidoGlobal, 0);
            }
        }

        else if(selecionado == 5){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);

            mvwprintw(janelaDireita, 0, 2, "[ Listar Itens de Pedido ]");
            mvwprintw(janelaDireita, 2, 2, "Exibe uma lista paginada de todos os itens de pedidos cadastrados.");
            mvwprintw(janelaDireita, 3, 2, "Use as setas para navegar entre as páginas.");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                // PEDIDO
                werase(janelaDireita);
                box(janelaDireita, 0, 0);
                wrefresh(janelaDireita);

                listarTodosItensPedidos(janelaDireita);
            }
        }

        else if(selecionado == 6){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);

            mvwprintw(janelaDireita, 0, 2, "[ Remover Item de Pedido ]");
            mvwprintw(janelaDireita, 2, 2, "Permite remover um item de um pedido existente.");
            mvwprintw(janelaDireita, 3, 2, "Você precisará informar o ID do item de pedido para remoção.");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                // PEDIDO
                werase(janelaDireita);
                box(janelaDireita, 0, 0);
                wrefresh(janelaDireita);

                removerItemPedido(janelaDireita);
            }
        }

        else if(selecionado == 7){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);

            mvwprintw(janelaDireita, 0, 2, "[ Apagar todos os Pedidos ]");
            mvwprintw(janelaDireita, 2, 2, "Permite remover todos os pedidos cadastrados no sistema.");
            mvwprintw(janelaDireita, 3, 2, "Cuidado: Esta ação é irreversível!");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                // PEDIDO
                werase(janelaDireita);
                box(janelaDireita, 0, 0);
                wrefresh(janelaDireita);

                removerTodosPedidos(janelaDireita);
            }
        }

        else if(selecionado == 8){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);

            mvwprintw(janelaDireita, 0, 2, "[ Apagar todos os Itens de Pedidos ]");
            mvwprintw(janelaDireita, 2, 2, "Permite remover todos os itens de pedidos cadastrados no sistema.");
            mvwprintw(janelaDireita, 3, 2, "Cuidado: Esta ação é irreversível!");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                // PEDIDO
                werase(janelaDireita);
                box(janelaDireita, 0, 0);
                wrefresh(janelaDireita);

                removerTodosItensPedido(janelaDireita);
            }
        }

        else if(selecionado == 9){
            werase(janelaDireita);
            box(janelaDireita, 0, 0);

            mvwprintw(janelaDireita, 0, 2, "[ Voltar ]");
            mvwprintw(janelaDireita, 2, 2, "Retorna ao menu principal do sistema.");
            wrefresh(janelaDireita);

            if(ch == 10 || ch == '\n'){
                delwin(janelaDireita);
                delwin(janelaEsquerda);
                estado_atual = ST_MENU_PRINCIPAL;
                return;
            }
        }
        
        delwin(janelaDireita);
        delwin(janelaEsquerda);
    }
}
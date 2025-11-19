#include "../include/estados.h"
#include "../include/interface.h"
#include "../include/cliente.h"
#include "../include/pedido.h"
#include "../include/persistencia.h"
#include "../include/utils.h"

int main(){
    /*estado_atual = ST_ABRINDO;

    while(estado_atual != ST_SAINDO){
        estado_atual = processaEstado(estado_atual);
    }
    */

    inicializaInterface();
    
    animacaoAbertura();
    menuPrincipal();

    finalizaInterface();

    return 0;
}
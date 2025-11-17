#include <stdio.h>
#include <cliente.h>
#include <interface.h>
#include <pedido.h>
#include <persistencia.h>
#include <produto.h>

int main(){
    struct Pedido P;

    cadastrarPedido(&P);

    return 0;
}
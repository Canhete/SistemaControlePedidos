#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char* obterDataAtual(){
    static char dataAtual[11];
    time_t agora;
    struct tm *infoTempo;

    time(&agora);
    infoTempo = localtime(&agora);
    strftime(dataAtual, sizeof(dataAtual), "%d/%m/%Y", infoTempo);

    return dataAtual;
}
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "util.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void obterDataHora(char *dest) {
    time_t t = time(NULL);
    struct tm *info = localtime(&t);
    strftime(dest, 20, "%d/%m/%Y %H:%M", info);
}

void formatarMoeda(float valor, char *destino) {
    char temp[50];
    sprintf(temp, "%.2f", valor);
    
    char *ponto = strchr(temp, '.');
    if (!ponto) { // Caso não tenha ponto (improvável com %.2f)
        strcpy(destino, temp);
        return;
    }
    
    int len_inteiro = ponto - temp;
    int pos_dest = 0;
    
    for (int i = 0; i < len_inteiro; i++) {
        // Adiciona ponto a cada 3 dígitos da direita para a esquerda
        if (i > 0 && (len_inteiro - i) % 3 == 0) {
            destino[pos_dest++] = '.';
        }
        destino[pos_dest++] = temp[i];
    }
    
    // Substitui ponto decimal por vírgula
    destino[pos_dest++] = ',';
    destino[pos_dest++] = ponto[1];
    destino[pos_dest++] = ponto[2];
    destino[pos_dest] = '\0';
}

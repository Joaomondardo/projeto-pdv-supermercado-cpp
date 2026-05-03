#include <stdio.h>
#include <time.h>
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

#ifndef UI_H
#define UI_H

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

// ─────────────────────────────────────────────────
// ATRIBUTOS DE CORES (BG, FG)
// ─────────────────────────────────────────────────
#define ATTR(bg, fg)     (((bg) << 4) | (fg))

#define C_PRETO   0
#define C_AZUL    1
#define C_VERDE   2
#define C_CIANO   3
#define C_VERM    4
#define C_MAG     5
#define C_AMAR    6
#define C_CINZA   7
#define C_CINZA_E 8
#define C_AZUL_B  9
#define C_VERDE_B 10
#define C_CIANO_B 11
#define C_VERM_B  12
#define C_AMAR_B  14
#define C_BRANCO  15

#define ATTR_NORMAL      ATTR(C_PRETO, C_CINZA)
#define ATTR_HEADER      ATTR(C_AZUL, C_BRANCO)
#define ATTR_SECTION     ATTR(C_PRETO, C_CIANO_B)
#define ATTR_PDV         ATTR(C_VERDE, C_BRANCO)
#define ATTR_TOTAL       ATTR(C_PRETO, C_AMAR_B)
#define ATTR_SUCESSO_BG  ATTR(C_VERDE, C_BRANCO)
#define ATTR_ERRO_BG     ATTR(C_VERM, C_BRANCO)
#define ATTR_DESTAQUE    ATTR(C_PRETO, C_AMAR_B)
#define ATTR_OPCAO_NUM   ATTR(C_PRETO, C_AMAR_B)
#define ATTR_PROMPT      ATTR(C_PRETO, C_VERDE_B)

#define LARGURA_UI       60

inline HANDLE _hConsole() {
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

inline void attr(int a) {
    SetConsoleTextAttribute(_hConsole(), a);
}

inline void cor_reset() {
    SetConsoleTextAttribute(_hConsole(), ATTR_NORMAL);
}

inline void limparTela() {
    system("cls");
}

// Imprime uma barra horizontal
inline void linha_horizontal(char c) {
    for (int i = 0; i < LARGURA_UI; i++) printf("%c", c);
    printf("\n");
}

// Cabeçalho estilizado
inline void cabecalho(const char* titulo) {
    attr(ATTR_HEADER);
    int len = (int)strlen(titulo);
    int pad = (LARGURA_UI - len) / 2;
    for (int i = 0; i < LARGURA_UI; i++) printf(" "); printf("\r");
    for (int i = 0; i < pad; i++) printf(" ");
    printf("%s\n", titulo);
    for (int i = 0; i < LARGURA_UI; i++) printf(" "); 
    cor_reset();
    printf("\n");
}

// Título de seção
inline void titulo_secao(const char* titulo) {
    printf("\n");
    attr(ATTR_SECTION);
    printf("  %s\n", titulo);
    cor_reset();
    printf("  ");
    for (int i = 0; i < (int)strlen(titulo) + 2; i++) printf("-");
    printf("\n");
}

// Opção de menu compacta
inline void item_menu(int num, const char* texto) {
    printf("    ");
    attr(ATTR_OPCAO_NUM);
    printf("%2d", num);
    cor_reset();
    printf(" . %-30s", texto);
    if (num % 2 == 0) printf("\n"); // Duas colunas se for par (ajuste manual no menu)
}

// Funções de compatibilidade
inline void cor(int c) {
    SetConsoleTextAttribute(_hConsole(), c);
}

inline void separador() {
    cor(C_CINZA_E);
    printf("  ");
    for (int i = 0; i < LARGURA_UI - 4; i++) printf("-");
    printf("\n");
    cor_reset();
}

inline void barra(int atrib, const char* texto) {
    attr(atrib);
    int len = (int)strlen(texto);
    int pad = (LARGURA_UI - len) / 2;
    for (int i = 0; i < LARGURA_UI; i++) printf(" "); printf("\r");
    for (int i = 0; i < pad; i++) printf(" ");
    printf("%s\n", texto);
    cor_reset();
}

// Aliases para compatibilidade com código antigo
#define COR_ERRO         C_VERM_B
#define COR_SUCESSO      C_VERDE_B
#define COR_DESTAQUE     C_AMAR_B
#define COR_SEPARADOR    C_CINZA_E
#define COR_INFO         C_AZUL_B
#define COR_TITULO       C_CIANO_B
#define COR_OPCAO        C_BRANCO

inline void secao(const char* t) { titulo_secao(t); }
inline void opcao_menu(int n, const char* t) { item_menu(n, t); }

inline void pausar() {
    printf("\n  ");
    attr(ATTR_PROMPT);
    printf(" Pressione ENTER para continuar... ");
    cor_reset();
    limparBuffer();
    getchar();
}

#endif

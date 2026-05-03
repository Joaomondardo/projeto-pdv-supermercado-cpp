#ifndef IMPRESSORA_H
#define IMPRESSORA_H

#include <vector>
#include <string>

// Comandos ESC/POS Universais
#define ESC_INIT         "\x1B\x40"          // Inicializa impressora
#define ESC_ALIGN_LEFT   "\x1B\x61\x00"      // Alinha esquerda
#define ESC_ALIGN_CENTER "\x1B\x61\x01"      // Alinha centro
#define ESC_ALIGN_RIGHT  "\x1B\x61\x02"      // Alinha direita
#define ESC_BOLD_ON      "\x1B\x45\x01"      // Negrito ON
#define ESC_BOLD_OFF     "\x1B\x45\x00"      // Negrito OFF
#define ESC_DOUBLE_ON    "\x1B\x21\x30"      // Fonte Grande
#define ESC_DOUBLE_OFF   "\x1B\x21\x00"      // Fonte Normal
#define ESC_CUT          "\x1D\x56\x42\x00"  // Corte de papel (Guilhotina)

struct ItemCupom {
    char nome[100];
    int qtd;
    float preco;
    float subtotal;
};

void imprimirCupomFisico(ItemCupom* itens, int totalItens, float total, float pago, float troco);

#endif

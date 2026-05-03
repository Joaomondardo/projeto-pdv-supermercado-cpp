#include <stdio.h>
#include <time.h>
#include <string.h>
#include "impressora.h"
#include "util.h"

void imprimirCupomFisico(ItemCupom* itens, int totalItens, float total, float pago, float troco)
{
    FILE *f = fopen("cupom_termico.bin", "wb"); 
    if (f == NULL) {
        printf("Erro ao acessar porta da impressora.\n");
        return;
    }

    char dh[20];
    obterDataHora(dh);

    fprintf(f, ESC_INIT);
    fprintf(f, ESC_ALIGN_CENTER);
    fprintf(f, ESC_DOUBLE_ON);
    fprintf(f, "SUPERMERCADO PDV\n");
    fprintf(f, ESC_DOUBLE_OFF);
    fprintf(f, "--------------------------------\n");

    fprintf(f, ESC_ALIGN_LEFT);
    fprintf(f, "Data/Hora: %s\n", dh);
    fprintf(f, "--------------------------------\n");
    fprintf(f, "ITEM   QTD   UN   PRECO   SUBTOTAL\n");

    for (int i = 0; i < totalItens; i++) {
        fprintf(f, "%-15.15s %3d x %6.2f %7.2f\n", 
                itens[i].nome, itens[i].qtd, itens[i].preco, itens[i].subtotal);
    }

    fprintf(f, "--------------------------------\n");
    fprintf(f, ESC_BOLD_ON);
    fprintf(f, ESC_ALIGN_RIGHT);
    fprintf(f, "TOTAL: R$ %10.2f\n", total);
    fprintf(f, ESC_BOLD_OFF);
    fprintf(f, "PAGO:  R$ %10.2f\n", pago);
    fprintf(f, "TROCO: R$ %10.2f\n", troco);
    fprintf(f, "\n");

    fprintf(f, ESC_ALIGN_CENTER);
    fprintf(f, "Obrigado pela preferencia!\n");
    fprintf(f, "Volte Sempre!\n\n\n\n");
    fprintf(f, ESC_CUT);

    fclose(f);

    printf("\n  [ OK ] Cupom enviado para a fila de impressao (cupom_termico.bin)!\n");
}

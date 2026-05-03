#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "caixa.h"
#include "estoque.h"
#include "movimentacoes.h"
#include "database.h"
#include "util.h"
#include "ui.h"
#include "impressora.h"

/* ═══════════════════════════════════════════════════════
   ESTRUTURA DO CARRINHO DE COMPRAS
   ═══════════════════════════════════════════════════════ */

#define MAX_ITENS_VENDA 200

typedef struct {
    int   codigo;
    char  nome[100];
    float preco_unit;
    int   qtd;
    float subtotal;
} ItemVenda;

static ItemVenda carrinho[MAX_ITENS_VENDA];
static int totalItens = 0;
static float totalVenda = 0.0f;

/* ═══════════════════════════════════════════════════════
   FUNÇÕES INTERNAS
   ═══════════════════════════════════════════════════════ */

static void limparCarrinho()
{
    totalItens = 0;
    totalVenda = 0.0f;
}

static void exibirCarrinho()
{
    int i;
    printf("\n");
    attr(ATTR_HEADER);
    printf("  %-5s | %-28s | %7s | %3s | %10s  ", "COD", "PRODUTO", "PRECO", "QTD", "SUBTOTAL");
    cor_reset();
    printf("\n");

    for (i = 0; i < totalItens; i++) {
        printf("  %04d  | %-28s | %7.2f | %3d | %10.2f\n",
               carrinho[i].codigo,
               carrinho[i].nome,
               carrinho[i].preco_unit,
               carrinho[i].qtd,
               carrinho[i].subtotal);
    }

    separador();
    printf("  %-42s  ", "");
    attr(ATTR_TOTAL);
    printf(" TOTAL: R$ %8.2f ", totalVenda);
    cor_reset();
    printf("\n");
}

static void salvarEImprimirCupom(int num_venda)
{
    int i;
    char filename[100];
    char dh[20];
    obterDataHora(dh);

    // Cria a pasta de cupons se não existir
    system("mkdir cupons 2>nul");

    sprintf(filename, "cupons\\cupom_%04d.txt", num_venda);
    FILE *arq = fopen(filename, "w");
    if (arq == NULL) {
        printf("  [!] Nao foi possivel salvar o cupom.\n");
        return;
    }

    fprintf(arq, "================================================\n");
    fprintf(arq, "        SUPERMERCADO - CUPOM NAO FISCAL\n");
    fprintf(arq, "================================================\n");
    fprintf(arq, "  Venda #%04d\n", num_venda);
    fprintf(arq, "  Data/Hora: %s\n", dh);
    fprintf(arq, "------------------------------------------------\n");
    fprintf(arq, "  %-28s  %5s  %10s\n", "Produto", "Qtd", "Subtotal");
    fprintf(arq, "  %-28s  %5s  %10s\n", "----------------------------", "-----", "----------");

    for (i = 0; i < totalItens; i++) {
        fprintf(arq, "  %-28s  %5d  R$%8.2f\n",
                carrinho[i].nome, carrinho[i].qtd, carrinho[i].subtotal);
    }

    fprintf(arq, "------------------------------------------------\n");
    fprintf(arq, "  TOTAL DA COMPRA:                   R$%8.2f\n", totalVenda);
    fprintf(arq, "================================================\n");
    fprintf(arq, "  Obrigado pela preferencia!\n");
    fprintf(arq, "================================================\n");
    fclose(arq);

    printf("  Cupom salvo em: %s\n", filename);

    // Manda imprimir na impressora padrão do Windows
    printf("  Deseja imprimir o cupom? (s/n): ");
    char resp;
    scanf("%c", &resp);
    limparBuffer();

    if (resp == 's' || resp == 'S') {
        char cmd[200];
        // ShellExecute com verbo "print" envia para a impressora padrão
        sprintf(cmd, "ShellExecute print \"%s\"", filename);
        // Usamos notepad /p para imprimir via Notepad silenciosamente
        sprintf(cmd, "notepad /p \"%s\"", filename);
        system(cmd);
        printf("  Cupom enviado para a impressora!\n");
    }
}

static void imprimirCupom(int num_venda)
{
    int i;
    printf("\n");
    printf("  ================================================\n");
    printf("        SUPERMERCADO - CUPOM NAO FISCAL\n");
    printf("  ================================================\n");
    printf("  Venda #%04d\n", num_venda);
    char dh[20];
    obterDataHora(dh);
    printf("  Data/Hora: %s\n", dh);
    printf("  ------------------------------------------------\n");
    printf("  %-28s  %5s  %10s\n", "Produto", "Qtd", "Subtotal");
    printf("  %-28s  %5s  %10s\n", "----------------------------", "-----", "----------");

    for (i = 0; i < totalItens; i++) {
        char sSub[20];
        formatarMoeda(carrinho[i].subtotal, sSub);
        printf("  %-28s  %5d  R$%10s\n",
               carrinho[i].nome,
               carrinho[i].qtd,
               sSub);
    }

    printf("  ------------------------------------------------\n");
    printf("  TOTAL DA COMPRA:                   R$%8.2f\n", totalVenda);
    printf("  ================================================\n");
    printf("  Obrigado pela preferencia!\n");
    printf("  ================================================\n\n");
}

static int adicionarItem(int codigo, int qtd)
{
    int i;
    Produto p;

    // Busca o produto direto no Banco de Dados para garantir dados reais
    if (!buscarProdutoPorCodigoDB(codigo, &p)) {
        cor(COR_ERRO);
        printf("  [!] Produto com codigo %d nao encontrado!\n", codigo);
        cor_reset();
        return 0;
    }

    // Verifica se há estoque suficiente
    if (p.quantidade < qtd) {
        cor(COR_ERRO);
        printf("  [!] Estoque insuficiente! Disponivel: %d unidades.\n", p.quantidade);
        cor_reset();
        return 0;
    }

    // Se o produto já está no carrinho, soma a quantidade
    for (i = 0; i < totalItens; i++) {
        if (carrinho[i].codigo == codigo) {
            if (p.quantidade < (carrinho[i].qtd + qtd)) {
                printf("  [!] Estoque insuficiente para adicionar mais %d unidades.\n", qtd);
                return 0;
            }
            carrinho[i].qtd += qtd;
            carrinho[i].subtotal = carrinho[i].preco_unit * carrinho[i].qtd;
            totalVenda += carrinho[i].preco_unit * qtd;
            cor(COR_SUCESSO);
            printf("  [+] Adicionado: %s (x%d) = R$ %.2f\n",
                   p.nome, qtd,
                   carrinho[i].preco_unit * qtd);
            cor_reset();
            return 1;
        }
    }

    // Produto novo no carrinho
    if (totalItens >= MAX_ITENS_VENDA) {
        printf("  [!] Limite de itens da venda atingido.\n");
        return 0;
    }

    carrinho[totalItens].codigo = codigo;
    strcpy(carrinho[totalItens].nome, p.nome);
    carrinho[totalItens].preco_unit = p.preco;
    carrinho[totalItens].qtd = qtd;
    carrinho[totalItens].subtotal = p.preco * qtd;
    totalVenda += carrinho[totalItens].subtotal;
    totalItens++;

    cor(COR_SUCESSO);
    printf("  [+] Adicionado: %s (x%d) = R$ %.2f\n",
           p.nome, qtd, p.preco * qtd);
    cor_reset();
    return 1;
}

static void exibirMenuCaixa()
{
    limparTela();
    cabecalho("PONTO DE VENDA (PDV) - CAIXA LIVRE");

    titulo_secao("OPERACOES");
    item_menu(1, "Adicionar Item (Cod)");
    item_menu(2, "Ver Carrinho");
    item_menu(6, "Buscar Produto (Nome)");
    printf("\n");
    item_menu(3, "Remover Ultimo");
    item_menu(5, "Cancelar Venda");
    printf("\n");

    titulo_secao("FINALIZACAO");
    printf("    ");
    attr(ATTR_HEADER);
    printf(" [ 4 ] FINALIZAR E PAGAR ");
    cor_reset();
    printf("\n");

    printf("\n    ");
    attr(ATTR_ERRO_BG);
    printf(" [ 0 ] SAIR DO CAIXA ");
    cor_reset();
    printf("\n");

    linha_horizontal('-');
}

static void finalizarVenda()
{
    int i, indice, opcao;
    float pago = 0.0f;

    if (totalItens == 0) {
        printf("  [!] Carrinho vazio. Nenhuma venda a finalizar.\n");
        return;
    }

    exibirCarrinho();

    printf("\n");
    titulo_secao("FORMA DE PAGAMENTO");
    item_menu(1, "Dinheiro");
    item_menu(2, "Cartao de Credito");
    item_menu(3, "Cartao de Debito");
    item_menu(4, "PIX");
    printf("\n");
    
    attr(ATTR_PROMPT);
    printf("\n  ESCOLHA: ");
    cor_reset();
    scanf("%d", &opcao);
    limparBuffer();

    const char* forma = "Cartao de Credito";
    if (opcao == 1) {
        forma = "Dinheiro";
        printf("  Valor pago: R$ ");
        scanf("%f", &pago);
        limparBuffer();

        if (pago < totalVenda) {
            printf("  [!] Valor insuficiente!\n");
            return;
        }
        char sTroco[20];
        formatarMoeda(pago - totalVenda, sTroco);
        printf("  Troco: R$ %s\n", sTroco);
    } else if (opcao == 3) {
        forma = "Cartao de Debito";
    } else if (opcao == 4) {
        forma = "PIX";
    }

    // Confirma a venda
    char sTotal[20];
    formatarMoeda(totalVenda, sTotal);
    printf("\n  Confirmar venda de R$ %s no %s? (s/n): ", sTotal, forma);
    char conf;
    scanf("%c", &conf);
    limparBuffer();
    if (conf != 's' && conf != 'S') {
        printf("  Venda cancelada.\n");
        return;
    }

    // ======================================================
    // PERSISTE A VENDA NO BANCO (AUDITORIA)
    // ======================================================
    char dh[20];
    obterDataHora(dh);

    // 1. Insere o cabeçalho da venda
    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO vendas (data_hora, total, forma_pagamento) VALUES ('%s', %.2f, '%s')",
        dh, totalVenda, forma);

    if (mysql_query(conexao, query)) {
        cor(COR_ERRO);
        printf("  [!] Erro fatal ao registrar venda no banco: %s\n", mysql_error(conexao));
        cor_reset();
        return;
    }

    // 2. Recupera o ID real gerado — esse é o número oficial do cupom!
    int num_venda = (int)mysql_insert_id(conexao);

    // 3. Insere cada item da venda
    for (i = 0; i < totalItens; i++) {
        char qitem[1024];
        char nomeBlindado[300];
        blindarTexto(carrinho[i].nome, nomeBlindado);

        snprintf(qitem, sizeof(qitem),
            "INSERT INTO itens_venda (venda_id, codigo_produto, nome_produto, quantidade, preco_unit, subtotal) "
            "VALUES (%d, %d, '%s', %d, %.2f, %.2f)",
            num_venda, carrinho[i].codigo, nomeBlindado,
            carrinho[i].qtd, carrinho[i].preco_unit, carrinho[i].subtotal);
            
        if (mysql_query(conexao, qitem)) {
            printf("  [!] Aviso: Erro ao registrar item %d no banco.\n", carrinho[i].codigo);
        }
    }
    // ======================================================

    // Registra saídas e atualiza estoque no banco
    for (i = 0; i < totalItens; i++) {
        indice = buscarIndicePorCodigo(carrinho[i].codigo);
        if (indice != -1) {
            produtos[indice].quantidade -= carrinho[i].qtd;

            char qstk[512];
            snprintf(qstk, sizeof(qstk), "UPDATE produtos SET quantidade=%d WHERE codigo=%d",
                    produtos[indice].quantidade, carrinho[i].codigo);
            
            if (mysql_query(conexao, qstk)) {
                printf("  [!] Erro critico ao atualizar estoque do produto %d!\n", carrinho[i].codigo);
            }

            registrarSaidaDireta(carrinho[i].codigo, carrinho[i].nome,
                                 carrinho[i].qtd, produtos[indice].quantidade);
        }
    }

    // Mostra e salva o cupom com o número real do banco
    imprimirCupom(num_venda);
    salvarEImprimirCupom(num_venda);

    // --- NOVA FUNCIONALIDADE: IMPRESSAO TERMICA ---
    printf("\n  Deseja imprimir o cupom físico na IMPRESSORA TERMICA? (s/n): ");
    char resp_fisica;
    scanf(" %c", &resp_fisica);
    limparBuffer();

    if (resp_fisica == 's' || resp_fisica == 'S') {
        ItemCupom itensFisicos[MAX_ITENS_VENDA];
        for (i = 0; i < totalItens; i++) {
            strncpy(itensFisicos[i].nome, carrinho[i].nome, sizeof(itensFisicos[i].nome)-1);
            itensFisicos[i].nome[sizeof(itensFisicos[i].nome)-1] = '\0';
            itensFisicos[i].qtd = carrinho[i].qtd;
            itensFisicos[i].preco = carrinho[i].preco_unit;
            itensFisicos[i].subtotal = carrinho[i].subtotal;
        }
        imprimirCupomFisico(itensFisicos, totalItens, totalVenda, pago > 0 ? pago : totalVenda, (pago > totalVenda ? pago - totalVenda : 0.0f));
    }
    // ----------------------------------------------

    limparCarrinho();
    printf("  Venda #%04d finalizada com sucesso!\n", num_venda);
    exibirMenuCaixa();
}

/* ═══════════════════════════════════════════════════════
   MODO CAIXA (PDV) - LOOP PRINCIPAL
   ═══════════════════════════════════════════════════════ */

void modoCaixa()
{
    int opcao, codigo, qtd;

    exibirMenuCaixa();

    limparCarrinho();

    do {
        /* ── STATUS DA VENDA ── */
        char sSubtotal[20];
        formatarMoeda(totalVenda, sSubtotal);
        printf("\n  ");
        attr(ATTR_HEADER);
        printf(" SUBTOTAL: R$ %s ", sSubtotal);
        cor_reset();
        printf("  |  ITENS: %d\n", totalItens);

        attr(ATTR_PROMPT);
        printf("  > CODIGO OU OPCAO: ");
        cor_reset();
        
        opcao = -1; // Reseta para evitar loop infinito em caso de erro no scanf
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                printf("  Codigo do produto: ");
                scanf("%d", &codigo);
                limparBuffer();
                printf("  Quantidade: ");
                scanf("%d", &qtd);
                limparBuffer();
                if (qtd <= 0) qtd = 1;
                adicionarItem(codigo, qtd);
                break;

            case 2:
                if (totalItens == 0)
                    printf("  Carrinho vazio.\n");
                else
                    exibirCarrinho();
                break;

            case 3:
                if (totalItens > 0) {
                    totalVenda -= carrinho[totalItens - 1].subtotal;
                    printf("  Removido: %s\n", carrinho[totalItens - 1].nome);
                    totalItens--;
                } else {
                    printf("  Carrinho ja esta vazio.\n");
                }
                break;

            case 6:
                buscarProduto();
                break;

            case 4:
                finalizarVenda();
                break;

            case 5:
                limparCarrinho();
                printf("  Venda cancelada. Carrinho limpo.\n");
                break;

            case 0:
                if (totalItens > 0) {
                    printf("  Atencao: ha %d item(ns) no carrinho. Cancelar venda? (s/n): ", totalItens);
                    char conf;
                    scanf("%c", &conf);
                    limparBuffer();
                    if (conf == 's' || conf == 'S')
                        limparCarrinho();
                    else
                        opcao = 1; // volta ao loop
                }
                break;

            default:
                // Tenta tratar o número digitado como um código de produto direto
                if (opcao > 0) {
                    printf("  Quantidade (Enter = 1): ");
                    char buf[20];
                    fgets(buf, sizeof(buf), stdin);
                    qtd = (buf[0] == '\n') ? 1 : atoi(buf);
                    if (qtd <= 0) qtd = 1;
                    adicionarItem(opcao, qtd);
                }
                break;
        }

    } while (opcao != 0);

    printf("  Saindo do modo caixa...\n");
}

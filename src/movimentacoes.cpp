#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movimentacoes.h"
#include "estoque.h"
#include "util.h"
#include "database.h"

void gravarMovimentacao(int codigo, const char *nome, char tipo, int qtd, int saldo)
{
    char dh[20];
    obterDataHora(dh);
    
    // Insere a movimentação direto na nova tabela
    char query[500];
    sprintf(query, "INSERT INTO movimentacoes (codigo_produto, nome_produto, tipo, quantidade, saldo_atual, data_hora) VALUES (%d, '%s', '%c', %d, %d, '%s')", 
            codigo, nome, tipo, qtd, saldo, dh);
            
    if (mysql_query(conexao, query)) {
        printf("ERRO AO SALVAR MOVIMENTACAO NO BANCO: %s\n", mysql_error(conexao));
    }
}

void registrarEntrada()
{
    int codigo, indice, qtd;

    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    limparBuffer();

    indice = buscarIndicePorCodigo(codigo);
    if (indice == -1)
    {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("Produto      : %s\n", produtos[indice].nome);
    printf("Saldo atual  : %d\n", produtos[indice].quantidade);
    printf("Qtd. a entrar: ");
    scanf("%d", &qtd);
    limparBuffer();

    if (qtd <= 0)
    {
        printf("Quantidade deve ser maior que zero.\n");
        return;
    }

    produtos[indice].quantidade += qtd;

    // ATUALIZA QUANTIDADE NO BANCO
    char query[200];
    sprintf(query, "UPDATE produtos SET quantidade=%d WHERE codigo=%d;", produtos[indice].quantidade, produtos[indice].codigo);
    mysql_query(conexao, query);

    gravarMovimentacao(codigo, produtos[indice].nome, 'E', qtd, produtos[indice].quantidade);
    printf("Entrada registrada. Novo saldo: %d\n", produtos[indice].quantidade);
}

void registrarSaida()
{
    int codigo, indice, qtd;

    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    limparBuffer();

    indice = buscarIndicePorCodigo(codigo);
    if (indice == -1)
    {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("Produto      : %s\n", produtos[indice].nome);
    printf("Saldo atual  : %d\n", produtos[indice].quantidade);
    printf("Qtd. a sair  : ");
    scanf("%d", &qtd);
    limparBuffer();

    if (qtd <= 0)
    {
        printf("Quantidade deve ser maior que zero.\n");
        return;
    }
    if (qtd > produtos[indice].quantidade)
    {
        printf("Estoque insuficiente. Disponivel: %d\n", produtos[indice].quantidade);
        return;
    }

    produtos[indice].quantidade -= qtd;

    // ATUALIZA QUANTIDADE NO BANCO
    char query[200];
    sprintf(query, "UPDATE produtos SET quantidade=%d WHERE codigo=%d;", produtos[indice].quantidade, produtos[indice].codigo);
    mysql_query(conexao, query);

    gravarMovimentacao(codigo, produtos[indice].nome, 'S', qtd, produtos[indice].quantidade);
    printf("Saida registrada. Novo saldo: %d\n", produtos[indice].quantidade);
}

/* Versao direta chamada pelo modulo de caixa (PDV) */
void registrarSaidaDireta(int codigo, const char *nome, int qtd, int saldo_novo)
{
    gravarMovimentacao(codigo, nome, 'S', qtd, saldo_novo);
}

void listarMovimentacoes()
{
    if (mysql_query(conexao, "SELECT codigo_produto, nome_produto, tipo, quantidade, saldo_atual, data_hora FROM movimentacoes")) {
        printf("Erro ao buscar movimentacoes: %s\n", mysql_error(conexao));
        return;
    }
    
    MYSQL_RES *resultado = mysql_store_result(conexao);
    if (resultado == NULL) return;
    
    int encontrou = 0;

    printf("\n%-6s  %-20s  %-7s  %6s  %6s  %-16s\n", "Cod.", "Nome", "Tipo", "Qtd.", "Saldo", "Data/Hora");
    printf("%-6s  %-20s  %-7s  %6s  %6s  %-16s\n", "------", "--------------------", "-------", "------", "------", "----------------");

    MYSQL_ROW linha;
    while ((linha = mysql_fetch_row(resultado))) {
        int codigo = atoi(linha[0]);
        const char* nome = linha[1];
        char tipo = linha[2][0];
        int qtd = atoi(linha[3]);
        int saldo = atoi(linha[4]);
        const char* dh = linha[5];
        
        printf("%-6d  %-20s  %-7s  %6d  %6d  %-16s\n", codigo, nome, tipo == 'E' ? "ENTRADA" : "SAIDA", qtd, saldo, dh);
        encontrou = 1;
    }

    if (!encontrou) printf("Nenhuma movimentacao registrada no banco.\n");
    mysql_free_result(resultado);
}

void listarMovimentacoesPorProduto()
{
    int codBusca, encontrou = 0;

    printf("Codigo do produto: ");
    scanf("%d", &codBusca);
    limparBuffer();

    if (buscarIndicePorCodigo(codBusca) == -1)
    {
        printf("Produto nao encontrado.\n");
        return;
    }

    char query[200];
    sprintf(query, "SELECT tipo, quantidade, saldo_atual, data_hora FROM movimentacoes WHERE codigo_produto=%d", codBusca);
    
    if (mysql_query(conexao, query)) {
        printf("Erro ao buscar movimentacoes: %s\n", mysql_error(conexao));
        return;
    }
    
    MYSQL_RES *resultado = mysql_store_result(conexao);
    if (resultado == NULL) return;

    printf("\n%-7s  %6s  %6s  %-16s\n", "Tipo", "Qtd.", "Saldo", "Data/Hora");
    printf("%-7s  %6s  %6s  %-16s\n", "-------", "------", "------", "----------------");

    MYSQL_ROW linha;
    while ((linha = mysql_fetch_row(resultado))) {
        char tipo = linha[0][0];
        int qtd = atoi(linha[1]);
        int saldo = atoi(linha[2]);
        const char* dh = linha[3];
        
        printf("%-7s  %6d  %6d  %-16s\n", tipo == 'E' ? "ENTRADA" : "SAIDA", qtd, saldo, dh);
        encontrou = 1;
    }

    if (!encontrou) printf("Nenhuma movimentacao para este produto.\n");
    mysql_free_result(resultado);
}

#include <stdio.h>
#include "menu.h"
#include "estoque.h"
#include "categorias.h"
#include "movimentacoes.h"
#include "auth.h"
#include "util.h"
#include "caixa.h"
#include "ui.h"

void menu()
{
    int opcao;
    do
    {
        limparTela();

        /* ── CABEÇALHO ─────────────────────────────────── */
        cabecalho("SISTEMA DE CONTROLE DE ESTOQUE - V1.5");

        /* ── PRODUTOS ──────────────────────────*/
        titulo_secao("GESTAO DE PRODUTOS");
        item_menu(1, "Cadastrar Novo");
        item_menu(2, "Listar Todos");
        item_menu(3, "Buscar Codigo");
        item_menu(6, "Editar Existente");
        item_menu(10, "Excluir Produto");
        printf("\n");

        /* ── ESTOQUE ───────────────────────────*/
        titulo_secao("MOVIMENTACAO & SALDO");
        item_menu(4, "Entrada (Compra)");
        item_menu(5, "Saida (Avulsa)");
        item_menu(7, "Valor Total R$");
        printf("\n");

        /* ── RELATORIOS ──────────────────────────────────*/
        titulo_secao("AUDITORIA & LOGS");
        item_menu(8, "Historico Geral");
        item_menu(9, "Kardex p/ Item");
        printf("\n");

        /* ── SISTEMA ─────────────────────────────────────*/
        titulo_secao("CONFIGURACOES");
        item_menu(11, "Trocar Senha");
        printf("\n");

        /* ── CAIXA PDV (DESTAQUE) ────────────────────────*/
        printf("\n");
        attr(ATTR_PDV);
        printf("  [ 12 ]  >>  ACESSAR PONTO DE VENDA (CAIXA)  <<  ");
        cor_reset();
        printf("\n");

        /* ── SAIR ────────────────────────────────────────*/
        printf("\n  ");
        attr(ATTR_ERRO_BG);
        printf(" 0 . SAIR ");
        cor_reset();
        printf("\n");

        /* ── PROMPT ──────────────────────────────────────*/
        printf("\n");
        linha_horizontal('-');
        attr(ATTR_PROMPT);
        printf("  DIGITE A OPCAO DESEJADA: ");
        cor_reset();
        
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
        } else {
            limparBuffer();
        }

        switch (opcao)
        {
            case  1: cadastrarProduto();              break;
            case  2: listarProdutos();                break;
            case  3: buscarProduto();                 break;
            case  4: registrarEntrada();              break;
            case  5: registrarSaida();                break;
            case  6: editarProduto();                 break;
            case  7: mostrarValorTotal();             break;
            case  8: listarMovimentacoes();           break;
            case  9: listarMovimentacoesPorProduto(); break;
            case 10: excluirProduto();                break;
            case 11: alterarSenha();                  break;
            case 12: modoCaixa();                     break;
            case  0:
                printf("\n  Saindo...");
                break;
            default:
                attr(ATTR_ERRO_BG);
                printf("\n  [!] OPCAO INVALIDA! Tente novamente.  ");
                cor_reset();
                Sleep(1000);
        }

    } while (opcao != 0);
}

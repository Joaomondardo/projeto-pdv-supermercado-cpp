#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "database.h" // Já traz o MySQL e a conexao global

#define MAX_PRODUTOS   1000

struct Produto
{
    int   codigo;
    char  nome[100];
    float preco;
    int   quantidade;
    int   categoria_id; // Chave estrangeira!
    char  categoria[50];
};

// Variáveis globais de controle de estoque (agora expostas para outros módulos)
extern Produto produtos[MAX_PRODUTOS];
extern int total;

// Funções auxiliares de busca
int buscarIndicePorCodigo(int codigo);
bool buscarProdutoPorCodigoDB(int codigo, Produto *p);

/* ── Produtos ─────────────────────────────────────── */
void carregarProdutos();
void cadastrarProduto();
void listarProdutos();
void buscarProduto();
void editarProduto();
void excluirProduto();
void mostrarValorTotal();

/* ── Movimentacoes de estoque ─────────────────────── */
void registrarEntrada();
void registrarSaida();
void listarMovimentacoes();
void listarMovimentacoesPorProduto();

/* ── Menu ─────────────────────────────────────────── */
void menu();

#endif

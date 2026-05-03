#ifndef MOVIMENTACOES_H
#define MOVIMENTACOES_H

void registrarEntrada();
void registrarSaida();
void registrarSaidaDireta(int codigo, const char *nome, int qtd, int saldo_novo);
void listarMovimentacoes();
void listarMovimentacoesPorProduto();

#endif

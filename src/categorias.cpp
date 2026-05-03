#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "categorias.h"
#include "database.h"
#include "util.h"

Categoria categorias[MAX_CATEGORIAS];
int totalCategorias = 0;

int buscarOuAdicionarCategoria(const char *cat)
{
    int i;
    // Se já temos em memória, retorna o ID
    for (i = 0; i < totalCategorias; i++)
        if (strcmp(categorias[i].nome, cat) == 0)
            return categorias[i].id;

    // Salva a nova categoria direto no MySQL!
    char query[400];
    char catBlindada[150];
    blindarTexto(cat, catBlindada);
    
    // Agora inserimos e precisamos do ID gerado
    sprintf(query, "INSERT IGNORE INTO categorias (nome) VALUES ('%s')", catBlindada);
    mysql_query(conexao, query);

    // Pega o ID gerado pelo banco para a categoria
    int novoId = mysql_insert_id(conexao);

    if (totalCategorias < MAX_CATEGORIAS)
    {
        categorias[totalCategorias].id = novoId;
        strcpy(categorias[totalCategorias].nome, cat);
        totalCategorias++;
    }
    return novoId;
}

void carregarCategorias()
{
    totalCategorias = 0;
    // Agora puxamos o ID junto
    if (mysql_query(conexao, "SELECT id, nome FROM categorias")) return;

    MYSQL_RES *resultado = mysql_store_result(conexao);
    if (resultado == NULL) return;

    MYSQL_ROW linha;
    while ((linha = mysql_fetch_row(resultado))) {
        if (totalCategorias >= MAX_CATEGORIAS) break;
        categorias[totalCategorias].id = atoi(linha[0]);
        strncpy(categorias[totalCategorias].nome, linha[1], sizeof(categorias[totalCategorias].nome) - 1);
        categorias[totalCategorias].nome[sizeof(categorias[totalCategorias].nome) - 1] = '\0';
        totalCategorias++;
    }
    mysql_free_result(resultado);
}

/* Exibe categorias e deixa o usuario escolher ou criar uma nova */
void selecionarCategoria(int *dest_id, char *dest_nome)
{
    int i, opcao;

    if (totalCategorias == 0)
    {
        printf("Nenhuma categoria. Digite o nome da primeira: ");
        fgets(dest_nome, 50, stdin);
        dest_nome[strcspn(dest_nome, "\n")] = '\0';
        *dest_id = buscarOuAdicionarCategoria(dest_nome);
        return;
    }

    printf("\nCategorias:\n");
    for (i = 0; i < totalCategorias; i++)
        printf("  %d - %s\n", i + 1, categorias[i].nome);
    printf("  0 - Nova categoria\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao >= 1 && opcao <= totalCategorias)
    {
        *dest_id = categorias[opcao - 1].id;
        strcpy(dest_nome, categorias[opcao - 1].nome);
    }
    else
    {
        printf("Nome da nova categoria: ");
        fgets(dest_nome, 50, stdin);
        dest_nome[strcspn(dest_nome, "\n")] = '\0';
        *dest_id = buscarOuAdicionarCategoria(dest_nome);
    }
}

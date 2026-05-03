#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estoque.h"
#include "auth.h"
#include "util.h"
#include "categorias.h"
#include "ui.h"

/* ═══════════════════════════════════════════════════════
   Variaveis Globais (removido o static para os modulos acessarem)
   ═══════════════════════════════════════════════════════ */

Produto produtos[MAX_PRODUTOS];
int total = 0;

/* ═══════════════════════════════════════════════════════
   CARGA (Produtos)
   ═══════════════════════════════════════════════════════ */

void carregarProdutos()
{
    total = 0; // Zera a contagem na memória

    // Carrega categorias se existirem no arquivo antigo, para manter a consistência
    carregarCategorias();

    // ========================================================
    // NOVO CÓDIGO: Lê direto do banco de dados MySQL usando um JOIN
    if (mysql_query(conexao, "SELECT p.codigo, p.nome, p.preco, p.quantidade, p.categoria_id, c.nome FROM produtos p LEFT JOIN categorias c ON p.categoria_id = c.id")) {
        printf("ERRO AO CARREGAR DO BANCO: %s\n", mysql_error(conexao));
        return;
    }

    MYSQL_RES *resultado = mysql_store_result(conexao);
    if (resultado == NULL) return;

    MYSQL_ROW linha;
    // O fetch_row vai pegando produto por produto do banco
    while ((linha = mysql_fetch_row(resultado))) {
        if (total >= MAX_PRODUTOS) break; // Se atingir o limite da memória, para
        
        produtos[total].codigo = atoi(linha[0]);
        strcpy(produtos[total].nome, linha[1]);
        produtos[total].preco = atof(linha[2]);
        produtos[total].quantidade = atoi(linha[3]);
        produtos[total].categoria_id = atoi(linha[4]);
        
        // Pode acontecer do banco retornar NULL se a categoria foi apagada
        if (linha[5]) strcpy(produtos[total].categoria, linha[5]);
        else strcpy(produtos[total].categoria, "Desconhecida");
        
        total++;
    }
    
    // Libera a memória do resultado do banco
    mysql_free_result(resultado);
    // ========================================================
}

/* ═══════════════════════════════════════════════════════
   ORDENACAO
   ═══════════════════════════════════════════════════════ */

void ordenarPorNome()
{
    int i, j;
    Produto tmp;
    for (i = 0; i < total - 1; i++)
        for (j = 0; j < total - 1 - i; j++)
            if (strcmp(produtos[j].nome, produtos[j + 1].nome) > 0)
            {
                tmp = produtos[j];
                produtos[j] = produtos[j + 1];
                produtos[j + 1] = tmp;
            }
}

void ordenarPorCodigo()
{
    int i, j;
    Produto tmp;
    for (i = 0; i < total - 1; i++)
        for (j = 0; j < total - 1 - i; j++)
            if (produtos[j].codigo > produtos[j + 1].codigo)
            {
                tmp = produtos[j];
                produtos[j] = produtos[j + 1];
                produtos[j + 1] = tmp;
            }
}

/* ═══════════════════════════════════════════════════════
   BUSCA
   ═══════════════════════════════════════════════════════ */

int buscarIndicePorCodigo(int codigo)
{
    int i;
    for (i = 0; i < total; i++)
        if (produtos[i].codigo == codigo)
            return i;
    return -1;
}

bool buscarProdutoPorCodigoDB(int codigo, Produto *p)
{
    char query[300];
    sprintf(query, "SELECT p.codigo, p.nome, p.preco, p.quantidade, p.categoria_id, c.nome "
                   "FROM produtos p LEFT JOIN categorias c ON p.categoria_id = c.id "
                   "WHERE p.codigo = %d", codigo);

    if (mysql_query(conexao, query)) {
        printf("Erro ao buscar produto no banco: %s\n", mysql_error(conexao));
        return false;
    }

    MYSQL_RES *resultado = mysql_store_result(conexao);
    if (resultado == NULL) return false;

    MYSQL_ROW linha = mysql_fetch_row(resultado);
    if (linha) {
        p->codigo = atoi(linha[0]);
        strcpy(p->nome, linha[1]);
        p->preco = atof(linha[2]);
        p->quantidade = atoi(linha[3]);
        p->categoria_id = atoi(linha[4]);
        if (linha[5]) strcpy(p->categoria, linha[5]);
        else strcpy(p->categoria, "Desconhecida");
        
        mysql_free_result(resultado);
        return true;
    }

    mysql_free_result(resultado);
    return false;
}

/* ═══════════════════════════════════════════════════════
   CRUD DE PRODUTOS
   ═══════════════════════════════════════════════════════ */

void cadastrarProduto()
{
    int codigo;

    if (total >= MAX_PRODUTOS)
    {
        printf("Limite de %d produtos atingido.\n", MAX_PRODUTOS);
        return;
    }

    printf("Codigo: ");
    scanf("%d", &codigo);
    limparBuffer();

    if (codigo <= 0)
    {
        printf("Codigo deve ser maior que zero.\n");
        return;
    }
    if (buscarIndicePorCodigo(codigo) != -1)
    {
        printf("Ja existe um produto com o codigo %d.\n", codigo);
        return;
    }

    produtos[total].codigo = codigo;

    printf("Nome: ");
    fgets(produtos[total].nome, sizeof(produtos[total].nome), stdin);
    produtos[total].nome[strcspn(produtos[total].nome, "\n")] = '\0';

    if (strlen(produtos[total].nome) == 0)
    {
        printf("Nome nao pode ser vazio.\n");
        return;
    }

    printf("Preco: ");
    scanf("%f", &produtos[total].preco);
    limparBuffer();

    if (produtos[total].preco < 0)
    {
        printf("Preco nao pode ser negativo.\n");
        return;
    }

    printf("Quantidade inicial: ");
    scanf("%d", &produtos[total].quantidade);
    limparBuffer();

    if (produtos[total].quantidade < 0)
    {
        printf("Quantidade nao pode ser negativa.\n");
        return;
    }

    selecionarCategoria(&produtos[total].categoria_id, produtos[total].categoria);

    // ========================================================
    // NOVO CÓDIGO DO BANCO DE DADOS:
    char query[800];
    char nomeBlindado[200];
    
    // Escapa os textos para evitar quebra do SQL
    blindarTexto(produtos[total].nome, nomeBlindado);

    // Cria o comando SQL (INSERT) usando as variáveis blindadas (AGORA COM CATEGORIA_ID)
    sprintf(query, "INSERT INTO produtos (codigo, nome, preco, quantidade, categoria_id) VALUES (%d, '%s', %.2f, %d, %d);",
            produtos[total].codigo, 
            nomeBlindado, 
            produtos[total].preco, 
            produtos[total].quantidade, 
            produtos[total].categoria_id);

    // Executa o comando no banco!
    if (mysql_query(conexao, query)) {
        printf("ERRO AO SALVAR NO BANCO: %s\n", mysql_error(conexao));
    } else {
        printf("Produto cadastrado com sucesso DIRETO NO BANCO DE DADOS MySQL!\n");
        total++; // Mantém na memória só se salvou no banco
    }
    pausar();
}

void listarProdutos()
{
    int i, opcao;
    char filtro[50] = "";
    int filtrarCat = 0;

    if (total == 0)
    {
        printf("Nenhum produto cadastrado.\n");
        pausar();
        return;
    }

    printf("Filtrar por categoria? (1 = sim / outro = nao): ");
    scanf("%d", &opcao);
    limparBuffer();
    if (opcao == 1)
    {
        int filtro_id = 0;
        selecionarCategoria(&filtro_id, filtro);
        filtrarCat = 1;
    }

    printf("Ordenar: 1 - Codigo  2 - Nome  (outro = sem ordenacao): ");
    scanf("%d", &opcao);
    limparBuffer();
    if (opcao == 1) ordenarPorCodigo();
    else if (opcao == 2) ordenarPorNome();

    printf("\n%-6s  %-25s  %-15s  %8s  %6s\n",
           "Cod.", "Nome", "Categoria", "Preco", "Qtd.");
    printf("%-6s  %-25s  %-15s  %8s  %6s\n",
           "------", "-------------------------",
           "---------------", "--------", "------");

    for (i = 0; i < total; i++)
    {
        if (filtrarCat && strcmp(produtos[i].categoria, filtro) != 0)
            continue;
        printf("%-6d  %-25s  %-15s  %8.2f  %6d\n",
               produtos[i].codigo,
               produtos[i].nome,
               produtos[i].categoria,
               produtos[i].preco,
               produtos[i].quantidade);
    }
    pausar();
}

void buscarProduto()
{
    char termo[100];
    printf("\n");
    attr(ATTR_PROMPT);
    printf(" BUSCAR PRODUTO (Codigo ou Nome): ");
    cor_reset();
    
    fgets(termo, sizeof(termo), stdin);
    termo[strcspn(termo, "\n")] = '\0';

    if (strlen(termo) == 0) {
        printf("  [!] Termo de busca invalido.\n");
        pausar();
        return;
    }

    // Verifica se o usuário digitou apenas números
    int isNumeric = 1;
    for (int i = 0; termo[i] != '\0'; i++) {
        if (termo[i] < '0' || termo[i] > '9') {
            isNumeric = 0; 
            break;
        }
    }

    char query[600];
    char termoBlindado[200];
    blindarTexto(termo, termoBlindado);

    if (isNumeric) {
        sprintf(query, "SELECT p.codigo, p.nome, c.nome, p.preco, p.quantidade "
                       "FROM produtos p LEFT JOIN categorias c ON p.categoria_id = c.id "
                       "WHERE p.codigo = %d OR p.nome LIKE '%%%s%%'", atoi(termo), termoBlindado);
    } else {
        sprintf(query, "SELECT p.codigo, p.nome, c.nome, p.preco, p.quantidade "
                       "FROM produtos p LEFT JOIN categorias c ON p.categoria_id = c.id "
                       "WHERE p.nome LIKE '%%%s%%'", termoBlindado);
    }

    if (mysql_query(conexao, query)) {
        printf("  [!] ERRO AO BUSCAR NO BANCO: %s\n", mysql_error(conexao));
        return;
    }

    MYSQL_RES *resultado = mysql_store_result(conexao);
    if (resultado == NULL) return;

    if (mysql_num_rows(resultado) == 0) {
        printf("  [!] Nenhum produto encontrado com o termo \"%s\".\n", termo);
        mysql_free_result(resultado);
        pausar();
        return;
    }

    printf("\n");
    attr(ATTR_HEADER);
    printf("  %-6s | %-25s | %-15s | %8s | %6s  ", "COD.", "NOME", "CATEGORIA", "PRECO", "QTD.");
    cor_reset();
    printf("\n");

    MYSQL_ROW linha;
    while ((linha = mysql_fetch_row(resultado))) {
        printf("  %-6s | %-25s | %-15s | %8.2f | %6s\n", 
               linha[0], linha[1], linha[2] ? linha[2] : "---", atof(linha[3]), linha[4]);
    }

    mysql_free_result(resultado);
    printf("\n");
    pausar();
}

void editarProduto()
{
    int codigo, indice, opcao;
    char buf[100];
    float novoPreco;

    printf("Codigo do produto a editar: ");
    scanf("%d", &codigo);
    limparBuffer();

    indice = buscarIndicePorCodigo(codigo);
    if (indice == -1)
    {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("\nDados atuais:\n");
    printf("  Nome      : %s\n",   produtos[indice].nome);
    printf("  Categoria : %s\n",   produtos[indice].categoria);
    printf("  Preco     : %.2f\n", produtos[indice].preco);
    printf("  Quantidade: %d\n",   produtos[indice].quantidade);
    printf("\n(Enter = manter valor atual)\n\n");

    /* Nome */
    printf("Novo nome [%s]: ", produtos[indice].nome);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0)
        strcpy(produtos[indice].nome, buf);

    /* Categoria */
    printf("Alterar categoria? (1 = sim / outro = nao): ");
    scanf("%d", &opcao);
    limparBuffer();
    if (opcao == 1)
        selecionarCategoria(&produtos[indice].categoria_id, produtos[indice].categoria);

    /* Preco */
    printf("Novo preco [%.2f] (0 = manter): ", produtos[indice].preco);
    scanf("%f", &novoPreco);
    limparBuffer();
    if (novoPreco > 0)
        produtos[indice].preco = novoPreco;

    // ========================================================
    // ATUALIZA NO BANCO DE DADOS COM BLINDAGEM E FOREIGN KEY
    char query[800];
    char nomeBlindado[200];
    
    blindarTexto(produtos[indice].nome, nomeBlindado);

    sprintf(query, "UPDATE produtos SET nome='%s', categoria_id=%d, preco=%.2f WHERE codigo=%d;", 
            nomeBlindado, produtos[indice].categoria_id, produtos[indice].preco, produtos[indice].codigo);
    
    if (mysql_query(conexao, query)) {
        printf("ERRO AO ATUALIZAR NO BANCO: %s\n", mysql_error(conexao));
    } else {
        printf("Produto atualizado com sucesso no banco de dados!\n");
    }
    // ========================================================
}

void excluirProduto()
{
    int codigo, indice, i;
    char conf;

    printf("Codigo: ");
    scanf("%d", &codigo);
    limparBuffer();

    indice = buscarIndicePorCodigo(codigo);
    if (indice == -1)
    {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("Confirma exclusao de \"%s\"? (s/n): ", produtos[indice].nome);
    scanf("%c", &conf);
    limparBuffer();

    if (conf != 's' && conf != 'S')
    {
        printf("Exclusao cancelada.\n");
        return;
    }

    // ========================================================
    // EXCLUI DO BANCO DE DADOS
    char query[200];
    sprintf(query, "DELETE FROM produtos WHERE codigo=%d;", produtos[indice].codigo);
    
    if (mysql_query(conexao, query)) {
        printf("ERRO AO EXCLUIR DO BANCO: %s\n", mysql_error(conexao));
    } else {
        // Se deu certo no banco, apaga da memória também
        for (i = indice; i < total - 1; i++)
            produtos[i] = produtos[i + 1];
        total--;
        printf("Produto excluido com sucesso do banco de dados!\n");
    }
    // ========================================================
}

void mostrarValorTotal()
{
    int i;
    float totalGeral = 0;

    if (total == 0)
    {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("\n%-25s  %-15s  %8s  %6s  %12s\n",
           "Nome", "Categoria", "Preco", "Qtd.", "Subtotal");
    printf("%-25s  %-15s  %8s  %6s  %12s\n",
           "-------------------------", "---------------",
           "--------", "------", "------------");

    for (i = 0; i < total; i++)
    {
        float sub = produtos[i].preco * produtos[i].quantidade;
        printf("%-25s  %-15s  %8.2f  %6d  %12.2f\n",
               produtos[i].nome, produtos[i].categoria,
               produtos[i].preco, produtos[i].quantidade, sub);
        totalGeral += sub;
    }

    printf("%-25s  %-15s  %8s  %6s  %12.2f\n",
           "TOTAL", "", "", "", totalGeral);
}


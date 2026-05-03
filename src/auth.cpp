#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auth.h"
#include "database.h"
#include "util.h"

char usuarioLogado[50] = "";
char usuarioNivel[15] = "CAIXA";

int verificarSenha()
{
    char login[50];
    char tentativa[50];
    char armazenada[50] = "";
    int  t;

    // Verifica se existem usuários no banco
    if (mysql_query(conexao, "SELECT COUNT(*) FROM usuarios")) {
        printf("Erro ao verificar banco de dados.\n");
        return 0;
    }
    MYSQL_RES *res_count = mysql_store_result(conexao);
    MYSQL_ROW row_count = mysql_fetch_row(res_count);
    int total_users = atoi(row_count[0]);
    mysql_free_result(res_count);

    // Se o banco está vazio, cria o primeiro Gerente
    if (total_users == 0)
    {
        printf("=== CONFIGURACAO DE PRIMEIRO ACESSO (GERENTE) ===\n");
        printf("Defina o login do Gerente: ");
        fgets(login, sizeof(login), stdin);
        login[strcspn(login, "\n")] = '\0';

        do {
            printf("Defina a senha (max 5 caracteres): ");
            fgets(armazenada, sizeof(armazenada), stdin);
            armazenada[strcspn(armazenada, "\n")] = '\0';
            
            if (strlen(armazenada) > 5) printf("Maximo 5 caracteres!\n");
        } while (strlen(armazenada) > 5 || strlen(armazenada) == 0);

        char query[1024];
        char senhaBlindada[300];
        char loginBlindado[150];
        blindarTexto(armazenada, senhaBlindada);
        blindarTexto(login, loginBlindado);

        snprintf(query, sizeof(query), "INSERT INTO usuarios (login, senha, nivel) VALUES ('%s', '%s', 'GERENTE')", loginBlindado, senhaBlindada);
        if (mysql_query(conexao, query)) {
            printf("Erro ao criar usuario mestre.\n");
            return 0;
        }

        printf("Gerente criado com sucesso! Use este login para acessar.\n\n");
    }

    // Fluxo de Login Normal
    for (t = 3; t > 0; t--)
    {
        printf("\n--- LOGIN DO SISTEMA ---\n");
        printf("Usuario: ");
        fgets(login, sizeof(login), stdin);
        login[strcspn(login, "\n")] = '\0';

        printf("Senha: ");
        fgets(tentativa, sizeof(tentativa), stdin);
        tentativa[strcspn(tentativa, "\n")] = '\0';

        char loginBlindado[150];
        char tentativaBlindada[300];
        blindarTexto(login, loginBlindado);
        blindarTexto(tentativa, tentativaBlindada);

        char query[512];
        snprintf(query, sizeof(query), "SELECT senha, nivel FROM usuarios WHERE login='%s'", loginBlindado);
        
        if (mysql_query(conexao, query)) {
            printf("Erro ao buscar usuario.\n");
            continue;
        }

        MYSQL_RES *res = mysql_store_result(conexao);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row && strcmp(tentativaBlindada, row[0]) == 0) {
            strcpy(usuarioLogado, login);
            strcpy(usuarioNivel, row[1]);
            printf("Acesso permitido como %s.\n\n", usuarioNivel);
            mysql_free_result(res);
            return 1;
        }
        
        if (res) mysql_free_result(res);
        printf("Login ou senha incorretos. (%d tentativas restantes)\n", t - 1);
    }

    printf("Acesso bloqueado.\n");
    return 0;
}

void alterarSenha()
{
    char atual[50], nova[50], confirmacao[50], tentativa[50];

    // Busca a senha atual do usuario logado no banco
    char query_sel[512];
    char loginBlindado[150];
    blindarTexto(usuarioLogado, loginBlindado);
    snprintf(query_sel, sizeof(query_sel), "SELECT senha FROM usuarios WHERE login='%s'", loginBlindado);

    if (mysql_query(conexao, query_sel)) {
        printf("Erro ao consultar banco de dados.\n");
        return;
    }

    MYSQL_RES *resultado = mysql_store_result(conexao);
    if (resultado == NULL || mysql_num_rows(resultado) == 0) {
        printf("Usuario nao encontrado.\n");
        if (resultado) mysql_free_result(resultado);
        return;
    }

    MYSQL_ROW linha = mysql_fetch_row(resultado);
    strcpy(atual, linha[0]);
    mysql_free_result(resultado);

    printf("Senha atual: ");
    fgets(tentativa, sizeof(tentativa), stdin);
    tentativa[strcspn(tentativa, "\n")] = '\0';

    char tentativaBlindada[300];
    blindarTexto(tentativa, tentativaBlindada);

    if (strcmp(tentativaBlindada, atual) != 0)
    {
        printf("Senha incorreta.\n");
        return;
    }

    do {
        printf("Nova senha (max 5 caracteres): ");
        fgets(nova, sizeof(nova), stdin);
        nova[strcspn(nova, "\n")] = '\0';

        if (strlen(nova) > 5) {
            printf("A senha so pode ter no maximo 5 caracteres. Tente novamente.\n");
        } else if (strlen(nova) == 0) {
            printf("A senha nao pode ser vazia.\n");
        }
    } while (strlen(nova) > 5 || strlen(nova) == 0);

    printf("Confirme a nova senha: ");
    fgets(confirmacao, sizeof(confirmacao), stdin);
    confirmacao[strcspn(confirmacao, "\n")] = '\0';

    if (strcmp(nova, confirmacao) != 0)
    {
        printf("As senhas nao coincidem.\n");
        return;
    }

    // Grava a nova senha no banco de dados com protecao
    char query_upd[1024];
    char novaBlindada[300];
    blindarTexto(nova, novaBlindada);
    snprintf(query_upd, sizeof(query_upd), "UPDATE usuarios SET senha='%s' WHERE login='%s'", novaBlindada, loginBlindado);
    
    if (mysql_query(conexao, query_upd)) {
        printf("Erro ao salvar nova senha no banco.\n");
        return;
    }

    printf("Senha alterada com sucesso no Banco de Dados!\n");
}

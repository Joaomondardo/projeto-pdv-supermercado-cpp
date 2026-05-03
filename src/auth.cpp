#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "database.h" // Importamos o banco para poder ler a senha!

int verificarSenha()
{
    char armazenada[50] = "";
    char tentativa[50];
    int  t;

    // Busca a senha do usuário 'admin' no banco
    if (mysql_query(conexao, "SELECT senha FROM usuarios WHERE login='admin'")) {
        printf("Erro ao buscar senha no banco de dados: %s\n", mysql_error(conexao));
        return 0;
    }

    MYSQL_RES *resultado = mysql_store_result(conexao);

    // Se o banco está vazio (não encontrou o admin), é o primeiro acesso
    if (resultado == NULL || mysql_num_rows(resultado) == 0)
    {
        if (resultado) mysql_free_result(resultado);

        /* Primeiro acesso: definir senha */
        printf("=== PRIMEIRO ACESSO ===\n");
        do {
            printf("Defina uma senha para o sistema (max 5 caracteres): ");
            fgets(armazenada, sizeof(armazenada), stdin);
            armazenada[strcspn(armazenada, "\n")] = '\0';
            
            if (strlen(armazenada) > 5) {
                printf("A senha so pode ter no maximo 5 caracteres. Tente novamente.\n");
            } else if (strlen(armazenada) == 0) {
                printf("A senha nao pode ser vazia.\n");
            }
        } while (strlen(armazenada) > 5 || strlen(armazenada) == 0);

        char query[1024];
        char senhaBlindada[300];
        blindarTexto(armazenada, senhaBlindada);
        snprintf(query, sizeof(query), "INSERT INTO usuarios (login, senha) VALUES ('admin', '%s')", senhaBlindada);
        if (mysql_query(conexao, query)) {
            printf("Erro ao salvar a senha no banco.\n");
            return 0;
        }

        printf("Senha definida! Bem-vindo ao sistema.\n\n");
        return 1;
    }

    // Se encontrou, lê a senha armazenada
    MYSQL_ROW linha = mysql_fetch_row(resultado);
    strcpy(armazenada, linha[0]);
    mysql_free_result(resultado);

    for (t = 3; t > 0; t--)
    {
        printf("Senha (%d tentativa%s restante%s): ",
               t, t == 1 ? "" : "s", t == 1 ? "" : "s");
        fgets(tentativa, sizeof(tentativa), stdin);
        tentativa[strcspn(tentativa, "\n")] = '\0';

        char tentativaBlindada[300];
        blindarTexto(tentativa, tentativaBlindada);

        if (strcmp(tentativaBlindada, armazenada) == 0)
        {
            printf("Acesso permitido.\n\n");
            return 1;
        }
        printf("Senha incorreta.\n");
    }

    printf("Acesso bloqueado. Encerrando.\n");
    return 0;
}

void alterarSenha()
{
    char atual[50], nova[50], confirmacao[50], tentativa[50];

    // Busca a senha atual no banco
    if (mysql_query(conexao, "SELECT senha FROM usuarios WHERE login='admin'")) {
        printf("Erro ao consultar banco de dados.\n");
        return;
    }

    MYSQL_RES *resultado = mysql_store_result(conexao);
    if (resultado == NULL || mysql_num_rows(resultado) == 0) {
        printf("Nenhum usuario encontrado no banco.\n");
        if (resultado) mysql_free_result(resultado);
        return;
    }

    MYSQL_ROW linha = mysql_fetch_row(resultado);
    strcpy(atual, linha[0]);
    mysql_free_result(resultado);

    printf("Senha atual: ");
    fgets(tentativa, sizeof(tentativa), stdin);
    tentativa[strcspn(tentativa, "\n")] = '\0';

    if (strcmp(tentativa, atual) != 0)
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

    // Grava a nova senha no banco de dados com protecao contra SQL Injection!
    char query[1024];
    char novaBlindada[300];
    blindarTexto(nova, novaBlindada);
    snprintf(query, sizeof(query), "UPDATE usuarios SET senha='%s' WHERE login='admin'", novaBlindada);
    if (mysql_query(conexao, query)) {
        printf("Erro ao salvar nova senha no banco.\n");
        return;
    }

    printf("Senha alterada com sucesso no Banco de Dados!\n");
}

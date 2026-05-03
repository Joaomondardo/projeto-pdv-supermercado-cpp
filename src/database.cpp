#include <iostream>
#include "database.h"

using namespace std;

MYSQL* conexao; // Definição da variável global

bool conectarBanco() {
    conexao = mysql_init(NULL);

    if (conexao == NULL) {
        cout << "Erro ao inicializar o MySQL." << endl;
        return false;
    }

    if (mysql_real_connect(conexao, "localhost", "programa", "1234", "estoque_supermercado", 3306, NULL, 0)) {
        cout << "SUCESSO! Conectado ao banco de dados MySQL!" << endl;
        return true;
    } else {
        cout << "Falha ao conectar: " << mysql_error(conexao) << endl;
        return false;
    }
}

void fecharBanco() {
    if (conexao) {
        mysql_close(conexao);
    }
}

void blindarTexto(const char* entrada, char* saida) {
    // Usa a função oficial do MySQL para colocar uma barra invertida antes de 
    // caracteres perigosos como aspas (') e aspas duplas (")
    mysql_real_escape_string(conexao, saida, entrada, strlen(entrada));
}

#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>

// Variável de conexão global
extern MYSQL* conexao;

// Funções de gerenciamento do banco
bool conectarBanco();
void fecharBanco();

// Prevenção contra SQL Injection
void blindarTexto(const char* entrada, char* saida);

#endif

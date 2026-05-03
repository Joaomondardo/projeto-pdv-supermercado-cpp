#ifndef AUTH_H
#define AUTH_H

// Funções de autenticação do sistema
// Sessão do usuário logado
extern char usuarioLogado[50];
extern char usuarioNivel[15];

int verificarSenha();
void alterarSenha();

#endif

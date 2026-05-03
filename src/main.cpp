#include <iostream>
#include <windows.h>
#include "database.h" // Importa o nosso novo modulo de banco!
#include "estoque.h"
#include "auth.h"
#include "menu.h" // Incluindo o novo menu!

using namespace std;

int main() {
    // 1º Passo: Conectar ao banco de dados ANTES (pois a senha agora está lá!)
    if (!conectarBanco()) {
        system("pause");
        return 1;
    }

    // 2º Passo: Verificar a senha
    if (!verificarSenha()) {
        system("pause");
        fecharBanco();
        return 1;
    }

    system("pause");
    
    // 3º Passo: Carregar e rodar o menu do sistema!
    carregarProdutos();
    menu();
    
    // 4º Passo: Desconectar do banco de dados ao sair
    fecharBanco(); 
    return 0;
}

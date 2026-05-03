# 🛒 Sistema de PDV & Controle de Estoque (C++ + MySQL)

Este é um projeto experimental de um sistema de Ponto de Venda (PDV) e Gestão de Estoque para supermercados. O sistema foi desenvolvido em **C++** com integração direta ao banco de dados **MySQL**, focando em uma interface de terminal moderna, funcional e intuitiva.

> [!NOTE]
> Este projeto foi desenvolvido para fins de aprendizado e diversão, utilizando **Inteligência Artificial** como assistente de codificação para explorar integrações complexas e design de interface em modo texto.

---

## ✨ Funcionalidades

- **Módulo PDV (Caixa):** Venda de itens com busca por código, cálculo automático de subtotal, total e troco.
- **Gestão de Estoque:** Cadastro, edição, exclusão e listagem de produtos com filtros.
- **Integração SQL:** Persistência real de dados. O estoque é atualizado automaticamente no banco após cada venda.
- **Relatórios & Auditoria:** Histórico de movimentações (Kardex) para controle de entradas e saídas.
- **Interface Moderna:** Uso de cores, tabelas formatadas e feedbacks visuais para uma melhor experiência do usuário.

---

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** C++ (Padrão C++11 ou superior)
- **Banco de Dados:** MySQL Server 8.0
- **Driver de Conexão:** MySQL Connector/C (libmysql)
- **Ambiente:** Windows (com suporte a cores no CMD/PowerShell)

---

## 🚀 Como Executar o Projeto

Para rodar este projeto em sua máquina, siga os passos abaixo:

### 1. Pré-requisitos
*   Ter o **MySQL Server** instalado e rodando.
*   Um compilador C++ (recomendado **MinGW/g++**).
*   As bibliotecas de desenvolvimento do MySQL instaladas (headers e libmysql).

### 2. Configuração do Banco de Dados
No seu terminal MySQL ou MySQL Workbench, execute os scripts na seguinte ordem:
1.  `schema.sql` (Cria o banco e as tabelas).
2.  `seed.sql` (Insere dados de teste para você poder usar o sistema imediatamente).

### 3. Configuração de Credenciais
Antes de compilar, ajuste as credenciais de acesso ao seu banco de dados no arquivo:
`src/database.cpp`

```cpp
// Altere para o seu usuário e senha do MySQL
if (mysql_real_connect(conexao, "localhost", "SEU_USUARIO", "SUA_SENHA", "estoque_supermercado", 3306, NULL, 0))
```

### 4. Compilação
Você pode compilar o projeto utilizando o comando abaixo (ajuste os caminhos se necessário):

```bash
g++ src/*.cpp -I "C:\Program Files\MySQL\MySQL Server 8.0\include" -L "C:\Program Files\MySQL\MySQL Server 8.0\lib" -lmysql -o estoque.exe
```

---

## 📸 Demonstração do Visual
O sistema utiliza um sistema de cores customizado para facilitar a leitura:
- **Verde:** Sucesso e Entradas.
- **Vermelho:** Erros e Saídas de estoque.
- **Ciano/Amarelo:** Prompts e Cabeçalhos.

---

## 🤝 Contribuição
Sinta-se à vontade para clonar, testar e sugerir melhorias! Este é um projeto "playground" para ver até onde podemos levar a automação e organização de um sistema comercial simples.

---
**Desenvolvido por João Mondardo** (Com auxílio de IA)

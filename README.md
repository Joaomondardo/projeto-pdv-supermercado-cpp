# Sistema PDV & Controle de Estoque Supermercado (C++/MySQL/Docker)

Este é um projeto experimental é de um sistema de Ponto de Venda (PDV) e Gestão de Estoque profissional para supermercados. O sistema foi desenvolvido em **C++** com integração robusta ao banco de dados **MySQL**, utilizando **Docker** para infraestrutura e focando em uma interface de terminal moderna, segura e intuitiva.

> [!NOTE]
> Este projeto foi desenvolvido para fins de aprendizado e diversão, utilizando **IA** como assistente de codificação para explorar integrações complexas, design de interface em modo texto e segurança de dados.

---

## Funcionalidades Principais

- **Módulo PDV (Caixa):** Interface otimizada para vendas rápidas, cálculo automático de subtotal, total e troco.
- **Gestão de Estoque:** CRUD completo (Cadastro, Edição, Exclusão e Listagem) com suporte a categorias e filtros.
- **Persistência Real (MySQL):** Todos os dados são salvos em um banco de dados relacional. O estoque é atualizado em tempo real após cada venda.
- **Auditoria & Logs:** Histórico completo de todas as movimentações de entrada e saída (Kardex).
- **Segurança de Dados:** Proteção contra **SQL Injection** em todos os campos de entrada e tratamento de estouro de buffer para maior estabilidade.
- **Interface Moderna:** Uso de cores dinâmicas, tabelas formatadas e suporte a localidade (R$ e separadores decimais).

---

## Tecnologias Utilizadas

- **Linguagem:** C++17 (Compilador MinGW-w64)
- **Banco de Dados:** MySQL Server 8.0
- **Infraestrutura:** Docker & Docker Compose (MySQL Conteinerizado)
- **Driver:** MySQL Connector/C (libmysql)

---

## Como Rodar o Projeto

A maneira mais fácil e recomendada de rodar o projeto é utilizando o **Docker**.

### 1. Pré-requisitos
*   **Docker Desktop** instalado.
*   **VS Code** com a extensão de C++ instalada.
*   Compilador **MinGW-w64** configurado (ou o caminho ajustado no `tasks.json`).

### 2. Subindo o Banco de Dados
No terminal da raiz do projeto, execute:
```bash
docker-compose up -d
```
*Isso criará o container MySQL, configurará as tabelas (`schema.sql`) e inserirá os dados de teste (`seed.sql`) automaticamente na porta 3306.*

### 3. Compilação e Execução
O projeto já vem com as **Tasks do VS Code** configuradas:
1.  Aperte `Ctrl + Shift + B` para compilar o projeto.
2.  O executável será gerado na pasta `build/estoque.exe`.
3.  Para rodar, digite no terminal: `.\build\estoque.exe` ou aperte `F5`.

---

## Destaques Técnicos

Durante o desenvolvimento, o sistema foi refinado para seguir boas práticas de engenharia:
- **Tratamento de Locale:** Forçado para o padrão "C" para garantir que o MySQL receba decimais com ponto (`.`), independentemente do idioma do Windows.
- **Blindagem de SQL:** Uso de `mysql_real_escape_string` em todas as entradas de usuário.
- **Robustez:** Proteção contra entradas de caracteres inválidos (letras em campos numéricos) para evitar loops infinitos.

---

**Desenvolvido por João Vitor Mondardo dos Santos**

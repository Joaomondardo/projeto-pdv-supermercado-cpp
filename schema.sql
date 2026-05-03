CREATE DATABASE IF NOT EXISTS estoque_supermercado;
USE estoque_supermercado;

-- Criação do usuário do sistema
CREATE USER IF NOT EXISTS 'programa'@'localhost' IDENTIFIED BY '1234';
GRANT ALL PRIVILEGES ON estoque_supermercado.* TO 'programa'@'localhost';
FLUSH PRIVILEGES;

-- Criação da tabela de categorias de produtos
CREATE TABLE IF NOT EXISTS categorias (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(50) UNIQUE
);

-- Criação da tabela de produtos (Agora usando Foreign Key)
CREATE TABLE IF NOT EXISTS produtos (
    codigo INT PRIMARY KEY,
    nome VARCHAR(100),
    preco FLOAT,
    quantidade INT,
    categoria_id INT,
    FOREIGN KEY (categoria_id) REFERENCES categorias(id)
);

-- Criação da tabela de movimentações (log de estoque)
CREATE TABLE IF NOT EXISTS movimentacoes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    codigo_produto INT,
    nome_produto VARCHAR(100),
    tipo CHAR(1),
    quantidade INT,
    saldo_atual INT,
    data_hora VARCHAR(20)
);

-- Criação da tabela de usuários do sistema
CREATE TABLE IF NOT EXISTS usuarios (
    id INT AUTO_INCREMENT PRIMARY KEY,
    login VARCHAR(50) UNIQUE,
    senha VARCHAR(5)
);

-- Tabela de vendas (cabecalho de cada venda no PDV)
CREATE TABLE IF NOT EXISTS vendas (
    id INT AUTO_INCREMENT PRIMARY KEY,
    data_hora VARCHAR(20),
    total FLOAT,
    forma_pagamento VARCHAR(20)
);

-- Tabela de itens de cada venda (auditoria completa)
CREATE TABLE IF NOT EXISTS itens_venda (
    id INT AUTO_INCREMENT PRIMARY KEY,
    venda_id INT,
    codigo_produto INT,
    nome_produto VARCHAR(100),
    quantidade INT,
    preco_unit FLOAT,
    subtotal FLOAT,
    FOREIGN KEY (venda_id) REFERENCES vendas(id)
);

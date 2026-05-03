-- ============================================================
-- SEED DATA - Supermercado Controle de Estoque
-- Execute APÓS o schema.sql
-- ============================================================
USE estoque_supermercado;

-- ============================================================
-- CATEGORIAS
-- ============================================================
INSERT IGNORE INTO categorias (nome) VALUES
('Carnes e Aves'),
('Hortifruti'),
('Laticinios e Frios'),
('Padaria e Confeitaria'),
('Bebidas'),
('Mercearia e Graos'),
('Higiene Pessoal'),
('Limpeza'),
('Congelados'),
('Snacks e Guloseimas'),
('Bebidas Alcoolicas'),
('Pet Shop'),
('Bebe e Kids');

-- ============================================================
-- PRODUTOS
-- ============================================================
-- Carnes e Aves (id=1)
INSERT IGNORE INTO produtos (codigo, nome, preco, quantidade, categoria_id) VALUES
(1001, 'Picanha Bovina kg',          49.90, 80,  1),
(1002, 'Alcatra Bovina kg',          39.90, 120, 1),
(1003, 'Fraldinha Bovina kg',        34.90, 90,  1),
(1004, 'Costela Bovina kg',          28.90, 60,  1),
(1005, 'Carne Moida Patinho kg',     24.90, 150, 1),
(1006, 'File de Frango kg',          18.90, 200, 1),
(1007, 'Coxa e Sobrecoxa kg',        12.90, 180, 1),
(1008, 'Peito de Peru Defumado kg',  32.90, 50,  1),
(1009, 'Linguica Calabresa kg',      22.90, 100, 1),
(1010, 'Lombo Suino kg',             27.90, 70,  1),
(1011, 'Costelinha Suina kg',        25.90, 65,  1),
(1012, 'Salsicha Frankfurt pct',      9.90, 140, 1),
(1013, 'File de Tilapia kg',         24.90, 80,  1),
(1014, 'Camarao Cinza kg',           45.90, 40,  1),

-- Hortifruti (id=2)
(2001, 'Banana Prata kg',             3.99, 300, 2),
(2002, 'Maca Gala un',                2.49, 400, 2),
(2003, 'Laranja Bahia kg',            4.99, 250, 2),
(2004, 'Uva Italia kg',               8.99, 120, 2),
(2005, 'Mamao Papaia un',             5.49, 90,  2),
(2006, 'Morango kg',                 12.99, 70,  2),
(2007, 'Tomate Salada kg',            4.99, 200, 2),
(2008, 'Cebola kg',                   3.99, 300, 2),
(2009, 'Alho kg',                    14.99, 100, 2),
(2010, 'Batata Inglesa kg',           4.49, 250, 2),
(2011, 'Cenoura kg',                  3.49, 200, 2),
(2012, 'Abobrinha kg',                4.99, 150, 2),
(2013, 'Brocolis un',                 5.99, 100, 2),
(2014, 'Alface Crespa un',            2.49, 180, 2),
(2015, 'Couve Manteiga maco',         2.99, 150, 2),

-- Laticinios e Frios (id=3)
(3001, 'Leite Integral 1L',           4.89, 500, 3),
(3002, 'Leite Desnatado 1L',          4.99, 350, 3),
(3003, 'Queijo Mussarela kg',        39.90, 80,  3),
(3004, 'Queijo Prato kg',            36.90, 70,  3),
(3005, 'Queijo Minas Frescal kg',    22.90, 90,  3),
(3006, 'Manteiga com Sal 200g',       8.99, 200, 3),
(3007, 'Creme de Leite 200ml',        3.99, 300, 3),
(3008, 'Requeijao Cremoso 200g',      6.49, 250, 3),
(3009, 'Iogurte Natural 200g',        2.99, 400, 3),
(3010, 'Iogurte Grego Morango 100g',  3.99, 200, 3),
(3011, 'Presunto Cozido kg',         24.90, 60,  3),
(3012, 'Salame Italiano kg',         48.90, 40,  3),
(3013, 'Nata 500g',                   7.99, 120, 3),

-- Padaria e Confeitaria (id=4)
(4001, 'Pao Frances kg',              9.99, 200, 4),
(4002, 'Pao de Forma Integral 500g',  7.49, 150, 4),
(4003, 'Bolo de Chocolate fatio',     4.99, 80,  4),
(4004, 'Croissant un',                3.49, 120, 4),
(4005, 'Biscoito Maisena 400g',       4.99, 200, 4),
(4006, 'Biscoito Recheado 130g',      2.99, 300, 4),
(4007, 'Torrada Integral 150g',       5.49, 180, 4),

-- Bebidas (id=5)
(5001, 'Agua Mineral 1,5L',           2.49, 600, 5),
(5002, 'Coca-Cola 2L',                8.99, 400, 5),
(5003, 'Guarana Antarctica 2L',       7.99, 350, 5),
(5004, 'Suco de Laranja Del Valle 1L',8.99, 200, 5),
(5005, 'Cha Gelado Limao 1,5L',       6.49, 180, 5),
(5006, 'Refri Lata 350ml',            3.99, 500, 5),
(5007, 'Agua de Coco 1L',             5.99, 150, 5),
(5008, 'Energetico Monster 473ml',   10.99, 200, 5),

-- Mercearia e Graos (id=6)
(6001, 'Arroz Branco 5kg',           24.90, 300, 6),
(6002, 'Feijao Carioca 1kg',          8.99, 400, 6),
(6003, 'Feijao Preto 1kg',            9.49, 250, 6),
(6004, 'Acucar Cristal 1kg',          4.99, 500, 6),
(6005, 'Sal Refinado 1kg',            2.49, 400, 6),
(6006, 'Oleo de Soja 900ml',          7.99, 350, 6),
(6007, 'Azeite Extra Virgem 500ml',  22.90, 150, 6),
(6008, 'Macarrao Espaguete 500g',     4.49, 400, 6),
(6009, 'Farinha de Trigo 1kg',        4.99, 300, 6),
(6010, 'Farinha de Mandioca 500g',    3.99, 200, 6),
(6011, 'Molho de Tomate 340g',        3.49, 500, 6),
(6012, 'Extrato de Tomate 140g',      2.49, 400, 6),
(6013, 'Atum em Lata 170g',           6.99, 300, 6),
(6014, 'Sardinha em Lata 125g',       4.49, 250, 6),
(6015, 'Milho em Lata 200g',          3.99, 350, 6),
(6016, 'Ervilha em Lata 200g',        3.99, 350, 6),
(6017, 'Cafe Pilao 500g',            14.99, 200, 6),
(6018, 'Cafe Soluvel Nescafe 50g',   12.99, 180, 6),

-- Higiene Pessoal (id=7)
(7001, 'Sabonete Dove 90g',           3.99, 400, 7),
(7002, 'Shampoo Pantene 400ml',      18.99, 200, 7),
(7003, 'Condicionador Pantene 400ml',18.99, 150, 7),
(7004, 'Creme Dental Colgate 90g',    4.99, 500, 7),
(7005, 'Escova Dental Colgate',       5.99, 400, 7),
(7006, 'Fio Dental 50m',              4.49, 300, 7),
(7007, 'Desodorante Rexona 150ml',   12.99, 300, 7),
(7008, 'Papel Higienico 4un',         9.99, 400, 7),
(7009, 'Absorvente Sempre Livre 8un', 7.99, 200, 7),
(7010, 'Aparelho de Barbear 2un',     5.99, 250, 7),

-- Limpeza (id=8)
(8001, 'Detergente Ype 500ml',        2.99, 500, 8),
(8002, 'Sabao em Po Ariel 2kg',      24.90, 200, 8),
(8003, 'Agua Sanitaria 1L',           3.49, 400, 8),
(8004, 'Desinfetante Pinho Sol 500ml',5.99, 300, 8),
(8005, 'Esponja de Aco 3un',          3.49, 350, 8),
(8006, 'Rodo Sanfonado Bettanin',    22.90, 100, 8),
(8007, 'Saco de Lixo 50L pct',        8.99, 300, 8),
(8008, 'Amaciante Downy 1L',          9.99, 200, 8),

-- Congelados (id=9)
(9001, 'Pizza Sadia Mussarela',      18.90, 150, 9),
(9002, 'Lasanha Bolonhesa 600g',     19.90, 120, 9),
(9003, 'Nuggets de Frango 300g',     12.90, 200, 9),
(9004, 'Hamburguer Bovino 672g',     19.90, 180, 9),
(9005, 'Batata Palito Pre-Frita 1kg', 9.90, 200, 9),
(9006, 'Sorvete Kibon 1,5L',         19.90, 100, 9),
(9007, 'Peixe Empanado 400g',        14.90, 150, 9),

-- Snacks e Guloseimas (id=10)
(10001,'Chocolate Lacta ao Leite 80g', 5.99, 400, 10),
(10002,'Bis Chocolate 100g',           4.99, 350, 10),
(10003,'Bala Fini Sortida 500g',       9.99, 200, 10),
(10004,'Salgadinho Ruffles 96g',       5.99, 300, 10),
(10005,'Amendoim Crocante 500g',       8.99, 250, 10),
(10006,'Pipoca de Micro 100g',         3.99, 300, 10),
(10007,'Pipoca Doce 50g',              2.49, 400, 10),

-- Bebidas Alcoolicas (id=11)
(11001,'Cerveja Heineken Lata 350ml',  4.99, 500, 11),
(11002,'Cerveja Brahma 600ml',         5.99, 400, 11),
(11003,'Vinho Tinto Seco 750ml',      35.90, 100, 11),
(11004,'Espumante Chandon 750ml',     79.90, 50,  11),
(11005,'Vodka Smirnoff 998ml',        49.90, 80,  11),
(11006,'Pinga 51 1L',                 22.90, 120, 11),
(11007,'Whisky Jack Daniels 1L',     149.90, 40,  11),

-- Pet Shop (id=12)
(12001,'Racao Golden Gato 3kg',       49.90, 100, 12),
(12002,'Racao Pedigree Adulto 15kg',  99.90, 60,  12),
(12003,'Areia Sanitaria 4kg',         19.90, 150, 12),
(12004,'Petisco Ossinho para Cao',     5.99, 200, 12),
(12005,'Shampoo Sanol Dog 500ml',     14.99, 100, 12),

-- Bebe e Kids (id=13)
(13001,'Fraldas Pampers P pct',       59.90, 100, 13),
(13002,'Fraldas Huggies M pct',       64.90, 100, 13),
(13003,'Leite Nan Infantil 800g',     79.90, 60,  13),
(13004,'Papa de Fruta Nestle 120g',    4.99, 200, 13),
(13005,'Lencos Umedecidos 100un',     17.90, 150, 13);

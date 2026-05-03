#ifndef CATEGORIAS_H
#define CATEGORIAS_H

#define MAX_CATEGORIAS  20

struct Categoria {
    int id;
    char nome[50];
};

extern Categoria categorias[MAX_CATEGORIAS];
extern int totalCategorias;

int buscarOuAdicionarCategoria(const char *cat);
void carregarCategorias();
void selecionarCategoria(int *dest_id, char *dest_nome);

#endif

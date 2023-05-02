#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// alocam memorie pentru qcell(element al cozii)
qcell *initQCell(node *elem) {
    qcell *p = (qcell *)malloc(sizeof(qcell));
    p->elem = elem;
    p->next = NULL;
    return p;
}

// initializam coada
queue *initQ() {
    queue *q = (queue *)malloc(sizeof(queue));
    q->front = q->tail = NULL;
    return q;
}

// verificam daca coada este goala
int emptyQ(queue *q) {
    if (q->front == NULL) return 1;
    return 0;
    // 1 = empty
}

// stergem din coada
void popQ(queue *q) {
    qcell *u = q->front;
    q->front = u->next;

    // eliberam memoria alocata pentru qcell
    free(u);
}

// adaugam in coada
void addQ(queue *q, node *elem) {
    // cream un nou element de adaugat in coada
    qcell *nou = initQCell(elem);

    // verificam daca este primul element de adaugat in coada
    if (q->front == NULL) {
        q->front = nou;
        q->tail = nou;
    } else {
        q->tail->next = nou;
        q->tail = nou;
    }
}

// calculam folosind formula data pentru media valorilor unui bloc
ull med_red(pixel **grid, unsigned int size, unsigned int x, unsigned int y) {
    ull red = 0;
    unsigned int i, j;
    for (i = x; i < x + size; ++i) {
        for (j = y; j < y + size; ++j) {
            red += grid[i][j].red;
        }
    }
    return red / (size * size);
}

ull med_green(pixel **grid, unsigned int size, unsigned int x, unsigned int y) {
    ull green = 0;
    unsigned int i, j;
    for (i = x; i < x + size; ++i) {
        for (j = y; j < y + size; ++j) {
            green += grid[i][j].green;
        }
    }
    return green / (size * size);
}

ull med_blue(pixel **grid, unsigned int size, unsigned int x, unsigned int y) {
    ull blue = 0;
    unsigned int i, j;
    for (i = x; i < x + size; ++i) {
        for (j = y; j < y + size; ++j) {
            blue += grid[i][j].blue;
        }
    }
    return blue / (size * size);
}

// aflam scorul similaritatii
ull mean_func(pixel **grid, unsigned int size, unsigned int x, unsigned int y,
              ull red, ull green, ull blue) {
    ull mean = 0;
    unsigned int i, j;
    for (i = x; i < x + size; ++i) {
        for (j = y; j < y + size; ++j) {
            mean += (red - grid[i][j].red) * (red - grid[i][j].red);
            mean += (green - grid[i][j].green) * (green - grid[i][j].green);
            mean += (blue - grid[i][j].blue) * (blue - grid[i][j].blue);
        }
    }
    mean = mean / (3 * size * size);

    return mean;
}

// alocam memorie pentru o variabila de tip node
node *create_node() {
    node *new_node = (node *)malloc(sizeof(node));
    new_node->tip = 0;
    (new_node->value).blue = (new_node->value).red = (new_node->value).green =
        0;
    new_node->unu = new_node->doi = new_node->trei = new_node->patru = NULL;
    return new_node;
}

// cream arborele cuaternar
void create_arb(pixel **grid, unsigned int size, unsigned int x, unsigned int y,
                node **root, ull prag, unsigned int *size_maxim) {
    ull red = med_red(grid, size, x, y);
    ull blue = med_blue(grid, size, x, y);
    ull green = med_green(grid, size, x, y);
    ull mean = mean_func(grid, size, x, y, red, green, blue);
    (*root) = create_node();

    // daca scorul similaritatii este mai mare decat pragul atunci impartim
    // matricea in 4
    if (mean > prag) {
        // nu e frunza
        (*root)->tip = 0;
        // impartim matricea in 4
        size /= 2;
        create_arb(grid, size, x, y, &(*root)->unu, prag, size_maxim);
        create_arb(grid, size, x, y + size, &(*root)->doi, prag, size_maxim);
        create_arb(grid, size, x + size, y + size, &(*root)->trei, prag,
                   size_maxim);
        create_arb(grid, size, x + size, y, &(*root)->patru, prag, size_maxim);
    } else {
        // nr blocuri creste cu 1
        if ((*size_maxim) < size) (*size_maxim) = size;
        (*root)->tip = 1;
        ((*root)->value).red = red;
        ((*root)->value).blue = blue;
        ((*root)->value).green = green;
    }
}

// calculam nr de frunze ale arborelui
void nr_leaves(node *root, unsigned int *lvs) {
    if (root != NULL) {
        if (root->tip == 1) (*lvs)++;
        nr_leaves(root->unu, lvs);
        nr_leaves(root->doi, lvs);
        nr_leaves(root->trei, lvs);
        nr_leaves(root->patru, lvs);
    }
}

// aflam nr de nivele
unsigned int get_depth(node *root) {
    if (root == NULL) {
        return 0;
    } else {
        unsigned int unu_depth = get_depth(root->unu);
        unsigned int doi_depth = get_depth(root->doi);
        unsigned int trei_depth = get_depth(root->trei);
        unsigned int patru_depth = get_depth(root->patru);

        unsigned int max_depth = unu_depth;
        if (doi_depth > max_depth) max_depth = doi_depth;
        if (trei_depth > max_depth) max_depth = trei_depth;
        if (patru_depth > max_depth) max_depth = patru_depth;

        return max_depth + 1;
    }
}

// stergem arborele
void delete_arb(node *root) {
    // daca nu ne aflam pe ultimul nivel
    if (root != NULL) {
        delete_arb(root->unu);
        delete_arb(root->doi);
        delete_arb(root->trei);
        delete_arb(root->patru);
        free(root);
    }
}

// stergem matricea ce contine imaginea
void delete_imag(pixel **grid, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; ++i) {
        free(grid[i]);
    }
    free(grid);
}

// functia pentru compresia imaginii este un bfs
void compresie(node *root, FILE *out) {
    // cream o coada
    queue *q;
    // alocam memorie pentru coada
    q = initQ();

    // adaugam radacina in coada si o marcam ca vizitata
    addQ(q, root);
    node *current;

    // cat timp coada nu e goala
    while (!emptyQ(q)) {
        // luam elementul din capul cozii
        current = q->front->elem;

        // afisam nodul din coada
        fwrite(&current->tip, sizeof(current->tip), 1, out);
        if (current->tip == 1) {
            fwrite(&current->value.red, sizeof(current->value.red), 1, out);
            fwrite(&current->value.green, sizeof(current->value.green), 1, out);
            fwrite(&current->value.blue, sizeof(current->value.blue), 1, out);
        }

        // verificam daca e nod intern
        if (current->tip == 0) {
            // adaugam fiii in coada
            addQ(q, current->unu);
            addQ(q, current->doi);
            addQ(q, current->trei);
            addQ(q, current->patru);
        }
        // stergem din coada primul element
        popQ(q);
    }
    free(q);
}

// cream arborele pe baza compresiei imaginii
void decompresie(node **root, queue *q, FILE *in) {
    node *current;
    // adaugam radacina in coada
    addQ(q, (*root));
    while (!emptyQ(q)) {
        //  adresa nodului curent din coada
        current = q->front->elem;
        fread(&current->tip, sizeof(current->tip), 1, in);

        // daca e nod intern alocam memorie pentru fii
        // si ii adaugam in coada
        if (current->tip == 0) {
            current->unu = create_node();
            addQ(q, current->unu);
            current->doi = create_node();
            addQ(q, current->doi);
            current->trei = create_node();
            addQ(q, current->trei);
            current->patru = create_node();
            addQ(q, current->patru);
        }

        // daca e frunza adaugam rgb.ul nodului
        if (current->tip == 1) {
            unsigned char red, green, blue;
            fread(&red, sizeof(red), 1, in);
            fread(&green, sizeof(green), 1, in);
            fread(&blue, sizeof(blue), 1, in);
            current->value.red = red;
            current->value.green = green;
            current->value.blue = blue;
        }
        // stergem primul element din coada
        popQ(q);
    }
    // eliberam memoria pentru coada
    free(q);
}

// cream matricea pe baza arborelui
void create_matrix(node *root, pixel ***decom, unsigned int size, int x,
                   int y) {
    int i, j;
    // daca e frunza adaugam rgb-ul in matrice
    if (root->tip == 1) {
        for (i = x; i < x + size; ++i) {
            for (j = y; j < y + size; ++j) {
                (*decom)[i][j].red = root->value.red;
                (*decom)[i][j].green = root->value.green;
                (*decom)[i][j].blue = root->value.blue;
            }
        }
    } else {
        // daca e intern impartim matricea in 4
        size /= 2;
        create_matrix(root->unu, decom, size, x, y);
        create_matrix(root->doi, decom, size, x, y + size);
        create_matrix(root->trei, decom, size, x + size, y + size);
        create_matrix(root->patru, decom, size, x + size, y);
    }
}
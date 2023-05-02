/*Radu Andrei-Laurentiu-312CC */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#define ull unsigned long long

qcell *initQCell(node *elem);
// initializam coada
queue *initQ();

// verificam daca coada este goala
int emptyQ(queue *q);

// stergem din coada
void popQ(queue *q);

// adaugam in coada
void addQ(queue *q, node *elem);

// calculam folosind formula data pentru media valorilor unui bloc
ull med_red(pixel **grid, unsigned int size, unsigned int x, unsigned int y);

ull med_green(pixel **grid, unsigned int size, unsigned int x, unsigned int y);

ull med_blue(pixel **grid, unsigned int size, unsigned int x, unsigned int y);

// aflam scorul similaritatii
ull mean_func(pixel **grid, unsigned int size, unsigned int x, unsigned int y,
              ull red, ull green, ull blue);
// alocam memorie pentru o variabila de tip node
node *create_node();

// cream arborele cuaternar
void create_arb(pixel **grid, unsigned int size, unsigned int x, unsigned int y,
                node **root, ull prag, unsigned int *size_maxim);

// calculam nr de frunze ale arborelui
void nr_leaves(node *root, unsigned int *lvs);

// aflam nr de nivele
unsigned int get_depth(node *root);

// stergem arborele
void delete_arb(node *root);

// stergem matricea ce contine imaginea
void delete_imag(pixel **grid, unsigned int size);

// functia pentru compresia imaginii este un bfs
void compresie(node *root, FILE *out);

// cream arborele pe baza compresiei imaginii
void decompresie(node **root, queue *q, FILE *in);

// cream matricea pe baza arborelui
void create_matrix(node *root, pixel ***decom, unsigned int size, int x,
                   int y);

int main(int argc, char const *argv[]) {
    FILE *in, *out;
    // cerintele 1-2
    if (strcmp(argv[1], "-c1") == 0 || strcmp(argv[1], "-c2") == 0) {
        // fisierul din care citim(primit ca argument in linia de comanda)
        in = fopen(argv[3], "rb");

        // fisierul in care afisam(primit ca argument in linia de comanda)
        out = fopen(argv[4], "wb");

        // citim tipul fisierului
        char tip_fisier[2];
        fscanf(in, "%s\n", tip_fisier);

        // citim cele 3 informatii
        unsigned int height, width, max_color;
        fscanf(in, "%u %u %u", &height, &width, &max_color);

        // citim enterul
        char spatiu;
        fscanf(in, "%c", &spatiu);

        // citim matricea ce reprezinta poza
        pixel **imag = (pixel **)malloc(height * sizeof(pixel *));
        unsigned int i, j;

        // citim imaginea din zona binara
        for (i = 0; i < height; ++i) {
            imag[i] = (pixel *)malloc(width * sizeof(pixel));
            for (j = 0; j < width; ++j) {
                fread(&imag[i][j], sizeof(imag[i][j]), 1, in);
            }
        }

        // size va fi height sau width fiind matrice patratica
        unsigned int size = height;

        // root va retine radacina arborelui cuaternar
        node *root = NULL;

        // pragul primit ca argument in linia de comanda
        ull prag = atoi(argv[2]);

        // latura de lungime maxima
        unsigned int size_maxim = 0;
        create_arb(imag, size, 0, 0, &root, prag, &size_maxim);

        // pentru cerinta 1
        if (strcmp(argv[1], "-c1") == 0) {
            // parcurgem arborele si aflam nr de nivele
            unsigned int levels = get_depth(root);
            unsigned int blocks = 0;
            nr_leaves(root, &blocks);

            // nr de nivele din arbore
            fprintf(out, "%u\n", levels);
            // nr de blocuri este nr de frunze
            fprintf(out, "%u\n", blocks);
            // fprintf(out, "\n");

            // latura cea mai mare(frunzele de pe cel mai sus nivel)
            fprintf(out, "%u\n", size_maxim);

        } else {
            // pentru cerinta 2

            // afisam dimensiunea
            fwrite(&size, sizeof(size), 1, out);
            // facem compresia imaginii
            compresie(root, out);
        }

        // eliberam memoria
        delete_arb(root);
        delete_imag(imag, size);
    } else {
        // fisierul din care citim(primit ca argument in linia de comanda)
        in = fopen(argv[2], "rb");

        // fisierul in care afisam(primit ca argument in linia de comanda)
        out = fopen(argv[3], "wb");

        unsigned int size;
        fread(&size, sizeof(size), 1, in);
        // root va retine radacina arborelui cuaternar
        node *root = create_node();
        queue *q;
        q = initQ();

        // alocam memorie pentru matricea imaginii
        pixel **decom = (pixel **)malloc(size * sizeof(pixel *));
        for (int i = 0; i < size; ++i) {
            decom[i] = (pixel *)malloc(sizeof(pixel) * size);
        }

        // cream arborele
        decompresie(&root, q, in);

        // cream matricea pe baza arborelui
        create_matrix(root, &decom, size, 0, 0);

        // afisam tipul fisierului, dimensiunile si valoarea maxima ale matricei
        fprintf(out, "P6\n");
        fprintf(out, "%u %u\n", size, size);
        fprintf(out, "255\n");

        // afisam matricea in fisierul ppm
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                fwrite(&decom[i][j].red, sizeof(decom[i][j].red), 1, out);
                fwrite(&decom[i][j].green, sizeof(decom[i][j].green), 1, out);
                fwrite(&decom[i][j].blue, sizeof(decom[i][j].blue), 1, out);
            }
        }

        // eliberam memoria
        delete_imag(decom, size);
        delete_arb(root);
    }

    // inchidem fisierele
    fclose(in);
    fclose(out);

    return 0;
}

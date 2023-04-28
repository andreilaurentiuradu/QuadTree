#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ull unsigned long long

typedef struct pixel {
    unsigned char red, green, blue;
} pixel;

typedef struct Node {
    pixel value;
    int viz;
    unsigned char tip;
    struct Node *unu, *doi, *trei, *patru;
} node;

// celulele pentru coada
typedef struct qCell {
    node *elem;
    struct qCell *next;
} qcell;

typedef struct Queue {
    qcell *front, *tail;
} queue;

// alocam memorie pentru qcell(element al cozii)
qcell *initQCell(node *elem) {
    qcell *p = (qcell *)malloc(sizeof(qcell));
    p->elem = (node *)malloc(sizeof(node));
    p->elem = elem;
    p->next = NULL;
    return p;
}

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

    // eliberam memoria alocata pentru campul elem(node *) si qcell in sine
    // free(u->elem);
    // free(u);
}

// // adaugam in coada(head, element, element, element, tail)
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

ull med_red(pixel **grid, unsigned int size, unsigned int x, unsigned int y) {
    ull red = 0;
    unsigned int i, j;
    for (i = x; i < x + size; ++i) {
        for (j = y; j < y + size; ++j) {
            red += (grid[i][j].red);
        }
    }
    return red / (size * size);
}

ull med_green(pixel **grid, unsigned int size, unsigned int x, unsigned int y) {
    ull green = 0;
    unsigned int i, j;
    for (i = x; i < x + size; ++i) {
        for (j = y; j < y + size; ++j) {
            green += (grid[i][j].green);
        }
    }
    return green / (size * size);
}

ull med_blue(pixel **grid, unsigned int size, unsigned int x, unsigned int y) {
    ull blue = 0;
    unsigned int i, j;
    for (i = x; i < x + size; ++i) {
        for (j = y; j < y + size; ++j) {
            blue += (grid[i][j].blue);
        }
    }
    return blue / (size * size);
}

ull mean_func(pixel **grid, unsigned int size, unsigned int x, unsigned int y,
              ull red, ull green, ull blue) {
    ull mean = 0;
    unsigned int i, j;
    for (i = x; i < x + size; ++i) {
        for (j = y; j < y + size; ++j) {
            mean += (red - (grid[i][j].red)) * (red - (grid[i][j].red));
            mean += (green - (grid[i][j].green)) * (green - (grid[i][j].green));
            mean += (blue - (grid[i][j].blue)) * (blue - (grid[i][j].blue));
        }
    }
    mean = mean / (3 * size * size);

    return mean;
}

node *create_node() {
    node *new_node = (node *)malloc(sizeof(node));
    new_node->tip = 0;
    new_node->viz = 0;
    (new_node->value).blue = (new_node->value).red = (new_node->value).green =
        0;
    new_node->unu = new_node->doi = new_node->trei = new_node->patru = NULL;
    return new_node;
}
// int nr_nod;
void create_arb(pixel **grid, unsigned int size, unsigned int x, unsigned int y,
                node **root, ull prag, unsigned int *size_minim) {
    ull red = med_red(grid, size, x, y);
    ull blue = med_blue(grid, size, x, y);
    ull green = med_green(grid, size, x, y);
    ull mean = mean_func(grid, size, x, y, red, green, blue);
    // ++nr_nod;
    // printf("nr_nod:%u   red:%llu   green:%llu   blue:%llu  mean:%llu\n",
    //  nr_nod, red, green, blue, mean);
    (*root) = create_node();
    // printf("mean:%llu    prag:%llu\n", mean, prag);
    if (mean > prag) {
        (*root)->tip = 0;  // nu e frunza
        size /= 2;
        create_arb(grid, size, x, y, &(*root)->unu, prag, size_minim);
        create_arb(grid, size, x, y + size, &(*root)->doi, prag, size_minim);
        create_arb(grid, size, x + size, y + size, &(*root)->trei, prag,
                   size_minim);
        create_arb(grid, size, x + size, y, &(*root)->patru, prag, size_minim);
    } else {
        // printf("size este:%d\n", size);
        // nr blocuri creste cu 1
        if ((*size_minim) < size) (*size_minim) = size;
        (*root)->tip = 1;
        ((*root)->value).red = red;
        ((*root)->value).blue = blue;
        ((*root)->value).green = green;
    }
}

void nr_leaves(node *root, unsigned int *lvs) {
    if (root != NULL) {
        if (root->tip == 1) (*lvs)++;
        nr_leaves(root->unu, lvs);
        nr_leaves(root->doi, lvs);
        nr_leaves(root->trei, lvs);
        nr_leaves(root->patru, lvs);
    }
}

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

void delete_imag(pixel **grid, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; ++i) {
        free(grid[i]);
    }
    free(grid);
}

// void print_node(node *p, FILE *out) {
//     fwrite(p->tip, sizeof(p->tip), 1, out);
//     if (p->tip == 1) {
//         fwrite(p->value, sizeof(p->value), 1, out);
//     }
// }

void compresie(node *root, FILE *out) {
    root->viz = 1;
    queue *q;
    q = initQ();
    addQ(q, root);
    // printf("root %u\n", (q->tail)->elem->viz);
    node *current;
    while (!emptyQ(q)) {
        // luam elementul din capul cozii
        current = q->front->elem;
        // printf("current %u\n", current->viz);
        // il stergem din coada
        popQ(q);
        // printf("current %u\n", current->viz);
        // afisam nodul din coada
        // print_node(current, out);

        fwrite(&current->tip, sizeof(current->tip), 1, out);
        if (current->tip == 1) {
            fwrite(&current->value.red, sizeof(current->value.red), 1, out);
            fwrite(&current->value.green, sizeof(current->value.green), 1, out);
            fwrite(&current->value.blue, sizeof(current->value.blue), 1, out);
        }
        // printf("current_tip %u\n", current->tip);
        if(current->tip == 0)
        {
            if (current->unu->viz == 0) {
            current->unu->viz = 2;
            addQ(q, current->unu);
            // printf("unu %u\n", (q->tail)->elem->viz);
            }

            if (current->doi->viz == 0) {
                current->doi->viz = 3;
                addQ(q, current->doi);
                // printf("doi %u\n", (q->tail)->elem->viz);
            }

            if (current->trei->viz == 0) {
                current->trei->viz = 4;
                addQ(q, current->trei);
                // printf("ttrei %u\n", (q->tail)->elem->viz);
            }

            if (current->patru->viz == 0) {
                current->patru->viz = 5;
                addQ(q, current->patru);
                // printf("patru %u\n", (q->tail)->elem->viz);
            }
        }

    }

}

int main(int argc, char const *argv[]) {
    // facem pentru cerinta 1
    if (strcmp(argv[1], "-c1") == 0) {
        // fisierul din care citim(primit ca argument in linia de comanda)
        FILE *in = fopen(argv[3], "rb");

        // fisierul in care afisam(primit ca argument in linia de comanda)
        FILE *out = fopen(argv[4], "wb");

        // citim tipul fisierului
        char tip_fisier[2];
        fscanf(in, "%s\n", tip_fisier);

        // citim cele 3 informatii
        unsigned int height, width, max_color;
        fscanf(in, "%u %u %u", &height, &width, &max_color);
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

        // size va fi height sau width fiin matrice patratica
        unsigned int size = height;

        // root va retine radacina arborelui cuaternar
        node *root = NULL;
        ull prag = atoi(argv[2]);

        unsigned int size_minim = 0;
        create_arb(imag, size, 0, 0, &root, prag, &size_minim);
        // order(root);

        // parcurgem arborele si aflam nr de nivele
        unsigned int levels = get_depth(root);
        unsigned int blocks = 0;
        nr_leaves(root, &blocks);

        // nr de nivele din arbore este corect
        fprintf(out, "%u\n", levels);
        // nr de blocuri este nr de frunze(pica testele 4-5, nimeni nu stie dc)
        fprintf(out, "%u\n", blocks);
        // fprintf(out, "\n");

        // latura cea mai mare(practic frunzele de pe cel mai sus nivel)
        fprintf(out, "%u\n", size_minim);

        // eliberam memoria si inchidem fisierele
        delete_arb(root);
        delete_imag(imag, size);
        fclose(in);
        fclose(out);
    }

    if (strcmp(argv[1], "-c2") == 0) {
        // fisierul din care citim(primit ca argument in linia de comanda)
        FILE *in = fopen(argv[3], "rb");

        // fisierul in care afisam(primit ca argument in linia de comanda)
        FILE *out = fopen(argv[4], "wb");

        // citim tipul fisierului
        char tip_fisier[2];
        fscanf(in, "%s\n", tip_fisier);

        // citim cele 3 informatii
        unsigned int height, width, max_color;
        fscanf(in, "%u %u %u", &height, &width, &max_color);
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

        // size va fi height sau width fiin matrice patratica
        unsigned int size = height;

        // root va retine radacina arborelui cuaternar
        node *root = NULL;
        ull prag = atoi(argv[2]);

        unsigned int size_minim = 0;
        create_arb(imag, size, 0, 0, &root, prag, &size_minim);

        fwrite(&size, sizeof(size), 1, out);
        compresie(root, out);
        // eliberam memoria si inchidem fisierele
        delete_arb(root);
        delete_imag(imag, size);
        fclose(in);
        fclose(out);
    }
    // queue *q;
    // q = initQ();

    // node *nou = create_node();
    // (nou->value).blue = 1;
    // addQ(q, nou);
    // (nou->value).blue = 2;
    // addQ(q, nou);
    // (nou->value).blue = 3;
    // addQ(q, nou);
    // printf("%u\n", (q->tail)->elem->value.blue);
    // // popQ(q);
    // if (!emptyQ(q)) {
    //     printf("nu e goala\n");
    // } else
    //     printf(" este goasla\n");
    return 0;
}

/*TO DO
// cerinta 2(compresia) pui in binar 0 daca e interior sau 1 si rgb daca e
frunza
    // faci bfs si afisezi progresiv
// cerinta 3(decompresia)
    //1 cand e 0 il bagi in coada
    //2 daca nu e 0 citesti rgb
    //3 refaci arborele practic
    //4 scrii in ppm tipul , sizeurile si 255
    //
*/

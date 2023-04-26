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
    unsigned char tip;
    struct Node *unu, *doi, *trei, *patru;
} node;

ull med_red(pixel **grid, unsigned int size, unsigned int x, unsigned int y) {
    ull red = 0;
    for (unsigned int i = x; i < x + size; ++i) {
        for (unsigned int j = y; j < y + size; ++j) {
            red += (unsigned int)(grid[i][j].red);
        }
    }
    return red / size / size;
}

ull med_green(pixel **grid, unsigned int size, unsigned int x, unsigned int y) {
    ull green = 0;
    for (unsigned int i = x; i < x + size; ++i) {
        for (unsigned int j = y; j < y + size; ++j) {
            green += (unsigned int)(grid[i][j].green);
        }
    }
    return green / size / size;
}

ull med_blue(pixel **grid, unsigned int size, unsigned int x, unsigned int y) {
    ull blue = 0;
    for (unsigned int i = x; i < x + size; ++i) {
        for (unsigned int j = y; j < y + size; ++j) {
            blue += (unsigned int)(grid[i][j].blue);
        }
    }
    return blue / size / size;
}

ull mean_func(pixel **grid, unsigned int size, unsigned int x, unsigned int y, ull red, ull green,
              ull blue) {
    ull mean = 0;

    for (unsigned int i = x; i < x + size; ++i) {
        for (unsigned int j = y; j < y + size; ++j) {
            mean += (red - (unsigned int)(grid[i][j].red)) *
                    (red - (unsigned int)(grid[i][j].red));
            mean += (green - (unsigned int)(grid[i][j].green)) *
                    (green - (unsigned int)(grid[i][j].green));
            mean += (blue - (unsigned int)(grid[i][j].blue)) *
                    (blue - (unsigned int)(grid[i][j].blue));
        }
    }
    mean = mean / (3 * size * size);

    // for(unsigned int i = x; i < x + size; ++i) {
    //     for(unsigned int j = y; j < y + size; ++j) {
    //         prunsigned intf("%u ", grid[i][j]);
    //     }
    //     prunsigned intf("\n");
    // }
    return mean;
}

node *create_node() {
    node *new_node = (node *)malloc(sizeof(node));
    new_node->tip = 0;
    (new_node->value).blue = (new_node->value).red = (new_node->value).green =
        0;
    new_node->unu = new_node->doi = new_node->trei = new_node->patru = NULL;
    return new_node;
}

// unsigned int nr_nod = 0;
void create_arb(pixel **grid, unsigned int size, unsigned int x, unsigned int y, node **root, ull prag,
                unsigned int *size_minim, unsigned int *blocks) {
    ull red = med_red(grid, size, x, y);
    ull blue = med_blue(grid, size, x, y);
    ull green = med_green(grid, size, x, y);
    ull mean = mean_func(grid, size, x, y, red, green, blue);

    (*root) = create_node();
    if (mean > prag) {
        // printf("oare unsigned intra aici????\n");
        (*root)->tip = '0';  // nu e frunza
        // printf("size este in if:%d\n", size);
        size /= 2;
        // printf("cat e size:%d\n", size);
        create_arb(grid, size, x, y, &(*root)->unu, prag, size_minim, blocks);
        create_arb(grid, size, x, y + size, &(*root)->doi, prag, size_minim,
                   blocks);
        create_arb(grid, size, x + size, y + size, &(*root)->trei, prag,
                   size_minim, blocks);
        create_arb(grid, size, x + size, y, &(*root)->patru, prag, size_minim,
                   blocks);
    } else {
        // printf("size este:%d\n", size);
        // creste numarul de blocuri ce respecta pragul
        (*blocks)++;
        if ((*size_minim) > size) (*size_minim) = size;
        (*root)->tip = '1';
        ((*root)->value).red = red;
        ((*root)->value).blue = blue;
        ((*root)->value).green = green;
    }
}

void order(node *root) {
    if (root != NULL) {
        // printf("unsigned intraaaaa????????????\n");
        // printf("nr_nod:%d\n", root->tip);
        order(root->unu);
        order(root->doi);
        order(root->trei);
        order(root->patru);
    }
}

ull get_depth(node *root) {
    if (root == NULL) {
        return 0;
    } else {
        ull unu_depth = get_depth(root->unu);
        ull doi_depth = get_depth(root->doi);
        ull trei_depth = get_depth(root->trei);
        ull patru_depth = get_depth(root->patru);

        ull max_depth = unu_depth;
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
    for(unsigned int i = 0; i < size; ++i) {
        free(grid[i]);
    }
    free(grid);
}

int main(int argc, char const *argv[]) {
    if (strcmp(argv[1], "-c1") == 0) {
        // fisierul din care citim(primit ca argument in linia de comanda)
        FILE *in = fopen(argv[3], "rb");

        // fisierul in care afisam(primit ca argument in linia de comanda)
        FILE *out = fopen(argv[4], "wb");

        // citim tipul fisierului
        char tip_fisier[2];
        fscanf(in, "%s\n", tip_fisier);

        // afisam tipul in out ca sa scap de warning momentan
        // fprintf intf(out, "tip_fisier:%s\n", tip_fisier);
        // prunsigned intf("tip_fisier:%s\n", tip_fisier);

        unsigned int height, width, max_color;
        fscanf(in, "%u %u %u", &height, &width, &max_color);

        // prunsigned intf("height:%u\n", height);
        // prunsigned intf("width:%u\n", width);
        // prunsigned intf("max_color:%u\n", max_color);

        // citim matricea ce reprezunsigned inta poza
        pixel **imag = (pixel **)malloc(height * sizeof(pixel *));

        char spatiu;
        // citim enterul de dupa
        fscanf(in, "%c", &spatiu);

        for (unsigned int i = 0; i < height; ++i) {
            imag[i] = (pixel *)malloc(width * sizeof(pixel));
            for (unsigned int j = 0; j < width; ++j) {
                fread(&imag[i][j], sizeof(imag[i][j]), 1, in);
            }
        }

        // prunsigned intf("red:%d   green:%d  blue:%d\n", imag[0][200].red,
        // imag[0][200].green, imag[0][200].blue);
        unsigned int size = height;
        // ull red = med_red(imag, size, 0, 0);
        // ull blue = med_blue(imag, size, 0, 0);
        // ull green = med_green(imag, size, 0, 0);
        // ull mean = mean_func(imag, size, 0, 0, red, green, blue);
        node *root = NULL;
        ull prag = atoi(argv[2]);

        unsigned int size_minim = size, blocks = 0;
        create_arb(imag, size, 0, 0, &root, prag, &size_minim, &blocks);
        order(root);

        // printf("nr_nivele:%d\n", get_depth(root));
        ull levels = get_depth(root);
        fprintf(out, "%llu\n", levels);
        fprintf(out, "%d\n", blocks);
        fprintf(out, "%d\n", size_minim);

        // eliberam memoria si inchidem fisierele
        delete_arb(root);
        delete_imag(imag, size);
        fclose(in);
        fclose(out);
    }

    return 0;
}

/*TO DO
    2 dupa faci celelate 2 chestii de la cerunsigned inta 1 tot prin parcurgerea
    arborelui(bfs e bun)
    3 vezi ca a aparut checkerul modifici makefileul astfel
    incat sa testezi checkerul pentru cerunsigned inta 1
    4 treci la cerunsigned inta 2(compresia imaginii)
    5 treci la cerunsigned inta 3(decompresia imaginii)
*/

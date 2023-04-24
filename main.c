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
    int tip;
    struct Node *unu, *doi, *trei, *patru;
} node;

ull med_red(pixel **grid, int size, int x, int y) {
    ull red = 0;
    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            red += (unsigned int)(grid[i][j].red);
        }
    }
    return red / size / size;
}

ull med_green(pixel **grid, int size, int x, int y) {
    ull green = 0;
    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            green += (unsigned int)(grid[i][j].green);
        }
    }
    return green / size / size;
}

ull med_blue(pixel **grid, int size, int x, int y) {
    ull blue = 0;
    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            blue += (unsigned int)(grid[i][j].blue);
        }
    }
    return blue / size / size;
}

ull mean_func(pixel **grid, int size, int x, int y, ull red, ull green, ull blue) {
    ull mean = 0;

    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            mean += (red - (unsigned int)(grid[i][j].red)) *
                    (red - (unsigned int)(grid[i][j].red));
            mean += (green - (unsigned int)(grid[i][j].green)) *
                    (green - (unsigned int)(grid[i][j].green));
            mean += (blue - (unsigned int)grid[i][j].blue) *
                    (blue - (unsigned int)(grid[i][j].blue));
        }
    }
    mean = mean / (3 * size * size);

    // for(int i = x; i < x + size; ++i) {
    //     for(int j = y; j < y + size; ++j) {
    //         printf("%u ", grid[i][j]);
    //     }
    //     printf("\n");
    // }
    return mean;
}

node *create_node() {
    node *new_node = (node *)malloc(sizeof(node));
    new_node->tip = 0;
    (new_node->value).blue = (new_node->value).red = (new_node->value).green = 0;
    new_node->unu = new_node->doi = new_node->trei = new_node->patru = NULL;
    return new_node;
}

void create_arbore(pixel **grid, int size, int x, int y, node *root, ull prag) {
    ull red = med_red(grid, size, x, y);
    ull blue = med_blue(grid, size, x, y);
    ull green = med_green(grid, size, x, y);
    ull mean = mean_func(grid, size, x, y, red, green, blue);
    printf("red:%llu   green:%llu   blue:%llu   mean:%llu\n", red, green, blue, mean);
    root = create_node();
    if(mean > prag) {
        root->tip = 0; // nu e frunza
        size /= 2;
        printf("cat e size:%d\n", size);
        // create_arbore(grid, size, x, y, root, 0);
        // create_arbore(grid, size, x, y + size, root, 0);
        // create_arbore(grid, size, x + size, y + size, root, 0);
        // create_arbore(grid, size, x + size, y, root, 0);
        create_arbore(grid, size, x, y, root->unu, prag);
        create_arbore(grid, size, x, y + size, root->doi, prag);
        create_arbore(grid, size, x + size, y + size, root->trei, prag);
        create_arbore(grid, size, x + size, y, root->patru, prag);
    }
    else
    {
        root->tip = 1;
        (root->value).red = red;
        (root->value).blue = blue;
        (root->value).green = green;
    }
}


void order(node* root) {
    if (root) {
        printf("red: %d ", (root->value).red);
        printf("green: %d ", (root->value).green);
        printf("blue: %d\n", (root->value).blue);
        order(root->unu);
        order(root->doi);
        order(root->trei);
        order(root->patru);
    }
}

int main(int argc, char const *argv[]) {
    // fisierul din care citim(primit ca argument in linia de comanda)
    FILE *in = fopen(argv[1], "rb");

    // fisierul in care afisam(primit ca argument in linia de comanda)
    FILE *out = fopen(argv[2], "wb");

    // citim tipul fisierului
    char tip_fisier[2];
    fread(tip_fisier, sizeof(tip_fisier), 1, in);

    // afisam tipul in out ca sa scap de warning momentan
    fprintf(out, "tip_fisier:%s\n", tip_fisier);
    printf("tip_fisier:%s\n", tip_fisier);

    unsigned int height, width, max_color;
    fscanf(in, "%u %u %u", &height, &width, &max_color);

    printf("height:%u\n", height);
    printf("width:%u\n", width);
    printf("max_color:%u\n", max_color);

    // citim matricea ce reprezinta poza
    pixel **imag = (pixel **)malloc(height * sizeof(pixel *));

    char spatiu;
    fscanf(in, "%c", &spatiu);

    for (int i = 0; i < height; ++i) {
        imag[i] = (pixel *)malloc(width * sizeof(pixel));
        for (int j = 0; j < width; ++j) {
            fread(&imag[i][j], sizeof(imag[i][j]), 1, in);
        }
    }

    // for(int i = 0; i < height; ++i) {
    //     for(int j = 0; j < width; ++j) {
    //        fprintf(out, "red:%d   green:%d  blue:%d\n", imag[i][j].red,
    //        imag[i][j].green, imag[i][j].blue);
    //     }
    // }

    // printf("red:%d   green:%d  blue:%d\n", imag[0][200].red,
    // imag[0][200].green, imag[0][200].blue);
    int size = height;
    ull red = med_red(imag, size, 0, 0);
    ull blue = med_blue(imag, size, 0, 0);
    ull green = med_green(imag, size, 0, 0);

    node *root = NULL;
    ull mean = mean_func(imag, size, 0, 0, red, green, blue);
    ull prag = 0;

    printf("mean:%llu\n", mean);
    if (prag < mean) {
        printf("il separi\n");

    } else {
        printf("nu-l separi\n");
    }

    create_arbore(imag, size, 0, 0, root, prag);
    order(root);
    return 0;
}

/*TO DO
    1 vezi cate nivele are arborele(bfs e bun si pui un parametru)
    2 dupa faci celelate 2 chesti de la cerinta 1 tot prin parcurgerea arborelui(bfs e bun)
    3 vezi ca a aparut checkerul modifici makefileul astfel incat sa testezi checkerul pentru cerinta 1
    4 treci la cerinta 2(compresia imaginii)
    5 treci la cerinta 3(decompresia imaginii)
*/

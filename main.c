#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define ull unsigned long long

typedef struct pixel{
    unsigned char red, green, blue;
} pixel;

typedef struct Node {
    pixel value;
    int tip;
    struct Node *unu, *doi, *trei, *patru;
} node;

ull med_red(pixel **grid, int size, int x, int y) {
    ull red = 0;
    for(int i = x; i < x + size; ++i) {
        for(int j = y; j < y + size; ++j) {
            red += (unsigned int)(grid[i][j].red);
        }
    }
    return red / size / size;
}

ull med_green(pixel **grid, int size, int x, int y) {
    ull green = 0;
    for(int i = x; i < x + size; ++i) {
        for(int j = y; j < y + size; ++j) {
            green += (unsigned int)(grid[i][j].green);
        }
    }
    return green / size / size;
}

ull med_blue(pixel **grid, int size, int x, int y) {
    ull blue = 0;
    for(int i = x; i < x + size; ++i) {
        for(int j = y; j < y + size; ++j) {
            blue += (unsigned int)(grid[i][j].blue);
        }
    }
    return blue / size / size;
}

ull mean_func(pixel **grid, int size, int x, int y) {
    ull red = med_red(grid, size, x, y);
    ull blue = med_blue(grid, size, x, y);
    ull green = med_green(grid, size, x, y);

    ull mean = 0;

    for(int i = x; i < x + size; ++i) {
        for(int j = y; j < y + size; ++j) {
            mean += (red - (unsigned int)(grid[i][j].red)) * (red - (unsigned int)(grid[i][j].red));
            mean += (green - (unsigned int)(grid[i][j].green)) * (green - (unsigned int)(grid[i][j].green));
            mean += (blue - (unsigned int)grid[i][j].blue) * (blue - (unsigned int)(grid[i][j].blue));
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


node* create_node(pixel *p, int tip) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->tip = tip;
    (new_node->value).red = p->red;
    (new_node->value).blue = p->blue;
    (new_node->value).green = p->green;
    new_node->unu = new_node->doi = new_node->trei = new_node->patru = NULL;
    return new_node;
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
    pixel **imag = (pixel **) malloc(height * sizeof(pixel *));

    char spatiu;
    fscanf(in, "%c", &spatiu);

    for(int i = 0; i < height; ++i) {
        imag[i] = (pixel *) malloc (width * sizeof(pixel));
        for(int j = 0; j < width; ++j) {
            fread(&imag[i][j], sizeof(imag[i][j]), 1, in);
        }
    }

    // for(int i = 0; i < height; ++i) {
    //     for(int j = 0; j < width; ++j) {
    //        fprintf(out, "red:%d   green:%d  blue:%d\n", imag[i][j].red, imag[i][j].green, imag[i][j].blue);
    //     }
    // }

    //printf("red:%d   green:%d  blue:%d\n", imag[0][200].red, imag[0][200].green, imag[0][200].blue);
    int factor = 0, size = height;
    node *root = NULL;

    ull mean = mean_func(imag, size, 0, 0);
    //printf("size:%")
    printf("mean:%llu\n", mean);
    // if(factor < mean) {
    //     printf("il separi\n");
    // }
    // else
    //     printf("nu-l separi\n");


    return 0;
}
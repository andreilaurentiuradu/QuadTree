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

// celulele pentru coada
typedef struct qCell {
    node *elem;
    struct qCell *next;
} qcell;

// structura de tip coada
typedef struct Queue {
    qcell *front, *tail;
} queue;

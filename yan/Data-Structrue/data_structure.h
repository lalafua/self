#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100
#define SUCCESS 1
#define FAILURE -1

typedef int elem_t;

typedef struct tree_node {
    elem_t data;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

#endif

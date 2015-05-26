#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "vector.h"
#include "avl.h"

avl_type_content(int, int)
vector_type_content(int, int)

int main(void) {
    struct avl *tree = avl_int_new();
    struct vector *vec = vector_int_new();

    int i;
    for (i = 0; i < 100; ++i) {
        int r = rand() % 1000;
        avl_int_insert(tree, r);
        vector_int_push(vec, r);
    }

    printf("Content:\n");
    for (i = 0; i < vector_size(vec); ++i) {
        printf("%d%c", *vector_int_nth(vec, i), " \n"[i == vector_size(vec) - 1]);
    }

    printf("In order:\n");
    for (i = 0; i < avl_size(tree); ++i) {
        printf("%d%c", *avl_int_nth(tree, i), " \n"[i == avl_size(tree) - 1]);
    }

    avl_free(tree);
    return 0;
}


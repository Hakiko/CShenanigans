#include "vector.h"

#include <stdlib.h>

struct vector {
    struct vector_type_info type;
    int alloc_count;
    int used_count;
    void *data;
};

struct vector *vector_new(const struct vector_type_info *info) {
    struct vector *vec = (struct vector*)malloc(sizeof(struct vector));
    vec->type = *info;
    vec->alloc_count = 1;
    vec->used_count = 0;
    vec->data = malloc(info->allocSize);
    return vec;
}

void vector_free(struct vector *vec) {
    int i;
    for (i = 0; i < vec->used_count; ++i) {
        vec->type.deinit(vector_nth(vec, i));
    }
    free(vec->data);
    free(vec);
}

void vector_push(struct vector *vec, const void *obj) {
    if (vec->alloc_count == vec->used_count) {
        vec->data = realloc(vec->data,
                            vec->alloc_count * 2 * vec->type.allocSize);
        vec->alloc_count *= 2;
    }
    vec->type.copy(vector_nth(vec, vec->used_count), obj, 1);
    vec->used_count += 1;
}

void *vector_nth(const struct vector *vec, int n) {
    return (char*)vec->data + vec->type.allocSize * n;
}

int vector_size(const struct vector *vec) {
    return vec->used_count;
}

void vector_clear(struct vector *vec) {
    int i;
    for (i = 0; i < vec->used_count; ++i) {
        vec->type.deinit(vector_nth(vec, i));
    }
    vec->used_count = 0;
}

void vector_pop(struct vector *vec)
{
    vec->type.deinit(vector_nth(vec, vec->used_count - 1));
    vec->used_count -= 1;
}

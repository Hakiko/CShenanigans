#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>

struct vector_type_info {
    void (*deinit)(void *obj);
    void (*copy)(void *dst, const void *src, int count);
    int allocSize;
};

struct vector;

struct vector *vector_new(const struct vector_type_info *info);

void vector_free(struct vector *vec);

void vector_push(struct vector *vec, const void *obj);

void vector_pop(struct vector *vec);

void *vector_nth(const struct vector *vec, int n);

int vector_size(const struct vector *vec);

void vector_clear(struct vector *vec);

#define vector_type_info_deinit(type, type_name) \
    void _vector_ ## type_name ## _deinit(void *obj) {\
        (void)obj;\
    }

#define vector_type_info_copy(type, type_name) \
    void _vector_ ## type_name ## _copy(void *dst, const void *src, int count) {\
        memcpy(dst, src, sizeof(type) * count);\
    }

#define vector_type_info_pack(type, type_name) \
    vector_type_info_deinit(type, type_name) \
    vector_type_info_copy(type, type_name)

#define vector_type_new(type, type_name) \
    struct vector *vector_ ## type_name ## _new() {\
        struct vector_type_info type_name ## _info;\
        type_name ## _info.deinit = _vector_ ## type_name ## _deinit;\
        type_name ## _info.copy = _vector_ ## type_name ## _copy;\
        type_name ## _info.allocSize = sizeof(type);\
        return vector_new(&type_name ## _info);\
    }

#define vector_type_push(type, type_name) \
    void vector_ ## type_name ## _push(struct vector *vec, type obj) {\
        vector_push(vec, &obj);\
    }

#define vector_type_nth(type, type_name) \
    type *vector_ ## type_name ## _nth(const struct vector *vec, int n) {\
        return (type*)vector_nth(vec, n);\
    }

#define vector_type_functions(type, type_name) \
    vector_type_new(type, type_name) \
    vector_type_push(type, type_name) \
    vector_type_nth(type, type_name)

#define vector_type_content(type, type_name)\
    vector_type_info_pack(type, type_name) \
    vector_type_functions(type, type_name)

#endif // VECTOR_H

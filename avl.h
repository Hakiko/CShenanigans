#ifndef AVL_H
#define AVL_H

struct avl_type_info {
    void *(*allocate)();
    void (*free)(void *obj);
    void (*copy)(void *dst, const void *src);
    int (*compare)(const void *a, const void *b);
};

struct avl;

struct avl *avl_new(const struct avl_type_info *info);

void avl_free(struct avl *tree);

void avl_insert(struct avl *tree, const void *obj);

void *avl_find(const struct avl *tree, const void *obj);

void avl_erase(struct avl *tree, const void *obj);

void *avl_nth(const struct avl *tree, int n);

int avl_size(const struct avl *tree);

void avl_clear(struct avl *tree);

#define avl_type_info_alloc(type, type_name) \
    void *_avl_ ## type_name ## _alloc() {\
        return malloc(sizeof(type));\
    }

#define avl_type_info_free(type, type_name) \
    void _avl_ ## type_name ## _free(void *obj) {\
        free((type*)obj);\
    }

#define avl_type_info_copy(type, type_name) \
    void _avl_ ## type_name ## _copy(void *dst, const void *src) {\
        *(type*)dst = *(type*)src;\
    }

#define avl_type_info_compare(type, type_name) \
    int _avl_ ## type_name ## _compare(const void *a, const void *b) {\
        int d = *(type*)a - *(type*)b;\
        return d < 0 ? -1 : (d > 0 ? 1 : 0);\
    }

#define avl_type_info_pack(type, type_name) \
    avl_type_info_alloc(type, type_name) \
    avl_type_info_free(type, type_name) \
    avl_type_info_copy(type, type_name) \
    avl_type_info_compare(type, type_name)

#define avl_type_new(type, type_name) \
    struct avl *avl_ ## type_name ## _new() {\
        struct avl_type_info type_name ## _info;\
        type_name ## _info.allocate = _avl_ ## type_name ## _alloc;\
        type_name ## _info.free = _avl_ ## type_name ## _free;\
        type_name ## _info.copy = _avl_ ## type_name ## _copy;\
        type_name ## _info.compare = _avl_ ## type_name ## _compare;\
        return avl_new(&type_name ## _info);\
    }

#define avl_type_insert(type, type_name) \
    void avl_ ## type_name ## _insert(struct avl *tree, const type obj) {\
        avl_insert(tree, &obj);\
    }

#define avl_type_nth(type, type_name) \
    type *avl_ ## type_name ## _nth(const struct avl *tree, int n) {\
        return (type*)avl_nth(tree, n);\
    }

#define avl_type_find(type, type_name) \
    type *avl_ ## type_name ## _find(const struct avl *tree, const type obj) {\
        return (type*)avl_find(tree, &obj);\
    }

#define avl_type_erase(type, type_name) \
    void avl_ ## type_name ## _erase(struct avl *tree, const type obj) {\
        avl_erase(tree, &obj);\
    }

#define avl_type_functions(type, type_name) \
    avl_type_new(type, type_name) \
    avl_type_insert(type, type_name) \
    avl_type_nth(type, type_name) \
    avl_type_find(type, type_name) \
    avl_type_erase(type, type_name)

#define avl_type_content(type, type_name)\
    avl_type_info_pack(type, type_name) \
    avl_type_functions(type, type_name)

#endif

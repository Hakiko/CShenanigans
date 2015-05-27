#include "avl.h"

#include <stdlib.h>

struct avl_node {
    int depth;
    int size;
    struct avl_node *left;
    struct avl_node *right;
    void *obj;
};

struct avl_node *avl_node_new() {
    struct avl_node *res = (struct avl_node*)malloc(sizeof(struct avl_node));
    res->depth = 0;
    res->size = 0;
    res->left = NULL;
    res->right = NULL;
    res->obj = NULL;
    return res;
}

void avl_node_free(struct avl_node *node, const struct avl_type_info *info) {
    if (node->obj)
        info->free(node->obj);
    if (node->left)
        avl_node_free(node->left, info);
    if (node->right)
        avl_node_free(node->right, info);
    free(node);
}

int avl_node_depth_factor(const struct avl_node *node) {
    return (node->left ? node->left->depth + 1 : 0)
            - (node->right ? node->right->depth + 1 : 0);
}

void avl_node_update(struct avl_node *node) {
    int ld = node->left ? node->left->depth + 1 : 0;
    int rd = node->right ? node->right->depth + 1 : 0;
    node->depth = ld > rd ? ld : rd;
    node->size = (node->left ? node->left->size + 1 : 0)
            + (node->right ? node->right->size + 1 : 0);
}

struct avl_node *avl_node_rotate_left(struct avl_node *node) {
    struct avl_node *top = node->right;
    node->right = top->left;
    top->left = node;
    avl_node_update(top->left);
    avl_node_update(top);
    return top;
}

struct avl_node *avl_node_rotate_right(struct avl_node *node) {
    struct avl_node *top = node->left;
    node->left = top->right;
    top->right = node;
    avl_node_update(top->right);
    avl_node_update(top);
    return top;
}

struct avl_node *avl_node_balance(struct avl_node *node) {
    avl_node_update(node);
    int fac = avl_node_depth_factor(node);
    if (fac < -1) {
        if (avl_node_depth_factor(node->right) > 1)
            node->right = avl_node_rotate_right(node->right);
        node = avl_node_rotate_left(node);
    } else if (fac > 1) {
        if (avl_node_depth_factor(node->left) < -1)
            node->left = avl_node_rotate_left(node->left);
        node = avl_node_rotate_right(node);
    }
    return node;
}

struct avl_node *avl_node_insert(struct avl_node *node, const void *obj,
                                 const struct avl_type_info *info) {
    if (!node) {
        struct avl_node *res = avl_node_new();
        res->obj = info->allocate();
        info->copy(res->obj, obj);
        return res;
    }
    int cmp = info->compare(obj, node->obj);
    if (cmp < 0)
        node->left = avl_node_insert(node->left, obj, info);
    else if (cmp > 0)
        node->right = avl_node_insert(node->right, obj, info);
    return avl_node_balance(node);
}

void *avl_node_find(const struct avl_node *node, const void *obj,
                    const struct avl_type_info *info) {
    if (!node)
        return NULL;
    int cmp = info->compare(obj, node->obj);
    if (cmp < 0)
        return avl_node_find(node->left, obj, info);
    if (cmp > 0)
        return avl_node_find(node->right, obj, info);
    return node->obj;
}

struct avl_node *avl_node_extract(struct avl_node *node, struct avl_node **min) {
    if (!node->left) {
        struct avl_node *res = node->right;
        node->right = NULL;
        avl_node_update(node);
        if (min)
            *min = node;
        if (res) {
            avl_node_balance(res);
        }
        return res;
    } else {
        node->left = avl_node_extract(node->left, min);
        return avl_node_balance(node);
    }
}

struct avl_node *avl_node_erase(struct avl_node *node, const void *obj,
                                const struct avl_type_info *info) {
    if (!node)
        return NULL;
    int cmp = info->compare(obj, node->obj);
    if (cmp < 0)
        node->left = avl_node_erase(node->left, obj, info);
    else if (cmp > 0)
        node->right = avl_node_erase(node->right, obj, info);
    else {
        struct avl_node *res;
        if (node->left == NULL)
            res = node->right;
        else if (node->right == NULL)
            res = node->left;
        else {
            node->right = avl_node_extract(node->right, &res);
            res->left = node->left;
            res->right = node->right;
        }
        node->left = node->right = NULL;
        avl_node_free(node, info);
        if (res) {
            return avl_node_balance(res);
        }
        return NULL;
    }
    return avl_node_balance(node);
}

void *avl_node_nth(const struct avl_node *node, int n) {
    if (!node)
        return NULL;
    int ls = node->left ? node->left->size + 1 : 0;
    if (n < ls)
        return avl_node_nth(node->left, n);
    if (n > ls)
        return avl_node_nth(node->right, n - ls - 1);
    return node->obj;
}

struct avl {
    struct avl_node *root;
    struct avl_type_info type;
};

struct avl *avl_new(const struct avl_type_info *info)
{
    struct avl *res = (struct avl*)malloc(sizeof(struct avl));
    res->root = NULL;
    res->type = *info;
    return res;
}

void avl_free(struct avl *tree)
{
    if (tree->root)
        avl_node_free(tree->root, &tree->type);
    free(tree);
}

void avl_insert(struct avl *tree, const void *obj)
{
    tree->root = avl_node_insert(tree->root, obj, &tree->type);
}


void *avl_find(const struct avl *tree, const void *obj)
{
    return avl_node_find(tree->root, obj, &tree->type);
}


void avl_erase(struct avl *tree, const void *obj)
{
    tree->root = avl_node_erase(tree->root, obj, &tree->type);
}


void *avl_nth(const struct avl *tree, int n)
{
    return avl_node_nth(tree->root, n);
}


int avl_size(const struct avl *tree)
{
    return tree->root ? tree->root->size + 1 : 0;
}


void avl_clear(struct avl *tree)
{
    if (tree->root) {
        avl_node_free(tree->root, &tree->type);
        tree->root = NULL;
    }
}

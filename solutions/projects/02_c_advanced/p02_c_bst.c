/* p02_c_bst -- C advanced project. Reference solution. */
#include "cztest.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct BstNode {
    int key;
    struct BstNode *left;
    struct BstNode *right;
} BstNode;

int bst_insert(BstNode **root, int key)
{
    if (root == NULL) {
        return -1;
    }
    BstNode **slot = root;
    while (*slot != NULL) {
        if (key == (*slot)->key) {
            return 1;
        }
        slot = key < (*slot)->key ? &(*slot)->left : &(*slot)->right;
    }
    BstNode *node = malloc(sizeof *node);
    if (node == NULL) {
        return -1;
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    *slot = node;
    return 0;
}

int bst_contains(const BstNode *root, int key)
{
    for (const BstNode *node = root; node != NULL;) {
        if (key == node->key) {
            return 1;
        }
        node = key < node->key ? node->left : node->right;
    }
    return 0;
}

size_t bst_size(const BstNode *root)
{
    if (root == NULL) {
        return 0;
    }
    return 1 + bst_size(root->left) + bst_size(root->right);
}

int bst_height(const BstNode *root)
{
    if (root == NULL) {
        return 0;
    }
    const int left = bst_height(root->left);
    const int right = bst_height(root->right);
    return 1 + (left > right ? left : right);
}

static size_t write_inorder(const BstNode *root, int *out, size_t cap,
                            size_t written)
{
    if (root == NULL || written == cap) {
        return written;
    }
    written = write_inorder(root->left, out, cap, written);
    if (written < cap) {
        out[written++] = root->key;
    }
    return write_inorder(root->right, out, cap, written);
}

size_t bst_to_array(const BstNode *root, int *out, size_t cap)
{
    if (out == NULL || cap == 0) {
        return 0;
    }
    return write_inorder(root, out, cap, 0);
}

void bst_free(BstNode **root)
{
    if (root == NULL || *root == NULL) {
        return;
    }
    bst_free(&(*root)->left);
    bst_free(&(*root)->right);
    free(*root);
    *root = NULL;
}

int main(void)
{
    BstNode *root = NULL;

    CZ_CHECK_INT((long)bst_size(root), 0);
    CZ_CHECK_INT(bst_height(root), 0);
    CZ_CHECK_INT(bst_contains(root, 10), 0);
    CZ_CHECK_INT((long)bst_to_array(root, NULL, 0), 0);

    const int keys[] = {50, 30, 70, 20, 40, 60, 80, 30};
    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; ++i) {
        const int expected = keys[i] == 30 && i > 1 ? 1 : 0;
        CZ_CHECK_INT(bst_insert(&root, keys[i]), expected);
    }

    CZ_CHECK_INT((long)bst_size(root), 7);
    CZ_CHECK_INT(bst_height(root), 3);
    CZ_CHECK_INT(bst_contains(root, 40), 1);
    CZ_CHECK_INT(bst_contains(root, 45), 0);
    CZ_CHECK_INT(bst_contains(root, 80), 1);

    int out[16] = {0};
    const size_t count = bst_to_array(root, out, 16);
    CZ_CHECK_INT((long)count, 7);
    if (count == 7) {
        const int expected[] = {20, 30, 40, 50, 60, 70, 80};
        for (int i = 0; i < 7; ++i) {
            CZ_CHECK_INT(out[i], expected[i]);
        }
    }

    CZ_CHECK_INT((long)bst_to_array(root, out, 3), 3);
    CZ_CHECK_INT(out[0], 20);
    CZ_CHECK_INT(out[2], 40);

    bst_free(&root);
    CZ_CHECK(root == NULL);
    CZ_CHECK_INT((long)bst_size(root), 0);
    bst_free(&root);
    CZ_CHECK(root == NULL);

    return cztest_summary();
}

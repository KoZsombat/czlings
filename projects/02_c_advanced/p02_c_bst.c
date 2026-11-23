/* p02_c_bst -- C advanced project: an integer binary search tree.
 *
 * Prog1 lecture 11 builds binary trees with pointers. This project implements
 * the standard mutable BST:
 *
 *   typedef struct BstNode {
 *       int key;
 *       struct BstNode *left, *right;
 *   } BstNode;
 *
 *   bst_insert(&root, key)   0 inserted, 1 duplicate, -1 allocation
 *   bst_contains(root, key)  1 if present, else 0
 *   bst_size(root)           number of nodes
 *   bst_height(root)         0 for empty, 1 for a leaf
 *   bst_to_array(root, out, cap)  in-order keys; return how many were copied
 *   bst_free(&root)          free every node, set root to NULL
 *
 * The tree owns every node. There is no parent pointer; insert walks a
 * `BstNode **` so the new node can be linked directly.
 *
 * From TS: a tree node is an object kept alive by references. Here each node is
 * a separate malloc, and free must visit every child before its parent.
 */
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
    /* TODO: walk with a BstNode **slot; stop on a duplicate (return 1) or a
     * null link; allocate a leaf and link it there. Return -1 on malloc
     * failure. */
    (void)root;
    (void)key;
    return -1;
}

int bst_contains(const BstNode *root, int key)
{
    /* TODO: walk left or right until the key is found or the pointer is
     * NULL. */
    (void)root;
    (void)key;
    return 0;
}

size_t bst_size(const BstNode *root)
{
    /* TODO: 1 + size(left) + size(right). */
    (void)root;
    return 0;
}

int bst_height(const BstNode *root)
{
    /* TODO: 0 for NULL, otherwise 1 + max(height(left), height(right)). */
    (void)root;
    return 0;
}

size_t bst_to_array(const BstNode *root, int *out, size_t cap)
{
    /* TODO: in-order traversal (left, root, right). Stop when cap values have
     * been written and return the count. */
    (void)root;
    (void)out;
    (void)cap;
    return 0;
}

void bst_free(BstNode **root)
{
    /* TODO: post-order free every node, then set *root = NULL. */
    (void)root;
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

// I AM NOT DONE

/* zh2_list -- final ZH, task 2 (15 points). BME Prog1 style linked-list task.
 *
 * A playlist is a singly linked list of ints. Implement the five operations:
 *
 *   insert_sorted(&head, value)  insert so the list stays ascending;
 *                                duplicates are allowed
 *   remove_duplicates(head)      since the list is sorted, free every node
 *                                whose value equals the previous kept value
 *   reverse(&head)               reverse the links in place
 *   to_array(head, out, cap)     copy at most cap values; return the count
 *   list_free(&head)             free every node and set *head = NULL
 *
 * The list owns each Node. There is no dummy node and no tail pointer.
 *
 * From TS: `array.push` and `array.filter` allocate a new array. Here every
 * insert allocates one node, every removal frees one, and the `next` pointers
 * are the only links; losing a link leaks the rest of the chain.
 */
#include "cztest.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

void insert_sorted(Node **head, int value)
{
    /* TODO: allocate a node, walk with a Node **slot until *slot is NULL or
     * (*slot)->value >= value, then link the new node in front of *slot. */
    (void)head;
    (void)value;
}

void remove_duplicates(Node *head)
{
    /* TODO: walk the sorted chain. When head->next has the same value as
     * head, unlink and free it; otherwise advance. */
    (void)head;
}

void reverse(Node **head)
{
    /* TODO: relink each node so the chain ends up in the opposite order. */
    (void)head;
}

size_t to_array(const Node *head, int *out, size_t cap)
{
    size_t count = 0;
    for (const Node *node = head; node != NULL && count < cap;
         node = node->next) {
        out[count++] = node->value;
    }
    return count;
}

void list_free(Node **head)
{
    Node *node = *head;
    while (node != NULL) {
        Node *next = node->next;
        free(node);
        node = next;
    }
    *head = NULL;
}

int main(void)
{
    Node *head = NULL;

    CZ_CHECK_INT((long)to_array(head, NULL, 0), 0);

    const int values[] = {5, 1, 3, 3, 2, 5, 1};
    for (size_t i = 0; i < sizeof values / sizeof values[0]; ++i) {
        insert_sorted(&head, values[i]);
    }

    int out[8] = {0};
    size_t count = to_array(head, out, 8);
    CZ_CHECK_INT((long)count, 7);
    if (count == 7) {
        const int expected[] = {1, 1, 2, 3, 3, 5, 5};
        for (int i = 0; i < 7; ++i) {
            CZ_CHECK_INT(out[i], expected[i]);
        }
    }

    remove_duplicates(head);
    count = to_array(head, out, 8);
    CZ_CHECK_INT((long)count, 4);
    if (count == 4) {
        CZ_CHECK_INT(out[0], 1);
        CZ_CHECK_INT(out[1], 2);
        CZ_CHECK_INT(out[2], 3);
        CZ_CHECK_INT(out[3], 5);
    }

    reverse(&head);
    count = to_array(head, out, 8);
    CZ_CHECK_INT((long)count, 4);
    if (count == 4) {
        CZ_CHECK_INT(out[0], 5);
        CZ_CHECK_INT(out[1], 3);
        CZ_CHECK_INT(out[2], 2);
        CZ_CHECK_INT(out[3], 1);
    }

    count = to_array(head, out, 2);
    CZ_CHECK_INT((long)count, 2);
    if (count == 2) {
        CZ_CHECK_INT(out[0], 5);
        CZ_CHECK_INT(out[1], 3);
    }

    list_free(&head);
    CZ_CHECK(head == NULL);
    CZ_CHECK_INT((long)to_array(head, out, 8), 0);
    list_free(&head);
    CZ_CHECK(head == NULL);

    return cztest_summary();
}

// I AM NOT DONE

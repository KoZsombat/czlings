/* zh2_list -- final ZH, task 2 (15 points). Reference solution. */
#include "cztest.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

void insert_sorted(Node **head, int value)
{
    Node *node = malloc(sizeof *node);
    if (node == NULL) {
        return;
    }
    node->value = value;

    Node **slot = head;
    while (*slot != NULL && (*slot)->value < value) {
        slot = &(*slot)->next;
    }
    node->next = *slot;
    *slot = node;
}

void remove_duplicates(Node *head)
{
    Node *node = head;
    while (node != NULL && node->next != NULL) {
        if (node->next->value == node->value) {
            Node *duplicate = node->next;
            node->next = duplicate->next;
            free(duplicate);
        } else {
            node = node->next;
        }
    }
}

void reverse(Node **head)
{
    Node *previous = NULL;
    Node *node = *head;
    while (node != NULL) {
        Node *next = node->next;
        node->next = previous;
        previous = node;
        node = next;
    }
    *head = previous;
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

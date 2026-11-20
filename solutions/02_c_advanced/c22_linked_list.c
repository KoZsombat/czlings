/* c22_linked_list: a node owns its next pointer; the list owns every node.
 *
 * Finish push_front() so it links a new node in front of the current head,
 * make list_length() and list_sum() walk the whole chain, and make list_free()
 * release every node.  Freeing only the head leaks the rest.
 *
 * Coming from TypeScript: objects are garbage-collected and references keep
 * their targets alive.  Here each Node is a separate malloc; the `next`
 * pointer is the only link, so losing it loses the rest of the list.
 */
#include "cztest.h"
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

void push_front(Node **head, int value)
{
    Node *node = malloc(sizeof *node);
    if (node == NULL) {
        return;
    }
    node->value = value;
    node->next = *head;
    *head = node;
}

size_t list_length(const Node *head)
{
    size_t count = 0;
    for (const Node *p = head; p != NULL; p = p->next) {
        ++count;
    }
    return count;
}

long list_sum(const Node *head)
{
    long total = 0;
    for (const Node *p = head; p != NULL; p = p->next) {
        total += p->value;
    }
    return total;
}

void list_free(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void)
{
    Node *head = NULL;
    push_front(&head, 1);
    push_front(&head, 2);
    push_front(&head, 3);

    CZ_CHECK(head != NULL && head->value == 3);
    CZ_CHECK_INT((long)list_length(head), 3);
    CZ_CHECK_INT(list_sum(head), 6);

    list_free(head);
    return cztest_summary();
}

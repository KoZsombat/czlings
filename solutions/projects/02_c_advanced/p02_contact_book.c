/* p02_contact_book -- C advanced project 2. Reference solution. */
#include "cztest.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONTACT_FAVORITE 1u
#define CONTACT_BLOCKED 2u

typedef struct Contact {
    char name[32];
    char phone[16];
    unsigned flags;
    struct Contact *next;
} Contact;

int contact_push(Contact **head, const char *name, const char *phone,
                 unsigned flags)
{
    Contact *node = malloc(sizeof *node);
    if (node == NULL) {
        return -1;
    }
    snprintf(node->name, sizeof node->name, "%s", name);
    snprintf(node->phone, sizeof node->phone, "%s", phone);
    node->flags = flags;
    node->next = *head;
    *head = node;
    return 0;
}

Contact *contact_find(Contact *head, const char *name)
{
    for (Contact *node = head; node != NULL; node = node->next) {
        if (strcmp(node->name, name) == 0) {
            return node;
        }
    }
    return NULL;
}

int contact_remove(Contact **head, const char *name)
{
    Contact **slot = head;
    while (*slot != NULL) {
        Contact *node = *slot;
        if (strcmp(node->name, name) == 0) {
            *slot = node->next;
            free(node);
            return 0;
        }
        slot = &node->next;
    }
    return -1;
}

size_t contact_count(const Contact *head)
{
    size_t count = 0;
    for (const Contact *node = head; node != NULL; node = node->next) {
        ++count;
    }
    return count;
}

size_t contact_count_flag(const Contact *head, unsigned flag)
{
    size_t count = 0;
    for (const Contact *node = head; node != NULL; node = node->next) {
        if ((node->flags & flag) != 0u) {
            ++count;
        }
    }
    return count;
}

void contact_free(Contact **head)
{
    Contact *node = *head;
    while (node != NULL) {
        Contact *next = node->next;
        free(node);
        node = next;
    }
    *head = NULL;
}

int main(void)
{
    Contact *head = NULL;

    CZ_CHECK_INT(contact_push(&head, "Alice", "111", CONTACT_FAVORITE), 0);
    CZ_CHECK_INT(contact_push(&head, "Bob", "222", CONTACT_BLOCKED), 0);
    CZ_CHECK_INT(
        contact_push(&head, "Carol", "333",
                     CONTACT_FAVORITE | CONTACT_BLOCKED),
        0);

    CZ_CHECK_INT((long)contact_count(head), 3);
    CZ_CHECK_INT((long)contact_count_flag(head, CONTACT_FAVORITE), 2);
    CZ_CHECK_INT((long)contact_count_flag(head, CONTACT_BLOCKED), 2);

    Contact *found = contact_find(head, "Bob");
    CZ_CHECK(found != NULL);
    if (found != NULL) {
        CZ_CHECK_STR(found->phone, "222");
        CZ_CHECK_INT(found->flags, CONTACT_BLOCKED);
    }
    CZ_CHECK(contact_find(head, "Nobody") == NULL);

    CZ_CHECK_INT(contact_remove(&head, "Bob"), 0);
    CZ_CHECK_INT((long)contact_count(head), 2);
    CZ_CHECK(contact_find(head, "Bob") == NULL);
    CZ_CHECK(contact_find(head, "Alice") != NULL);
    CZ_CHECK(contact_find(head, "Carol") != NULL);

    CZ_CHECK_INT(contact_remove(&head, "Alice"), 0);
    CZ_CHECK_INT((long)contact_count(head), 1);
    CZ_CHECK(head != NULL);
    if (head != NULL) {
        CZ_CHECK_STR(head->name, "Carol");
    }
    CZ_CHECK_INT(contact_remove(&head, "Alice"), -1);

    contact_free(&head);
    CZ_CHECK(head == NULL);
    CZ_CHECK_INT((long)contact_count(head), 0);
    contact_free(&head);
    CZ_CHECK(head == NULL);

    return cztest_summary();
}

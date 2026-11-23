/* p02_contact_book -- C advanced project 2.
 *
 * A singly linked contact list. This project combines structs, C strings,
 * manual memory, ownership and bit flags.
 *
 *   typedef struct Contact {
 *       char name[32];
 *       char phone[16];
 *       unsigned flags;          // CONTACT_FAVORITE / CONTACT_BLOCKED
 *       struct Contact *next;
 *   } Contact;
 *
 *   contact_push(&head, name, phone, flags)  add at the front; 0 / -1
 *   contact_find(head, name)                 first exact-name match or NULL
 *   contact_remove(&head, name)              unlink and free; 0 / -1
 *   contact_count(head)                      number of contacts
 *   contact_count_flag(head, flag)           contacts with that flag set
 *   contact_free(&head)                      free every node, set *head = NULL
 *
 * Name and phone are fixed-size arrays inside the node, so one free() per node
 * releases everything. push() truncates safely and NUL-terminates. remove()
 * must handle the head, the middle and a missing name.
 *
 * From TS: objects are reference-counted and a Map removal is one call. Here
 * each node is a separate malloc, `next` is the only link, and remove() must
 * unlink before free or the rest of the list is lost.
 */
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
    /* TODO: allocate one node, copy name and phone with snprintf, store the
     * flags and link the node in front of *head. Return 0, or -1 if malloc
     * fails. */
    (void)head;
    (void)name;
    (void)phone;
    (void)flags;
    return -1;
}

Contact *contact_find(Contact *head, const char *name)
{
    /* TODO: walk the chain and return the first node whose name equals name
     * with strcmp, or NULL. */
    (void)head;
    (void)name;
    return NULL;
}

int contact_remove(Contact **head, const char *name)
{
    /* TODO: walk with a Contact **slot; when the current node matches, unlink
     * it, free it, and return 0. Return -1 when the name is not found. */
    (void)head;
    (void)name;
    return -1;
}

size_t contact_count(const Contact *head)
{
    /* TODO: walk the chain and count the nodes. */
    (void)head;
    return 0;
}

size_t contact_count_flag(const Contact *head, unsigned flag)
{
    /* TODO: count nodes where (node->flags & flag) != 0. */
    (void)head;
    (void)flag;
    return 0;
}

void contact_free(Contact **head)
{
    /* TODO: free every node and set *head = NULL. */
    (void)head;
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

// I AM NOT DONE

/* p00_receipt -- beginner project 2. Reference solution. */
#include "cztest.h"

#include <stddef.h>

typedef struct Item {
    const char *name;
    int unit_cents;
    int quantity;
} Item;

int item_total(const Item *item)
{
    return item->unit_cents * item->quantity;
}

int receipt_total(const Item *items, size_t count)
{
    int total = 0;
    for (size_t i = 0; i < count; ++i) {
        total += item_total(&items[i]);
    }
    return total;
}

int apply_discount(int total_cents)
{
    if (total_cents >= 5000) {
        return total_cents - total_cents / 10;
    }
    return total_cents;
}

int count_expensive(const Item *items, size_t count, int unit_cents)
{
    int matches = 0;
    for (size_t i = 0; i < count; ++i) {
        if (items[i].unit_cents >= unit_cents) {
            ++matches;
        }
    }
    return matches;
}

int main(void)
{
    const Item items[] = {
        {"coffee", 450, 2},
        {"bagel", 300, 3},
        {"tea", 250, 1}
    };
    const size_t item_count = sizeof items / sizeof items[0];

    CZ_CHECK_INT(item_total(&items[0]), 900);
    CZ_CHECK_INT(item_total(&items[1]), 900);
    CZ_CHECK_INT(receipt_total(items, item_count), 2050);

    CZ_CHECK_INT(apply_discount(4999), 4999);
    CZ_CHECK_INT(apply_discount(5000), 4500);
    CZ_CHECK_INT(apply_discount(2050), 2050);

    CZ_CHECK_INT(count_expensive(items, item_count, 300), 2);
    CZ_CHECK_INT(count_expensive(items, item_count, 1000), 0);

    return cztest_summary();
}

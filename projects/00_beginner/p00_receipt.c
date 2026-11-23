/* p00_receipt -- beginner project 2.
 *
 * Structs, arrays and functions: total a small shopping receipt.
 *
 *   typedef struct Item { const char *name; int unit_cents; int quantity; };
 *
 *   item_total(item)                 unit_cents * quantity
 *   receipt_total(items, count)      sum of every item_total
 *   apply_discount(total_cents)      10% off when the total is at least 5000
 *                                    cents; otherwise unchanged
 *   count_expensive(items, count, c) items whose unit price is >= c
 *
 * All money stays in integer cents.
 *
 * From TS: objects are cheap and numbers are floats. Here the struct is a
 * plain value, the array has no length field, and money is an int so 0.1 +
 * 0.2 never becomes 0.30000000000000004.
 */
#include "cztest.h"

#include <stddef.h>

typedef struct Item {
    const char *name;
    int unit_cents;
    int quantity;
} Item;

int item_total(const Item *item)
{
    /* TODO: return unit_cents * quantity. */
    (void)item;
    return 0;
}

int receipt_total(const Item *items, size_t count)
{
    /* TODO: loop over the items and add item_total(&items[i]). */
    (void)items;
    (void)count;
    return 0;
}

int apply_discount(int total_cents)
{
    /* TODO: when total_cents >= 5000 return total_cents minus 10 percent;
     * otherwise return total_cents unchanged. */
    (void)total_cents;
    return 0;
}

int count_expensive(const Item *items, size_t count, int unit_cents)
{
    /* TODO: count the items whose unit_cents is >= the threshold. */
    (void)items;
    (void)count;
    (void)unit_cents;
    return 0;
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

// I AM NOT DONE

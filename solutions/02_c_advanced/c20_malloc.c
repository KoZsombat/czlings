/* c20_malloc: dynamic arrays live on the heap and grow with realloc().
 *
 * Complete vec_push() so it stores the value, increments len, and doubles the
 * capacity when full.  Keep the pointer returned by realloc: the old block may
 * have moved.  vec_free() must return the vector to its empty state.
 *
 * Coming from TypeScript: there is no garbage collector.  Every malloc family
 * result must eventually be freed, realloc can move the block, and accessing
 * data[len] is not bounds-checked.  size_t is unsigned, so count down with
 * care.
 */
#include "cztest.h"
#include <stdlib.h>

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} IntVec;

void vec_init(IntVec *v)
{
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
}

int vec_push(IntVec *v, int value)
{
    if (v->len == v->cap) {
        size_t next = v->cap == 0 ? 4 : v->cap * 2;
        int *grown = realloc(v->data, next * sizeof *grown);
        if (grown == NULL) {
            return -1;
        }
        v->data = grown;
        v->cap = next;
    }

    v->data[v->len] = value;
    v->len += 1;
    return 0;
}

void vec_free(IntVec *v)
{
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
}

int main(void)
{
    IntVec v;
    vec_init(&v);
    CZ_CHECK(v.data == NULL && v.len == 0 && v.cap == 0);

    int ok = 1;
    for (int i = 0; i < 10; ++i) {
        ok = ok && vec_push(&v, i * i) == 0;
    }
    CZ_CHECK(ok && v.len == 10);

    int sum = 0;
    for (size_t i = 0; i < v.len; ++i) {
        sum += v.data[i];
    }
    CZ_CHECK_INT(sum, 285);
    CZ_CHECK(v.cap >= 10);

    vec_free(&v);
    CZ_CHECK(v.data == NULL && v.len == 0 && v.cap == 0);

    return cztest_summary();
}

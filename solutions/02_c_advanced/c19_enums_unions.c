/* c19_enums_unions: enums name integer constants, unions overlay storage.
 *
 * Give Color the values 1, 2 and 4 (independent flags), make Word a union so
 * both members start at the same address, and give Flags a 3-bit `a` and a
 * 5-bit `b`.  Add the missing blue case to color_name().
 *
 * Coming from TypeScript: enum members are plain integers, not strings or
 * objects.  A union makes several members share bytes, and bitfields pack
 * narrow integers into a word.  Only the shared-start rule is guaranteed;
 * padding and bit order are implementation-defined, so never print raw bytes.
 */
#include "cztest.h"
#include <stddef.h>

typedef enum {
    CZ_RED = 1,
    CZ_GREEN = 2,
    CZ_BLUE = 4
} Color;

typedef union {
    unsigned int u;
    unsigned char bytes[sizeof(unsigned int)];
} Word;

typedef struct {
    unsigned int a : 3;
    unsigned int b : 5;
} Flags;

const char *color_name(Color c)
{
    switch (c) {
    case CZ_RED:
        return "red";
    case CZ_GREEN:
        return "green";
    case CZ_BLUE:
        return "blue";
    default:
        return "unknown";
    }
}

int main(void)
{
    CZ_CHECK(CZ_RED == 1 && CZ_GREEN == 2 && CZ_BLUE == 4);

    Word w;
    w.u = 0;
    CZ_CHECK((const void *)&w.u == (const void *)&w.bytes[0] &&
             offsetof(Word, bytes) == 0);

    Flags f;
    f.a = 5;
    f.b = 17;
    CZ_CHECK(f.a == 5 && f.b == 17);
    unsigned wide = 9;
    f.a = wide;
    CZ_CHECK_INT(f.a, 1);

    CZ_CHECK(strcmp(color_name(CZ_BLUE), "blue") == 0);

    return cztest_summary();
}

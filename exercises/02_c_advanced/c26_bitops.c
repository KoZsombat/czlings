/* c26_bitops: an unsigned word is a set of bits; positions count from 0.
 *
 * Fix set_bit() so setting an already-set bit is a no-op, clear_bit() so it
 * only clears, is_power_of_two() so zero is rejected, and swap_nibbles() so
 * it swaps the two nibbles of the low byte and leaves the rest alone.
 *
 * Coming from TypeScript: bitwise operators produce a signed 32-bit value and
 * shift counts are not masked.  Use `1u << n` with unsigned values; shifting
 * by the width of the type (or more) is undefined behaviour, and so is a
 * negative shift count.
 */
#include "cztest.h"

unsigned set_bit(unsigned v, unsigned n)
{
    return v + (1u << n);
}

unsigned clear_bit(unsigned v, unsigned n)
{
    return v ^ (1u << n);
}

unsigned toggle_bit(unsigned v, unsigned n)
{
    return v ^ (1u << n);
}

int test_bit(unsigned v, unsigned n)
{
    return (v & (1u << n)) != 0;
}

unsigned popcount(unsigned v)
{
    unsigned count = 0;
    while (v != 0) {
        count += v & 1u;
        v >>= 1;
    }
    return count;
}

int is_power_of_two(unsigned v)
{
    return (v & (v - 1u)) == 0;
}

unsigned swap_nibbles(unsigned v)
{
    return (v >> 8) | (v << 8);
}

int main(void)
{
    CZ_CHECK(set_bit(8u, 3) == 8u && set_bit(0u, 5) == 32u &&
             test_bit(32u, 5));
    CZ_CHECK(clear_bit(0xFu, 1) == 0xDu && clear_bit(0xDu, 1) == 0xDu);
    CZ_CHECK_INT(toggle_bit(5u, 0), 4u);

    CZ_CHECK(popcount(0xF0F0u) == 8u && is_power_of_two(64u) &&
             !is_power_of_two(0u) && !is_power_of_two(48u));

    CZ_CHECK_INT(swap_nibbles(0x12u), 0x21u);

    return cztest_summary();
}

// I AM NOT DONE

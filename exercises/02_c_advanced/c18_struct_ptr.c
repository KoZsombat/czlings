/* c18_struct_ptr: pass a struct by address when the callee must update it.
 *
 * Fix boost() so it updates the Player the pointer names, best() so it returns
 * the player with the highest score, and rename_player() so the stored name is
 * always terminated.  best() receives a read-only view and must not write.
 *
 * Note: `p->score` is shorthand for `(*p).score`.  Nothing checks the pointer
 * for NULL, and a `const Player *` parameter promises the caller that the
 * struct will not be modified.
 *
 * From TS: passing an object to a function lets the function mutate it. In C
 * that only happens through a pointer; a by-value struct argument is a copy.
 */
#include "cztest.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char name[16];
    int score;
    int plays;
} Player;

void boost(Player *p, int amount)
{
    Player local = *p;
    local.score += amount;
    local.plays += 1;
}

const Player *best(const Player *players, size_t n)
{
    if (n == 0) {
        return NULL;
    }
    return &players[0];
}

void rename_player(Player *p, const char *name)
{
    strncpy(p->name, name, sizeof p->name);
}

int main(void)
{
    Player players[] = {
        {"alice", 10, 1},
        {"bob", 30, 1},
        {"carol", 20, 1},
    };

    boost(&players[0], 5);
    CZ_CHECK(players[0].score == 15 && players[0].plays == 2);

    const Player *top = best(players, 3);
    CZ_CHECK(top != NULL && strcmp(top->name, "bob") == 0);
    CZ_CHECK(top != NULL && top->score == 30);

    rename_player(&players[1], "Zoe");
    CZ_CHECK(strcmp(players[1].name, "Zoe") == 0);

    return cztest_summary();
}

// I AM NOT DONE

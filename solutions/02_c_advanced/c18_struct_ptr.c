/* c18_struct_ptr: pass a struct by address when the callee must update it.
 *
 * Fix boost() so it updates the Player the pointer names, best() so it returns
 * the player with the highest score, and rename_player() so the stored name is
 * always terminated.  best() receives a read-only view and must not write.
 *
 * Coming from TypeScript: `p->score` means `(*p).score`; there is no implicit
 * dereference and no automatic null check.  A `const Player *` parameter
 * promises the caller that the struct will not be modified.
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
    p->score += amount;
    p->plays += 1;
}

const Player *best(const Player *players, size_t n)
{
    if (n == 0) {
        return NULL;
    }

    const Player *winner = &players[0];
    for (size_t i = 1; i < n; ++i) {
        if (players[i].score > winner->score) {
            winner = &players[i];
        }
    }
    return winner;
}

void rename_player(Player *p, const char *name)
{
    snprintf(p->name, sizeof p->name, "%s", name);
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

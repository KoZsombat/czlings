/* p02_log_analyzer: a small log book in C, built in three levels.
 *
 * C advanced capstone. It combines the whole C chapter: structs,
 * ownership, dynamic arrays, C strings, parsing, bit masks, qsort and stdio.
 * Work through the levels in order; the checks at the bottom cover all three.
 *
 * Level 1 -- an owning dynamic array
 *   logbook_init()    start with an empty book
 *   logbook_push()    grow the buffer and append one entry
 *   logbook_free()    release the buffer and reset the book
 *
 * Level 2 -- parsing and filters
 *   level_from_name()             "warn" / "WARN" -> LOG_WARN
 *   logbook_parse()               "<timestamp> <LEVEL> <message...>" lines
 *   logbook_count_mask()          count levels selected by a bit mask
 *   logbook_first_at_or_above()   first matching entry after a timestamp
 *
 * Level 3 -- ordering and output
 *   cmp_entry()                   timestamp asc, level asc, message asc
 *   logbook_sort()                qsort the entries
 *   logbook_write_report()        "[timestamp] LEVEL message" per line
 *
 * A LogBook owns its `items` buffer. Entries never own `message`: it is a
 * fixed-size array inside each LogEntry, so one free() releases everything.
 * push() must keep the buffer big enough before writing, and free() must leave
 * the book reusable by logbook_init().
 *
 * From TS: push() on an array grows storage for you. Here the struct owns a
 * malloc'd buffer, so you reserve capacity, copy the message into the entry,
 * and free the buffer exactly once.
 */
#include "cztest.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_MESSAGE_CAP 64

typedef enum LogLevel {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARN = 2,
    LOG_ERROR = 3
} LogLevel;

typedef struct LogEntry {
    int timestamp;
    LogLevel level;
    char message[LOG_MESSAGE_CAP];
} LogEntry;

typedef struct LogBook {
    LogEntry *items;
    size_t len;
    size_t cap;
} LogBook;

const char *level_name(LogLevel level)
{
    /* TODO: map each LogLevel to "DEBUG", "INFO", "WARN", "ERROR". */
    (void)level;
    return "?";
}

void logbook_init(LogBook *book)
{
    /* TODO: set items to NULL, len and cap to 0. */
    (void)book;
}

int logbook_push(LogBook *book, int timestamp, LogLevel level,
                 const char *message)
{
    /* TODO: if len == cap, grow (start at 4 entries, then double). Allocate
     * with realloc, copy at most LOG_MESSAGE_CAP - 1 bytes of message with
     * snprintf so the result is always NUL-terminated, store the fields, and
     * increment len. Return 0 on success and -1 on allocation failure. */
    (void)book;
    (void)timestamp;
    (void)level;
    (void)message;
    return -1;
}

void logbook_free(LogBook *book)
{
    /* TODO: free items and reset the book through logbook_init(). */
    (void)book;
}

int level_from_name(const char *name)
{
    /* TODO: compare case-insensitively with "debug", "info", "warn" and
     * "error"; return the matching LogLevel or -1. */
    (void)name;
    return -1;
}

size_t logbook_parse(LogBook *book, const char *const *lines, size_t count)
{
    /* TODO: for each line, parse `timestamp level message` with
     * sscanf(line, "%d %15s %63[^\n]", ...). Skip lines that do not produce
     * all three fields or whose level name is unknown. Append each valid line
     * with logbook_push() and return the number appended. */
    (void)book;
    (void)lines;
    (void)count;
    return 0;
}

size_t logbook_count_mask(const LogBook *book, unsigned level_mask)
{
    /* TODO: count entries whose level bit is set in level_mask. Level L
     * corresponds to (1u << L). */
    (void)book;
    (void)level_mask;
    return 0;
}

long logbook_first_at_or_above(const LogBook *book, LogLevel minimum,
                               int after_timestamp)
{
    /* TODO: return the index of the first entry, in storage order, whose
     * timestamp is greater than after_timestamp and whose level is at least
     * minimum. Return -1 when nothing matches. */
    (void)book;
    (void)minimum;
    (void)after_timestamp;
    return -1;
}

int cmp_entry(const void *a, const void *b)
{
    /* TODO: order by timestamp ascending, then level ascending, then message
     * lexicographically with strcmp. Return a negative, zero or positive int. */
    (void)a;
    (void)b;
    return 0;
}

void logbook_sort(LogBook *book)
{
    /* TODO: qsort book->items by cmp_entry when there is more than one
     * entry. */
    (void)book;
}

long logbook_write_report(const LogBook *book, FILE *out)
{
    /* TODO: write one line per entry in storage order:
     *   "[timestamp] LEVEL message\n"
     * Return the number of lines written, or -1 if an fprintf fails. */
    (void)book;
    (void)out;
    return 0;
}

int main(void)
{
    LogBook book = {NULL, 0, 0};

    /* ---- Level 1: owning dynamic array -------------------------------- */
    logbook_init(&book);
    CZ_CHECK(book.items == NULL && book.len == 0 && book.cap == 0);

    int pushed = 0;
    for (int i = 0; i < 10; ++i) {
        char message[32];
        snprintf(message, sizeof message, "event %d", i);
        if (logbook_push(&book, 100 + i, (LogLevel)(i % 4), message) == 0) {
            ++pushed;
        }
    }
    CZ_CHECK_INT(pushed, 10);
    CZ_CHECK_INT((long)book.len, 10);
    CZ_CHECK(book.cap >= 10);
    if (book.len == 10) {
        CZ_CHECK_INT(book.items[0].timestamp, 100);
        CZ_CHECK_INT(book.items[0].level, LOG_DEBUG);
        CZ_CHECK_STR(book.items[0].message, "event 0");
        CZ_CHECK_INT(book.items[9].level, LOG_INFO);
    }

    char oversized[100];
    memset(oversized, 'x', sizeof oversized);
    oversized[sizeof oversized - 1] = '\0';
    CZ_CHECK_INT(logbook_push(&book, 200, LOG_ERROR, oversized), 0);
    if (book.len == 11) {
        CZ_CHECK_INT((long)strlen(book.items[10].message),
                     LOG_MESSAGE_CAP - 1);
    }

    logbook_free(&book);
    CZ_CHECK(book.items == NULL && book.len == 0 && book.cap == 0);

    /* ---- Level 2: parsing and filters --------------------------------- */
    logbook_init(&book);

    CZ_CHECK_INT(level_from_name("debug"), LOG_DEBUG);
    CZ_CHECK_INT(level_from_name("WARN"), LOG_WARN);
    CZ_CHECK_INT(level_from_name("nope"), -1);

    const char *lines[] = {
        "40 INFO late event",
        "10 INFO boot complete",
        "25 ERROR disk full",
        "25 INFO low memory",
        "26 WARN retry scheduled",
        "5 DEBUG cache warm",
        "oops",
        "99 TRACE ignored",
        "30 WARN no message here",
        "31 ERROR trailing text"
    };
    const size_t line_count = sizeof lines / sizeof lines[0];
    CZ_CHECK_INT((long)logbook_parse(&book, lines, line_count), 8);
    CZ_CHECK_INT((long)book.len, 8);

    CZ_CHECK_INT((long)logbook_count_mask(&book,
                                          (1u << LOG_DEBUG) |
                                              (1u << LOG_WARN)),
                 3);
    CZ_CHECK_INT((long)logbook_count_mask(&book, 1u << LOG_ERROR), 2);
    CZ_CHECK_INT(logbook_first_at_or_above(&book, LOG_WARN, 5), 2);
    CZ_CHECK_INT(logbook_first_at_or_above(&book, LOG_WARN, 25), 4);
    CZ_CHECK_INT(logbook_first_at_or_above(&book, LOG_ERROR, 30), 7);
    CZ_CHECK_INT(logbook_first_at_or_above(&book, LOG_WARN, 40), -1);

    /* ---- Level 3: ordering and output --------------------------------- */
    logbook_sort(&book);
    CZ_CHECK_INT((long)book.len, 8);
    if (book.len == 8) {
        CZ_CHECK_INT(book.items[0].timestamp, 5);
        CZ_CHECK_INT(book.items[1].timestamp, 10);
        CZ_CHECK_INT(book.items[2].timestamp, 25);
        CZ_CHECK_INT(book.items[2].level, LOG_INFO);
        CZ_CHECK_INT(book.items[3].timestamp, 25);
        CZ_CHECK_INT(book.items[3].level, LOG_ERROR);
        CZ_CHECK_INT(book.items[7].timestamp, 40);
    }

    FILE *out = tmpfile();
    CZ_CHECK(out != NULL);
    if (out != NULL) {
        CZ_CHECK_INT(logbook_write_report(&book, out), 8);
        rewind(out);

        const char *expected[] = {
            "[5] DEBUG cache warm\n",
            "[10] INFO boot complete\n",
            "[25] INFO low memory\n",
            "[25] ERROR disk full\n",
            "[26] WARN retry scheduled\n",
            "[30] WARN no message here\n",
            "[31] ERROR trailing text\n",
            "[40] INFO late event\n"
        };
        char line[160];
        int index = 0;
        while (fgets(line, sizeof line, out) != NULL) {
            if (index < 8) {
                CZ_CHECK_STR(line, expected[index]);
            }
            ++index;
        }
        CZ_CHECK_INT(index, 8);
        fclose(out);
    }

    logbook_free(&book);
    return cztest_summary();
}

// I AM NOT DONE

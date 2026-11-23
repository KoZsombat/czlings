/* p02_log_analyzer: a small log book in C, built in three levels.
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
 */
#include "cztest.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
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
    switch (level) {
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_INFO:
        return "INFO";
    case LOG_WARN:
        return "WARN";
    case LOG_ERROR:
        return "ERROR";
    }
    return "?";
}

static int equals_ignore_case(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        ++a;
        ++b;
    }
    return *a == '\0' && *b == '\0';
}

void logbook_init(LogBook *book)
{
    book->items = NULL;
    book->len = 0;
    book->cap = 0;
}

static int logbook_reserve(LogBook *book, size_t needed)
{
    if (needed <= book->cap) {
        return 0;
    }
    size_t fresh_cap = book->cap == 0 ? 4 : book->cap;
    while (fresh_cap < needed) {
        if (fresh_cap > SIZE_MAX / 2) {
            return -1;
        }
        fresh_cap *= 2;
    }
    if (fresh_cap > SIZE_MAX / sizeof(LogEntry)) {
        return -1;
    }
    LogEntry *fresh = realloc(book->items, fresh_cap * sizeof *fresh);
    if (fresh == NULL) {
        return -1;
    }
    book->items = fresh;
    book->cap = fresh_cap;
    return 0;
}

int logbook_push(LogBook *book, int timestamp, LogLevel level,
                 const char *message)
{
    if (logbook_reserve(book, book->len + 1) != 0) {
        return -1;
    }
    LogEntry *entry = &book->items[book->len];
    entry->timestamp = timestamp;
    entry->level = level;
    snprintf(entry->message, sizeof entry->message, "%s",
             message == NULL ? "" : message);
    ++book->len;
    return 0;
}

void logbook_free(LogBook *book)
{
    free(book->items);
    logbook_init(book);
}

int level_from_name(const char *name)
{
    if (equals_ignore_case(name, "debug")) {
        return LOG_DEBUG;
    }
    if (equals_ignore_case(name, "info")) {
        return LOG_INFO;
    }
    if (equals_ignore_case(name, "warn")) {
        return LOG_WARN;
    }
    if (equals_ignore_case(name, "error")) {
        return LOG_ERROR;
    }
    return -1;
}

size_t logbook_parse(LogBook *book, const char *const *lines, size_t count)
{
    size_t parsed = 0;
    for (size_t i = 0; i < count; ++i) {
        int timestamp = 0;
        char name[16];
        char message[LOG_MESSAGE_CAP];
        if (sscanf(lines[i], "%d %15s %63[^\n]", &timestamp, name, message) !=
            3) {
            continue;
        }
        const int level = level_from_name(name);
        if (level < 0) {
            continue;
        }
        if (logbook_push(book, timestamp, (LogLevel)level, message) == 0) {
            ++parsed;
        }
    }
    return parsed;
}

size_t logbook_count_mask(const LogBook *book, unsigned level_mask)
{
    size_t count = 0;
    for (size_t i = 0; i < book->len; ++i) {
        const unsigned bit = 1u << (unsigned)book->items[i].level;
        if ((level_mask & bit) != 0u) {
            ++count;
        }
    }
    return count;
}

long logbook_first_at_or_above(const LogBook *book, LogLevel minimum,
                               int after_timestamp)
{
    for (size_t i = 0; i < book->len; ++i) {
        const LogEntry *entry = &book->items[i];
        if (entry->timestamp > after_timestamp && entry->level >= minimum) {
            return (long)i;
        }
    }
    return -1;
}

int cmp_entry(const void *a, const void *b)
{
    const LogEntry *left = (const LogEntry *)a;
    const LogEntry *right = (const LogEntry *)b;

    if (left->timestamp != right->timestamp) {
        return (left->timestamp > right->timestamp) -
               (left->timestamp < right->timestamp);
    }
    if (left->level != right->level) {
        return (left->level > right->level) - (left->level < right->level);
    }
    return strcmp(left->message, right->message);
}

void logbook_sort(LogBook *book)
{
    if (book->len > 1) {
        qsort(book->items, book->len, sizeof *book->items, cmp_entry);
    }
}

long logbook_write_report(const LogBook *book, FILE *out)
{
    long written = 0;
    for (size_t i = 0; i < book->len; ++i) {
        const LogEntry *entry = &book->items[i];
        if (fprintf(out, "[%d] %s %s\n", entry->timestamp,
                    level_name(entry->level), entry->message) < 0) {
            return -1;
        }
        ++written;
    }
    return written;
}

int main(void)
{
    LogBook book;

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

/* c24_file_binary: fwrite/fread move raw bytes; fseek measures in bytes.
 *
 * Fix write_records() so it writes all n records, count_records() so it
 * divides the file size by sizeof(Record), and read_last() so it seeks to the
 * final record and reads it.  Use fseek/ftell for positions.
 *
 * Note: the bytes written are the in-memory representation, padding included,
 * so the file is not portable between builds.  fread() returns the number of
 * complete items read, which may be short.
 *
 * From TS: JSON serializes fields by name. C writes the struct's raw bytes, so
 * padding and native byte order end up in the file.
 */
#include "cztest.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {
    int id;
    double score;
} Record;

int write_records(FILE *f, const Record *records, size_t n)
{
    size_t written = fwrite(records, sizeof *records, n > 0 ? n - 1 : 0, f);
    return written == n ? 0 : -1;
}

size_t count_records(FILE *f)
{
    if (fseek(f, 0, SEEK_END) != 0) {
        return 0;
    }
    long bytes = ftell(f);
    rewind(f);
    if (bytes < 0) {
        return 0;
    }
    return (size_t)bytes / sizeof(int);
}

int read_last(FILE *f, Record *out)
{
    if (fseek(f, -(long)sizeof *out, SEEK_END) != 0) {
        return -1;
    }
    if (fread(out, sizeof *out, 1, f) != 1) {
        return -1;
    }
    return 0;
}

int main(void)
{
    FILE *f = tmpfile();
    Record records[] = {{101, 1.5}, {102, 2.5}, {103, 3.5}};

    CZ_CHECK(f != NULL && write_records(f, records, 3) == 0);
    CZ_CHECK_INT((long)count_records(f), 3);

    Record last;
    CZ_CHECK_INT(read_last(f, &last), 0);
    CZ_CHECK(last.id == 103);
    CZ_CHECK_NEAR(last.score, 3.5, 1e-9);

    fclose(f);
    return cztest_summary();
}

// I AM NOT DONE

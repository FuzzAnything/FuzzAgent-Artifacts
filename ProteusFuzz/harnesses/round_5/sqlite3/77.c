// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_length at sqlite3.c:33215:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_str API functions.
 * Target functions:
 * - sqlite3_str_errcode
 * - sqlite3_str_value
 * - sqlite3_str_appendchar
 * - sqlite3_str_length
 * - sqlite3_str_append
 * - sqlite3_str_truncate
 */

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    int rc;

    /* Optional: open a database to provide a valid db pointer for sqlite3_str_new */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If we can't open a db, we can still test with NULL db pointer */
        db = NULL;
    }

    /* Create a new sqlite3_str object */
    str = sqlite3_str_new(db);
    if (str == NULL) {
        /* Cannot proceed without a valid string object */
        if (db) sqlite3_close(db);
        return 0;
    }

    /* Use input data to drive various operations */
    size_t idx = 0;

    /* 1. Append characters from input */
    if (idx + 1 < Size) {
        int n = Data[idx++];
        char c = (char)Data[idx++];
        sqlite3_str_appendchar(str, n, c);
    }

    /* 2. Append a substring from input */
    if (idx < Size) {
        int len = Data[idx++];
        if (len > (int)(Size - idx)) {
            len = (int)(Size - idx);
        }
        if (len > 0 && idx < Size) {
            sqlite3_str_append(str, (const char *)&Data[idx], len);
            idx += len;
        }
    }

    /* 3. Truncate the string */
    if (idx < Size) {
        int truncate_len = Data[idx++];
        sqlite3_str_truncate(str, truncate_len);
    }

    /* 4. Check error code */
    int err = sqlite3_str_errcode(str);
    (void)err; /* Use the return value to avoid unused warnings */

    /* 5. Get string length */
    int len = sqlite3_str_length(str);
    (void)len;

    /* 6. Get string value (this may append null terminator) */
    char *val = sqlite3_str_value(str);
    (void)val;

    /* 7. Perform additional random operations to explore more states */
    if (idx + 1 < Size) {
        int n2 = Data[idx++];
        char c2 = (char)Data[idx++];
        sqlite3_str_appendchar(str, n2, c2);
    }

    if (idx < Size) {
        int len2 = Data[idx++];
        if (len2 > (int)(Size - idx)) {
            len2 = (int)(Size - idx);
        }
        if (len2 > 0 && idx < Size) {
            sqlite3_str_append(str, (const char *)&Data[idx], len2);
            idx += len2;
        }
    }

    if (idx < Size) {
        int truncate_len2 = Data[idx++];
        sqlite3_str_truncate(str, truncate_len2);
    }

    /* Finalize the string object to prevent memory leaks */
    sqlite3_str_finish(str);

    /* Cleanup database if opened */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
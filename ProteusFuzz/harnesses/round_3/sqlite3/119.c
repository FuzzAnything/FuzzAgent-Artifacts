// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

/*
 * Fuzz driver for sqlite3_str* API functions.
 * Fixed to avoid passing invalid format strings or arguments that cause ASAN errors.
 */

static void cleanup_string(sqlite3_str *str) {
    if (str) {
        char *result = sqlite3_str_finish(str);
        if (result) {
            sqlite3_free(result);
        }
    }
}

static int try_appendf_safe(sqlite3_str *str, const uint8_t *data, size_t size) {
    if (!str || !data || size == 0) return 0;

    /* Create a safe, null-terminated format string */
    char *fmt = (char *)sqlite3_malloc(size + 1);
    if (!fmt) return 0;

    memcpy(fmt, data, size);
    fmt[size] = '\0';

    /* Sanitize format string: remove or replace '%' to prevent format string bugs */
    /* Also remove null bytes that might truncate the string unexpectedly */
    for (size_t i = 0; i < size; i++) {
        if (fmt[i] == '\0') {
            fmt[i] = ' ';
        }
        /* Replace % with %% to escape it, or just skip it to avoid parsing errors */
        if (fmt[i] == '%') {
            fmt[i] = ' '; 
        }
    }

    /* Use only safe argument types that match the sanitized format string */
    /* Since we removed %, we can pass a simple string argument safely */
    sqlite3_str_appendf(str, fmt, "safe_arg");

    sqlite3_free(fmt);
    return 1;
}

static int try_append_raw(sqlite3_str *str, const uint8_t *data, size_t size) {
    if (!str || !data || size == 0) return 0;

    /* Append raw bytes, ensuring N is within bounds */
    int n = (size % 100) + 1;
    if (n > (int)size) n = (int)size;

    sqlite3_str_append(str, (const char *)data, n);
    return 1;
}

static int try_appendchar(sqlite3_str *str, const uint8_t *data, size_t size) {
    if (!str || size == 0) return 0;

    char c = data[0];
    int n = (size % 50) + 1;

    sqlite3_str_appendchar(str, n, c);
    return 1;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    int ret;

    /* Initialize database for optional length limit enforcement */
    ret = sqlite3_open(":memory:", &db);
    if (ret != SQLITE_OK) {
        /* If DB open fails, we can still test with NULL DB */
        db = NULL;
    }

    /* Create string object */
    str = sqlite3_str_new(db);
    if (!str) {
        /* Should not happen per docs, but handle gracefully */
        if (db) sqlite3_close(db);
        return 0;
    }

    /* Partition input for different operations */
    size_t offset = 0;
    size_t chunk_size = Size / 4;
    if (chunk_size == 0) chunk_size = Size;

    /* 1. Try appendf with first chunk */
    if (offset + chunk_size <= Size) {
        try_appendf_safe(str, Data + offset, chunk_size);
        offset += chunk_size;
    }

    /* 2. Try append raw with second chunk */
    if (offset + chunk_size <= Size) {
        try_append_raw(str, Data + offset, chunk_size);
        offset += chunk_size;
    } else if (offset < Size) {
        try_append_raw(str, Data + offset, Size - offset);
        offset = Size;
    }

    /* 3. Try appendchar with third chunk */
    if (offset + chunk_size <= Size) {
        try_appendchar(str, Data + offset, chunk_size);
        offset += chunk_size;
    } else if (offset < Size) {
        try_appendchar(str, Data + offset, Size - offset);
        offset = Size;
    }

    /* 4. Retrieve value without finishing (for inspection) */
    char *val = sqlite3_str_value(str);
    if (val) {
        /* Just read, don't free - sqlite3_str_value does not allocate */
        (void)val;
    }

    /* 5. Finish and cleanup */
    char *result = sqlite3_str_finish(str);
    if (result) {
        sqlite3_free(result);
    }

    /* Cleanup database */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
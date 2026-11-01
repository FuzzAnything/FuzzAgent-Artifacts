// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

/*
 * Fuzz driver for sqlite3_str* functions.
 * Tests: sqlite3_str_new, sqlite3_str_appendf, sqlite3_str_append,
 *        sqlite3_str_appendall, sqlite3_str_value, sqlite3_str_finish.
 */

static void safe_cleanup(sqlite3_str *str, char *buf) {
    if (str) {
        /* If we haven't finished, finish to free resources */
        char *tmp = sqlite3_str_finish(str);
        if (tmp && tmp != buf) {
            sqlite3_free(tmp);
        }
        /* Note: str is invalid after finish, so don't use it again */
    }
    if (buf) {
        sqlite3_free(buf);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy database connection to pass to sqlite3_str_new if needed */
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If we can't open a DB, we can still test with NULL db parameter */
        db = NULL;
    }

    sqlite3_str *str = NULL;
    char *result = NULL;

    /* Initialize string builder */
    str = sqlite3_str_new(db);
    if (!str) {
        /* Should not happen per docs, but handle gracefully */
        if (db) sqlite3_close(db);
        return 0;
    }

    /* Use input data to drive various append operations */
    size_t idx = 0;

    /* 1. Try sqlite3_str_appendf with format strings derived from input */
    /* We construct a simple format string to avoid complex parsing issues */
    if (idx < Size) {
        /* Use a safe format string, treating input as data, not format */
        /* We'll use %s and pass a substring of Data as the argument */
        /* Ensure we have at least 2 bytes for a simple test */
        if (idx + 1 < Size) {
            /* Create a temporary buffer for the argument to ensure null termination */
            char arg_buf[256];
            size_t arg_len = (Size - idx) > 255 ? 255 : (Size - idx);
            memcpy(arg_buf, &Data[idx], arg_len);
            arg_buf[arg_len] = '\0';
            
            /* Try appending with a format string */
            sqlite3_str_appendf(str, "Value: %s", arg_buf);
            idx += arg_len;
        }
    }

    /* 2. Try sqlite3_str_appendall */
    if (idx < Size) {
        /* Ensure null termination for appendall */
        char *p = (char*)&Data[idx];
        /* Find the next null or limit to 1000 chars to avoid huge strings */
        size_t len = 0;
        while (idx + len < Size && len < 1000 && Data[idx + len] != '\0') {
            len++;
        }
        if (len > 0) {
            char temp[1001];
            memcpy(temp, &Data[idx], len);
            temp[len] = '\0';
            sqlite3_str_appendall(str, temp);
            idx += len;
        }
    }

    /* 3. Try sqlite3_str_append with specific length */
    if (idx < Size) {
        int n = (Data[idx] % 100) + 1;
        idx++;
        if (idx + (size_t)n <= Size) {
            sqlite3_str_append(str, (const char*)&Data[idx], n);
            idx += n;
        }
    }

    /* 4. Try sqlite3_str_value to inspect current state */
    char *val = sqlite3_str_value(str);
    if (val) {
        /* Just read it, don't free it as it's internal to str */
        (void)val;
    }

    /* 5. Finalize and check result */
    result = sqlite3_str_finish(str);
    str = NULL; /* str is now invalid */

    if (result) {
        /* Do something with result to keep compiler happy, then free */
        size_t len = strlen(result);
        if (len > 0) {
            /* Access first byte */
            volatile char c = result[0];
            (void)c;
        }
        sqlite3_free(result);
        result = NULL;
    }

    /* Cleanup database */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
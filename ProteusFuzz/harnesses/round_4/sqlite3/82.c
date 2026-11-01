// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

/*
 * Fuzz driver for sqlite3_str API functions.
 * Target functions:
 *   sqlite3_str_new
 *   sqlite3_str_appendf
 *   sqlite3_str_value
 *   sqlite3_str_reset
 *   sqlite3_str_finish
 *   sqlite3_str_free
 */

static void safe_str_cleanup(sqlite3_str *str) {
    if (str == NULL) return;
    /* Try to finish first to get the string and free the object */
    char *result = sqlite3_str_finish(str);
    if (result != NULL) {
        sqlite3_free(result);
    }
    /* If finish returned NULL, the object might be an OOM singleton.
     * The object is destroyed by finish.
     */
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Initialize a database connection. 
     * We use :memory: to avoid file system dependencies and keep it fast.
     */
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If we can't open, we can't test much, but we shouldn't crash. */
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Create an sqlite3_str object.
     * We pass 'db' to potentially respect length limits if set.
     */
    sqlite3_str *str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Parse the input data to drive the fuzzing logic.
     * We interpret the input bytes to decide which operations to perform.
     */
    size_t idx = 0;
    
    /* Use a simple state machine based on input bytes */
    while (idx < Size) {
        uint8_t op = Data[idx];
        idx++;

        if (op == 0) {
            /* Operation: Reset */
            sqlite3_str_reset(str);
        } 
        else if (op == 1) {
            /* Operation: Append a single char */
            char c = (idx < Size) ? (char)Data[idx] : 'a';
            if (idx < Size) idx++;
            sqlite3_str_appendchar(str, 1, c);
        }
        else if (op == 2) {
            /* Operation: Append formatted string (simple) */
            /* We construct a safe format string from input to avoid crashes */
            if (idx < Size) {
                char c = (char)Data[idx];
                idx++;
                /* Only allow %c or %d or %s to be safe, but we control args */
                /* Let's just append a fixed format with a char arg */
                sqlite3_str_appendf(str, "char:%c", c);
            }
        }
        else if (op == 3) {
            /* Operation: Append formatted string (integer) */
            if (idx < Size) {
                int val = (int)Data[idx];
                idx++;
                sqlite3_str_appendf(str, "int:%d", val);
            }
        }
        else if (op == 4) {
            /* Operation: Append formatted string (string) */
            /* We take a small chunk of data as the string */
            size_t len = 0;
            if (idx < Size) {
                len = Data[idx];
                idx++;
            }
            /* Limit length to avoid huge allocations or OOM in fuzzer */
            if (len > 100) len = 100;
            
            if (idx + len <= Size) {
                /* Create a temporary buffer with a null terminator */
                char *tmp = (char *)sqlite3_malloc(len + 1);
                if (tmp) {
                    memcpy(tmp, &Data[idx], len);
                    tmp[len] = '\0';
                    idx += len;
                    sqlite3_str_appendf(str, "str:%s", tmp);
                    sqlite3_free(tmp);
                }
            }
        }
        else if (op == 5) {
            /* Operation: Truncate */
            if (idx < Size) {
                int n = (int)Data[idx];
                idx++;
                sqlite3_str_truncate(str, n);
            }
        }
        else if (op == 6) {
            /* Operation: Check value (read-only) */
            char *val = sqlite3_str_value(str);
            if (val != NULL) {
                /* Just read it, don't free it. It's internal to the str object. */
                (void)val; 
            }
        }
        else if (op == 7) {
            /* Operation: Check error code */
            int err = sqlite3_str_errcode(str);
            if (err != SQLITE_OK) {
                /* If there's an error, we might want to reset or finish to recover */
                sqlite3_str_reset(str);
            }
        }
        else {
            /* Default: Append raw byte as char */
            sqlite3_str_appendchar(str, 1, (char)op);
        }
    }

    /* 
     * Finalize the string builder.
     * This returns a malloc'd string that we must free.
     */
    char *result = sqlite3_str_finish(str);
    if (result != NULL) {
        sqlite3_free(result);
    }

    /* 
     * Cleanup database connection.
     */
    sqlite3_close(db);

    return 0;
}
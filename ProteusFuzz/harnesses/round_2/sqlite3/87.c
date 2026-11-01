// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_free at sqlite3.c:33251:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

/*
 * Fuzz driver for sqlite3_str API functions.
 * Target functions:
 *   sqlite3_str_new
 *   sqlite3_str_appendf
 *   sqlite3_str_reset
 *   sqlite3_str_finish
 *   sqlite3_str_free
 *   sqlite3_str_value
 */

static void safe_free_result(char *ptr) {
    if (ptr != NULL) {
        sqlite3_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    char *result = NULL;
    int rc;

    /* Initialize database connection (optional for sqlite3_str_new, but good for coverage) */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If open fails, we can still test with NULL db */
        db = NULL;
    }

    /* Create a new dynamic string object */
    str = sqlite3_str_new(db);
    if (str == NULL) {
        /* sqlite3_str_new should always return a valid pointer (even on OOM),
           but handle gracefully just in case */
        if (db) sqlite3_close(db);
        return 0;
    }

    /*
     * Use input data to drive fuzzing logic.
     * We interpret the input as a sequence of commands.
     * Byte 0: Command type
     * Remaining bytes: Data for the command
     */
    unsigned char cmd = Data[0] % 6;
    const uint8_t *data_ptr = Data + 1;
    size_t data_len = Size - 1;

    switch (cmd) {
        case 0: {
            /* Test sqlite3_str_appendf with various formats */
            /* Ensure we have at least some data for format string */
            if (data_len > 0) {
                /* Create a safe null-terminated format string from input */
                char *fmt = sqlite3_malloc(data_len + 1);
                if (fmt) {
                    memcpy(fmt, data_ptr, data_len);
                    fmt[data_len] = '\0';

                    /* Try to append with format string */
                    /* We pass a fixed integer argument to avoid va_list complexity */
                    sqlite3_str_appendf(str, fmt, 42);

                    /* Also try with string argument if possible */
                    if (data_len > 1) {
                        char *arg = sqlite3_malloc(data_len);
                        if (arg) {
                            memcpy(arg, data_ptr, data_len - 1);
                            arg[data_len - 1] = '\0';
                            sqlite3_str_appendf(str, "Value: %s", arg);
                            sqlite3_free(arg);
                        }
                    }

                    sqlite3_free(fmt);
                }
            }
            break;
        }
        case 1: {
            /* Test sqlite3_str_reset */
            sqlite3_str_reset(str);
            break;
        }
        case 2: {
            /* Test sqlite3_str_finish */
            result = sqlite3_str_finish(str);
            /* Result must be freed by caller */
            safe_free_result(result);
            /* Re-create str for further testing if needed, but we'll just free it later */
            str = sqlite3_str_new(db);
            break;
        }
        case 3: {
            /* Test sqlite3_str_value */
            char *val = sqlite3_str_value(str);
            if (val != NULL) {
                /* Just access the string to ensure it's valid */
                (void)strlen(val);
            }
            /* Note: sqlite3_str_value modifies the buffer, but does not free it.
               We must still call sqlite3_str_finish or sqlite3_str_free later. */
            break;
        }
        case 4: {
            /* Test sqlite3_str_free directly */
            sqlite3_str_free(str);
            str = NULL;
            break;
        }
        case 5: {
            /* Mixed operations: append, reset, append, finish */
            if (data_len > 0) {
                char *fmt = sqlite3_malloc(data_len + 1);
                if (fmt) {
                    memcpy(fmt, data_ptr, data_len);
                    fmt[data_len] = '\0';
                    sqlite3_str_appendf(str, fmt, 123);
                    sqlite3_str_reset(str);
                    sqlite3_str_appendf(str, "Reset and appended", 456);
                    sqlite3_free(fmt);
                }
            }
            break;
        }
        default:
            break;
    }

    /* Cleanup: If str is still valid, finish it to free resources */
    if (str != NULL) {
        result = sqlite3_str_finish(str);
        safe_free_result(result);
        str = NULL;
    }

    /* Close database connection if it was opened */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
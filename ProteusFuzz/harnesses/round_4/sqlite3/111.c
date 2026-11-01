// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:33251:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

/*
 * Fuzz driver for sqlite3 dynamic string API functions:
 * - sqlite3_str_new
 * - sqlite3_str_appendf
 * - sqlite3_str_reset
 * - sqlite3_str_value
 * - sqlite3_str_finish
 * - sqlite3_str_free
 */

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    char *result = NULL;

    // Optional: create a database connection to pass to sqlite3_str_new
    // Using ":memory:" to avoid file system dependencies
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        // If opening fails, we can still test with NULL db
        db = NULL;
    }

    // Create a new dynamic string object
    str = sqlite3_str_new(db);
    if (str == NULL) {
        // Should not happen per documentation, but handle gracefully
        if (db) sqlite3_close(db);
        return 0;
    }

    // Use input data to generate format strings and arguments
    size_t offset = 0;
    while (offset < Size) {
        // Extract a small chunk for format string
        size_t fmt_len = 1;
        if (offset + 1 < Size) {
            fmt_len = Data[offset] % 10 + 1; // 1 to 10 bytes
        }
        if (offset + fmt_len > Size) fmt_len = Size - offset;

        // Create a null-terminated format string
        char *fmt_buf = (char *)malloc(fmt_len + 1);
        if (fmt_buf == NULL) break;
        memcpy(fmt_buf, Data + offset, fmt_len);
        fmt_buf[fmt_len] = '\0';

        // Sanitize format string to avoid dangerous specifiers
        // Allocate enough space: worst case is every '%' becomes "%%"
        size_t max_safe_len = fmt_len * 2 + 1;
        char *safe_fmt = (char *)malloc(max_safe_len);
        if (safe_fmt == NULL) {
            free(fmt_buf);
            break;
        }
        
        size_t j = 0;
        for (size_t i = 0; i < fmt_len; i++) {
            if (fmt_buf[i] == '%' && i + 1 < fmt_len) {
                char next = fmt_buf[i+1];
                if (next == 's' || next == 'd' || next == 'u' || next == 'x' || 
                    next == 'X' || next == 'c' || next == 'p' || next == '%') {
                    safe_fmt[j++] = fmt_buf[i];
                    safe_fmt[j++] = fmt_buf[i+1];
                    i++; // skip next char
                } else {
                    safe_fmt[j++] = '%';
                    safe_fmt[j++] = '%'; // escape it
                }
            } else {
                safe_fmt[j++] = fmt_buf[i];
            }
        }
        safe_fmt[j] = '\0';

        // Append formatted string
        // We'll use simple arguments based on input data
        int arg_int = 0;
        if (offset + fmt_len + 4 <= Size) {
            memcpy(&arg_int, Data + offset + fmt_len, 4);
        }
        
        sqlite3_str_appendf(str, safe_fmt, arg_int);

        // Check for errors
        int err = sqlite3_str_errcode(str);
        if (err != SQLITE_OK) {
            // Error occurred, reset and continue
            sqlite3_str_reset(str);
        }

        // Occasionally reset the string to explore different states
        if ((offset % 5) == 0) {
            sqlite3_str_reset(str);
        }

        free(fmt_buf);
        free(safe_fmt);
        offset += fmt_len + 4;
        if (offset > Size) offset = Size;
    }

    // Try to get the string value
    char *val = sqlite3_str_value(str);
    if (val != NULL) {
        // String is valid, we can use it
        // No action needed, just checking
    }

    // Finalize the string
    result = sqlite3_str_finish(str);
    if (result != NULL) {
        // Free the result to prevent memory leak
        sqlite3_free(result);
    }

    // Alternative: use sqlite3_str_free instead of finish
    // Create a new string for this test
    str = sqlite3_str_new(db);
    if (str != NULL) {
        // Append some data
        sqlite3_str_appendf(str, "test");
        // Free directly
        sqlite3_str_free(str);
    }

    // Cleanup
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
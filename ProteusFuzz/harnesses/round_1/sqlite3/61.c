// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_free at sqlite3.c:33251:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_free at sqlite3.c:33251:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_str_free at sqlite3.c:33251:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void safe_fuzz_cleanup(sqlite3 *db, sqlite3_str *str, char *finished_str) {
    if (db) {
        sqlite3_close(db);
    }
    if (str && finished_str == NULL) {
        sqlite3_str_free(str);
    }
    if (finished_str) {
        sqlite3_free(finished_str);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    char *finished_str = NULL;
    int ret = 0;

    // Initialize database connection
    ret = sqlite3_open(":memory:", &db);
    if (ret != SQLITE_OK) {
        db = NULL;
    }

    // Create a new dynamic string object
    str = sqlite3_str_new(db);
    if (str == NULL) {
        safe_fuzz_cleanup(db, NULL, NULL);
        return 0;
    }

    // Fuzzing logic
    const char *input_str = (const char *)Data;
    
    // 1. Test sqlite3_str_appendf with various format strings from input
    // We try to parse the input as a format string, but limit length to avoid huge allocations
    if (Size > 0) {
        // Create a safe format string buffer
        char safe_format[256];
        size_t fmt_len = (Size < 255) ? Size : 255;
        memcpy(safe_format, input_str, fmt_len);
        safe_format[fmt_len] = '\0';
        
        // Try to append formatted string
        // We use a simple integer argument derived from the first byte to avoid complex parsing
        int arg_val = (int)input_str[0];
        sqlite3_str_appendf(str, safe_format, arg_val);
    }

    // 2. Test sqlite3_str_reset
    sqlite3_str_reset(str);

    // 3. Test sqlite3_str_appendf again after reset
    if (Size > 0) {
        char safe_format2[256];
        size_t fmt_len2 = (Size < 255) ? Size : 255;
        memcpy(safe_format2, input_str, fmt_len2);
        safe_format2[fmt_len2] = '\0';
        int arg_val2 = (int)input_str[Size > 1 ? 1 : 0];
        sqlite3_str_appendf(str, safe_format2, arg_val2);
    }

    // 4. Test sqlite3_str_value (this modifies the buffer by adding \0)
    (void)sqlite3_str_value(str);

    // 5. Test sqlite3_str_finish
    finished_str = sqlite3_str_finish(str);
    str = NULL;

    // 6. Test sqlite3_str_free with NULL (should be safe)
    sqlite3_str_free(NULL);

    // 7. Test sqlite3_str_free with a fresh object (to test the free path without finish)
    sqlite3_str *str2 = sqlite3_str_new(db);
    if (str2) {
        // Append some data safely
        // The previous crash occurred because we passed raw fuzz data as a string to "%s"
        // without ensuring it was null-terminated or within bounds.
        // We create a safe, null-terminated copy of the input data first.
        if (Size > 0) {
            // Limit the size to prevent huge allocations and ensure safety
            size_t copy_size = (Size < 1024) ? Size : 1024;
            char *safe_input = (char *)sqlite3_malloc64(copy_size + 1);
            if (safe_input) {
                memcpy(safe_input, Data, copy_size);
                safe_input[copy_size] = '\0';
                sqlite3_str_appendf(str2, "%s", safe_input);
                sqlite3_free(safe_input);
            }
        }
        // Free directly without finish
        sqlite3_str_free(str2);
    }

    // Cleanup
    safe_fuzz_cleanup(db, NULL, finished_str);

    return 0;
}
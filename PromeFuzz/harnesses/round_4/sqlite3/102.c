// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:19130:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>

static void fuzz_appendf_wrapper(sqlite3_str *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    sqlite3_str_vappendf(str, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create dynamic string builder
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) {
        return 0;
    }

    // Use first byte to determine operation mix
    uint8_t op_selector = Data[0];
    
    // Create null-terminated string from fuzzer data (skip first byte)
    size_t str_len = Size - 1;
    const char *input_str = (const char *)(Data + 1);
    
    // Always append something to initialize the string
    if (str_len > 0) {
        sqlite3_str_append(str, input_str, (int)(str_len > 100 ? 100 : str_len));
    } else {
        sqlite3_str_append(str, "fuzz", 4);
    }

    // Test sqlite3_str_appendall if we have enough data
    if (str_len > 0 && (op_selector & 0x01)) {
        // Create a null-terminated copy for appendall
        char *null_term_str = malloc(str_len + 1);
        if (null_term_str) {
            memcpy(null_term_str, input_str, str_len);
            null_term_str[str_len] = '\0';
            sqlite3_str_appendall(str, null_term_str);
            free(null_term_str);
        }
    }

    // Test sqlite3_str_append with various lengths
    if (str_len > 0 && (op_selector & 0x02)) {
        int append_len = (int)(str_len % 256);
        if (append_len > 0) {
            sqlite3_str_append(str, input_str, append_len);
        }
    }

    // Test sqlite3_str_appendf with different format strings
    if (op_selector & 0x04) {
        if (str_len > 0) {
            // Use a simple format string
            sqlite3_str_appendf(str, "Formatted: %d bytes", (int)str_len);
        } else {
            sqlite3_str_appendf(str, "No data");
        }
    }

    // Test sqlite3_str_vappendf indirectly through wrapper
    if (op_selector & 0x08 && str_len > 0) {
        // Use the wrapper function which properly handles variadic arguments
        fuzz_appendf_wrapper(str, "Direct: %.*s", (int)(str_len > 20 ? 20 : str_len), input_str);
    }

    // Test sqlite3_str_value multiple times
    if (op_selector & 0x10) {
        for (int i = 0; i < 3; i++) {
            char *current_value = sqlite3_str_value(str);
            (void)current_value; // Use value to avoid unused variable warning
        }
    }

    // Test sqlite3_str_finish
    if (op_selector & 0x20) {
        char *result = sqlite3_str_finish(str);
        if (result) {
            sqlite3_free(result);
        }
        // str is now destroyed, so we need to create a new one for cleanup
        str = NULL;
    }

    // Cleanup if str still exists
    if (str) {
        char *result = sqlite3_str_finish(str);
        if (result) {
            sqlite3_free(result);
        }
    }

    return 0;
}
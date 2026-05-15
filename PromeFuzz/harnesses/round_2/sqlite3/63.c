// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:19130:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

static void dummy_vappendf(sqlite3_str *pStr, const char *zFormat, ...) {
    if (!pStr || !zFormat) return;
    
    va_list args;
    va_start(args, zFormat);
    sqlite3_str_vappendf(pStr, zFormat, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // Create a dynamic string builder
    sqlite3_str *pStr = sqlite3_str_new(NULL);
    if (!pStr) {
        return 0;
    }

    // Use the fuzzer input to determine which operations to perform
    uint8_t op_selector = 0;
    if (Size > 0) {
        op_selector = Data[0];
        Data++;
        Size--;
    }

    // Create a null-terminated string from the remaining input
    char *input_str = NULL;
    if (Size > 0) {
        input_str = (char *)malloc(Size + 1);
        if (input_str) {
            // Limit copy to reasonable size to avoid overflows
            size_t copy_size = Size;
            if (copy_size > 1024) copy_size = 1024;
            memcpy(input_str, Data, copy_size);
            input_str[copy_size] = '\0';
        }
    } else {
        // If no input, use a default string
        input_str = strdup("fuzz");
    }

    // Perform different operations based on the selector
    switch (op_selector % 6) {
        case 0:
            // Test sqlite3_str_value
            if (pStr) {
                sqlite3_str_value(pStr);
            }
            break;
        case 1:
            // Test sqlite3_str_finish and create a new string builder
            if (pStr) {
                char *result = sqlite3_str_finish(pStr);
                if (result) {
                    sqlite3_free(result);
                }
                pStr = sqlite3_str_new(NULL);
            }
            break;
        case 2:
            // Test sqlite3_str_vappendf through our helper
            if (pStr && input_str && Size > 0) {
                // Use simpler format specifiers that SQLite's printf supports
                dummy_vappendf(pStr, "Format: %s, int: %d", 
                              input_str ? input_str : "(null)", (int)(Size % 1000));
            }
            break;
        case 3:
            // Test sqlite3_str_appendall
            if (pStr && input_str) {
                sqlite3_str_appendall(pStr, input_str);
            }
            break;
        case 4:
            // Test sqlite3_str_appendf
            if (pStr && input_str) {
                // Use format specifiers that SQLite's printf definitely supports
                sqlite3_str_appendf(pStr, "Appendf: %s, size: %d", 
                                   input_str ? input_str : "(null)", (int)(Size % 1000));
            }
            break;
        case 5:
            // Test sqlite3_str_append
            if (pStr && input_str && Size > 0) {
                int append_len = (int)(Size % 256); // Limit to 255 bytes
                if (append_len > 0) {
                    sqlite3_str_append(pStr, input_str, append_len);
                }
            }
            break;
    }

    // Always call value and finish to explore more states
    if (pStr) {
        sqlite3_str_value(pStr);
        
        char *final_result = sqlite3_str_finish(pStr);
        if (final_result) {
            sqlite3_free(final_result);
        }
    }

    // Cleanup
    if (input_str) {
        free(input_str);
    }

    return 0;
}
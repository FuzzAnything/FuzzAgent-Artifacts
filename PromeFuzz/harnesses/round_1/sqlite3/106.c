// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_sqlite3_str_operations(const uint8_t *data, size_t size) {
    // Create a dynamic string object
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) {
        return;
    }

    // Use the first byte to determine operation sequence
    uint8_t op_selector = size > 0 ? data[0] : 0;
    
    // Create a null-terminated format string from the input data
    // Limit to reasonable size to avoid excessive memory usage
    size_t format_len = size > 1 ? (data[1] % 256) : 0;
    if (format_len > size - 2) {
        format_len = size > 2 ? size - 2 : 0;
    }
    
    char *format = NULL;
    if (format_len > 0 && size > 2) {
        format = (char*)malloc(format_len + 1);
        if (format) {
            memcpy(format, data + 2, format_len);
            format[format_len] = '\0';
            
            // Create argument string from remaining data
            size_t arg_len = size - 2 - format_len;
            char *arg = NULL;
            if (arg_len > 0) {
                arg = (char*)malloc(arg_len + 1);
                if (arg) {
                    memcpy(arg, data + 2 + format_len, arg_len);
                    arg[arg_len] = '\0';
                    
                    // Perform various operations based on selector
                    switch (op_selector % 8) {
                        case 0:
                            // Simple append with format
                            sqlite3_str_appendf(str, format, arg);
                            break;
                        case 1:
                            // Append with multiple format specifiers
                            sqlite3_str_appendf(str, "%s: %d bytes", arg, (int)arg_len);
                            break;
                        case 2:
                            // Append with integer formatting
                            sqlite3_str_appendf(str, "Data: %.*s", (int)(arg_len % 100), arg);
                            break;
                        case 3:
                            // Multiple appends
                            sqlite3_str_appendf(str, "Prefix: %s", arg);
                            sqlite3_str_appendf(str, " Suffix");
                            break;
                        case 4:
                            // Append then reset
                            sqlite3_str_appendf(str, format, arg);
                            sqlite3_str_reset(str);
                            sqlite3_str_appendf(str, "Reset: %s", arg);
                            break;
                        case 5:
                            // Append empty string
                            sqlite3_str_appendf(str, "");
                            sqlite3_str_appendf(str, format, arg);
                            break;
                        case 6:
                            // Test value retrieval
                            sqlite3_str_appendf(str, format, arg);
                            (void)sqlite3_str_value(str);
                            break;
                        case 7:
                            // Complex format string
                            sqlite3_str_appendf(str, "%s %d %f", arg, (int)arg_len, (double)arg_len);
                            break;
                    }
                    
                    free(arg);
                }
            } else {
                // No argument data, just format
                sqlite3_str_appendf(str, format);
            }
            
            free(format);
        }
    } else {
        // Minimal test with fixed format
        sqlite3_str_appendf(str, "Test %d", (int)size);
    }

    // Always test value retrieval
    (void)sqlite3_str_value(str);

    // Sometimes finish, sometimes free
    if (op_selector & 0x80) {
        char *result = sqlite3_str_finish(str);
        if (result) {
            sqlite3_free(result);
        }
    } else {
        sqlite3_str_free(str);
    }
}

static void fuzz_sqlite3_str_edge_cases(const uint8_t *data, size_t size) {
    // Test edge cases
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) {
        return;
    }

    // Test with empty input
    if (size == 0) {
        sqlite3_str_appendf(str, "");
        sqlite3_str_reset(str);
    }

    // Test with very small input
    if (size == 1) {
        sqlite3_str_appendf(str, "%c", data[0]);
    }

    // Test reset on empty string
    sqlite3_str_reset(str);

    // Test multiple resets
    sqlite3_str_reset(str);
    sqlite3_str_reset(str);

    // Test value on empty string
    (void)sqlite3_str_value(str);

    // Test finish on empty string
    char *result = sqlite3_str_finish(str);
    if (result) {
        sqlite3_free(result);
    }
}

static void fuzz_sqlite3_str_sequence(const uint8_t *data, size_t size) {
    // Test sequence of operations
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) {
        return;
    }

    // Use data to determine operation sequence
    for (size_t i = 0; i < size && i < 100; i++) {
        uint8_t op = data[i];
        switch (op % 6) {
            case 0:
                sqlite3_str_appendf(str, "A%d", (int)i);
                break;
            case 1:
                sqlite3_str_appendf(str, "B");
                break;
            case 2:
                sqlite3_str_reset(str);
                break;
            case 3:
                (void)sqlite3_str_value(str);
                break;
            case 4:
                sqlite3_str_appendf(str, "%c", op);
                break;
            case 5:
                // Do nothing
                break;
        }
    }

    sqlite3_str_free(str);
}

int LLVMFuzzerTestOneInput_106(const uint8_t *data, size_t size) {
    // Use different parts of the input for different test scenarios
    if (size > 0) {
        uint8_t test_selector = data[0];
        
        // Distribute tests based on selector
        if (test_selector < 85) {
            fuzz_sqlite3_str_operations(data, size);
        } else if (test_selector < 170) {
            fuzz_sqlite3_str_edge_cases(data, size);
        } else {
            fuzz_sqlite3_str_sequence(data, size);
        }
    } else {
        // Test with zero-sized input
        fuzz_sqlite3_str_edge_cases(data, size);
    }
    
    return 0;
}
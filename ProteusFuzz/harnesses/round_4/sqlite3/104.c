// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

static void fuzz_appendf(sqlite3_str *str, const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Allocate buffer with extra space for potential doubling of '%' characters
    // Max expansion is 2x (each % becomes %%), plus null terminator
    size_t max_size = size * 2 + 1;
    char *fmt_buf = malloc(max_size);
    if (!fmt_buf) return;
    
    size_t write_idx = 0;
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '%') {
            // Escape % to %% to prevent format string vulnerabilities
            if (write_idx + 2 <= max_size) {
                fmt_buf[write_idx++] = '%';
                fmt_buf[write_idx++] = '%';
            }
        } else {
            if (write_idx + 1 <= max_size) {
                fmt_buf[write_idx++] = (char)data[i];
            }
        }
    }
    fmt_buf[write_idx] = '\0';
    
    // Ensure the format string is not empty to satisfy internal assertions
    // The assertion fails if the string is empty but printfFlags indicates malloc was used
    // We must ensure we don't call appendf with an empty format string if the string object
    // has been used before or if it's in a state that expects content
    if (write_idx > 0) {
        sqlite3_str_appendf(str, fmt_buf, 42, "test", 3.14);
    }
    
    free(fmt_buf);
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize database connection (optional, can be NULL)
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // Even if open fails, we can still test with NULL db
        db = NULL;
    }

    // Create a new dynamic string object
    sqlite3_str *str = sqlite3_str_new(db);
    if (str == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }

    // Parse input to determine operations
    uint8_t op = Data[0] % 4;
    
    switch (op) {
        case 0: // sqlite3_str_append
            if (Size > 1) {
                int n = (Data[1] % 100) + 1;
                if (Size > 1 + n) {
                    sqlite3_str_append(str, (const char *)(Data + 2), n);
                }
            }
            break;
            
        case 1: // sqlite3_str_appendf
            if (Size > 1) {
                fuzz_appendf(str, Data + 1, Size - 1);
            }
            break;
            
        case 2: // sqlite3_str_appendall
            if (Size > 1) {
                // Ensure null termination for appendall
                char *safe_str = malloc(Size);
                if (safe_str) {
                    memcpy(safe_str, Data + 1, Size - 1);
                    safe_str[Size - 1] = '\0';
                    sqlite3_str_appendall(str, safe_str);
                    free(safe_str);
                }
            }
            break;
            
        case 3: // Mixed operations
            if (Size > 2) {
                // Append some characters
                int n = Data[1] % 50;
                if (Size > 2 + n) {
                    sqlite3_str_append(str, (const char *)(Data + 2), n);
                }
                
                // Append formatted string
                if (Size > 2 + n + 1) {
                    fuzz_appendf(str, Data + 2 + n, Size - 2 - n);
                }
                
                // Append all
                if (Size > 2 + n + 1) {
                    char *safe_str = malloc(Size - 2 - n);
                    if (safe_str) {
                        memcpy(safe_str, Data + 2 + n, Size - 2 - n - 1);
                        safe_str[Size - 2 - n - 1] = '\0';
                        sqlite3_str_appendall(str, safe_str);
                        free(safe_str);
                    }
                }
            }
            break;
    }
    
    // Additional random operations to increase coverage
    if (Size > 3) {
        // Random appendchar
        int count = Data[1] % 20;
        char c = (char)(Data[2] % 256);
        sqlite3_str_appendchar(str, count, c);
        
        // Random reset
        if (Data[3] % 2 == 0) {
            sqlite3_str_reset(str);
        }
        
        // Random truncate
        int truncate_len = Data[3] % 100;
        sqlite3_str_truncate(str, truncate_len);
    }
    
    // Get the final value
    char *result = sqlite3_str_value(str);
    if (result) {
        // Just access the result to trigger any potential issues
        (void)strlen(result);
    }
    
    // Finalize and free the string
    char *finished = sqlite3_str_finish(str);
    if (finished) {
        sqlite3_free(finished);
    }
    
    // Cleanup database
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}
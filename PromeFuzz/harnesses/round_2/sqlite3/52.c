// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
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
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Create a dummy database connection for testing
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Use fuzzer input to control behavior
    if (Size < 1) {
        sqlite3_close(db);
        return 0;
    }
    
    // Extract control byte from input
    uint8_t control = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Create dynamic string objects
    sqlite3_str *str1 = sqlite3_str_new(db);
    sqlite3_str *str2 = sqlite3_str_new(NULL);
    
    // Test different scenarios based on control byte
    switch (control % 8) {
        case 0:
            // Basic append and finish
            if (fuzz_size > 0) {
                char *format = "%.*s";
                sqlite3_str_appendf(str1, format, (int)fuzz_size, fuzz_data);
            }
            break;
            
        case 1:
            // Multiple appends
            if (fuzz_size > 0) {
                char *format1 = "Prefix: %.*s";
                char *format2 = " Suffix";
                sqlite3_str_appendf(str1, format1, (int)fuzz_size, fuzz_data);
                sqlite3_str_appendf(str1, format2);
            }
            break;
            
        case 2:
            // Append, reset, then append again
            if (fuzz_size > 0) {
                char *format = "First: %.*s";
                sqlite3_str_appendf(str1, format, (int)fuzz_size, fuzz_data);
                sqlite3_str_reset(str1);
                sqlite3_str_appendf(str1, "Reset: %.*s", (int)fuzz_size, fuzz_data);
            }
            break;
            
        case 3:
            // Test with both string objects
            if (fuzz_size > 0) {
                char *format = "Str1: %.*s";
                sqlite3_str_appendf(str1, format, (int)fuzz_size, fuzz_data);
                sqlite3_str_appendf(str2, "Str2: %.*s", (int)fuzz_size, fuzz_data);
            }
            break;
            
        case 4:
            // Test sqlite3_str_value
            if (fuzz_size > 0) {
                char *format = "%.*s";
                sqlite3_str_appendf(str1, format, (int)fuzz_size, fuzz_data);
                char *value = sqlite3_str_value(str1);
                if (value) {
                    // Access the value to exercise the code
                    volatile char c = value[0];
                    (void)c;
                }
            }
            break;
            
        case 5:
            // Test empty string operations
            sqlite3_str_reset(str1);
            sqlite3_str_appendf(str1, "");
            break;
            
        case 6:
            // Test format with different specifiers
            if (fuzz_size > 0) {
                sqlite3_str_appendf(str1, "String: %.*s, Int: %d, Hex: %x", 
                                   (int)fuzz_size, fuzz_data, 
                                   (int)fuzz_size, (int)fuzz_size);
            }
            break;
            
        case 7:
            // Test with maximum fuzz data
            if (fuzz_size > 0) {
                // Use a simple format to avoid format string vulnerabilities
                sqlite3_str_appendf(str1, "%.*s", (int)fuzz_size, fuzz_data);
                sqlite3_str_appendf(str2, "%.*s", (int)fuzz_size, fuzz_data);
            }
            break;
    }
    
    // Test sqlite3_str_finish
    char *finished1 = sqlite3_str_finish(str1);
    if (finished1) {
        sqlite3_free(finished1);
    }
    
    // Test sqlite3_str_free on second string
    sqlite3_str_free(str2);
    
    // Test edge cases
    sqlite3_str *null_str = NULL;
    sqlite3_str_free(null_str);
    sqlite3_str_reset(null_str);
    char *null_value = sqlite3_str_value(null_str);
    (void)null_value;
    
    // Create another string for additional testing
    sqlite3_str *str3 = sqlite3_str_new(db);
    if (str3) {
        sqlite3_str_appendf(str3, "Final test");
        char *finished3 = sqlite3_str_finish(str3);
        if (finished3) {
            sqlite3_free(finished3);
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:19230:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_append_operations(sqlite3_str *pStr, const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Use first byte to decide operation mix
    uint8_t op = Data[0];
    
    // Append some formatted text using a safe format string
    if (op & 0x01) {
        sqlite3_str_appendf(pStr, "Test string with size: %zu", Size);
    }
    
    // Append raw data as hex representation
    if (op & 0x02 && Size > 1) {
        size_t copy_len = Size > 100 ? 100 : Size;
        char *temp_buf = (char*)malloc(copy_len + 1);
        if (temp_buf) {
            memcpy(temp_buf, Data, copy_len);
            temp_buf[copy_len] = '\0';
            sqlite3_str_appendf(pStr, " Data: %s", temp_buf);
            free(temp_buf);
        }
    }
    
    // Append integer values
    if (op & 0x04 && Size > 4) {
        uint32_t val = 0;
        memcpy(&val, Data, Size > 4 ? 4 : Size);
        sqlite3_str_appendf(pStr, " Value: %u", val);
    }
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    // Test 1: Create string, append, get value, finish
    sqlite3_str *pStr1 = sqlite3_str_new(NULL);
    if (pStr1) {
        dummy_append_operations(pStr1, Data, Size);
        
        // Get current value
        char *current_val = sqlite3_str_value(pStr1);
        (void)current_val; // Use it to avoid unused warning
        
        // Finish and free
        char *result1 = sqlite3_str_finish(pStr1);
        if (result1) {
            sqlite3_free(result1);
        }
    }
    
    // Test 2: Create string, append, reset, append again, free
    sqlite3_str *pStr2 = sqlite3_str_new(NULL);
    if (pStr2) {
        dummy_append_operations(pStr2, Data, Size);
        
        // Reset and append different content
        sqlite3_str_reset(pStr2);
        if (Size > 0) {
            sqlite3_str_appendf(pStr2, "Reset and new data: first byte %u", Data[0]);
        }
        
        // Free directly
        sqlite3_str_free(pStr2);
    }
    
    // Test 3: Create string, append, get value multiple times, finish
    sqlite3_str *pStr3 = sqlite3_str_new(NULL);
    if (pStr3) {
        // Multiple append operations
        for (size_t i = 0; i < (Size > 10 ? 10 : Size); i++) {
            if (i < Size) {
                sqlite3_str_appendf(pStr3, "[%u]", Data[i]);
            }
        }
        
        // Get value multiple times
        for (int i = 0; i < 3; i++) {
            char *val = sqlite3_str_value(pStr3);
            (void)val;
        }
        
        char *result3 = sqlite3_str_finish(pStr3);
        if (result3) {
            sqlite3_free(result3);
        }
    }
    
    // Test 4: Edge case - empty string operations
    sqlite3_str *pStr4 = sqlite3_str_new(NULL);
    if (pStr4) {
        // Don't append anything
        char *empty_val = sqlite3_str_value(pStr4);
        (void)empty_val;
        
        char *result4 = sqlite3_str_finish(pStr4);
        if (result4) {
            sqlite3_free(result4);
        }
    }
    
    // Test 5: Create, immediately free
    sqlite3_str *pStr5 = sqlite3_str_new(NULL);
    sqlite3_str_free(pStr5);
    
    // Test 6: Create, reset multiple times, free
    sqlite3_str *pStr6 = sqlite3_str_new(NULL);
    if (pStr6) {
        for (int i = 0; i < 5; i++) {
            sqlite3_str_reset(pStr6);
            if (i < Size) {
                sqlite3_str_appendf(pStr6, "Iteration %d", i);
            }
        }
        sqlite3_str_free(pStr6);
    }
    
    return 0;
}
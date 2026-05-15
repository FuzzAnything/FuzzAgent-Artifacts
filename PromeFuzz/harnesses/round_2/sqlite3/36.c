// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>

static char dummy_buffer[1024];

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Use the first 8 bytes for heap limit values
    sqlite3_int64 limit1 = 0;
    sqlite3_int64 limit2 = 0;
    
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&limit1, Data, sizeof(sqlite3_int64));
        if (Size >= 2 * sizeof(sqlite3_int64)) {
            memcpy(&limit2, Data + sizeof(sqlite3_int64), sizeof(sqlite3_int64));
        }
    }
    
    // Call functions in the specified order
    sqlite3_shutdown();
    
    // First sqlite3_config call - using SQLITE_CONFIG_SINGLETHREAD (1)
    // which doesn't require additional arguments
    sqlite3_config(1);
    
    // First sqlite3_hard_heap_limit64 call
    sqlite3_hard_heap_limit64(limit1);
    
    // Second sqlite3_hard_heap_limit64 call
    sqlite3_hard_heap_limit64(limit2);
    
    // Second sqlite3_config call - using SQLITE_CONFIG_MULTITHREAD (2)
    sqlite3_config(2);
    
    // sqlite3_snprintf call
    // Use remaining data for format string if available
    const char* format = "Test: %d";
    if (Size > 2 * sizeof(sqlite3_int64)) {
        // Use up to 100 bytes for format string
        size_t format_len = Size - 2 * sizeof(sqlite3_int64);
        if (format_len > 100) format_len = 100;
        
        // Create a temporary buffer for format string
        char temp_format[101];
        memcpy(temp_format, Data + 2 * sizeof(sqlite3_int64), format_len);
        temp_format[format_len] = '\0';
        
        // Ensure the format string is valid (replace non-printable characters)
        for (size_t i = 0; i < format_len; i++) {
            if (temp_format[i] < 32 || temp_format[i] > 126) {
                temp_format[i] = ' ';
            }
        }
        format = temp_format;
    }
    
    sqlite3_snprintf(sizeof(dummy_buffer), dummy_buffer, format, 123);
    
    return 0;
}
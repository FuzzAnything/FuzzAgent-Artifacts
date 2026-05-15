// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Call functions in specified order
    
    // 1. sqlite3_shutdown
    sqlite3_shutdown();
    
    // 2. sqlite3_config - using first 4 bytes for configuration option
    int config_opt = 0;
    if (Size >= sizeof(int)) {
        memcpy(&config_opt, Data, sizeof(int));
    }
    sqlite3_config(config_opt);
    
    // 3. sqlite3_hard_heap_limit64 - using next 8 bytes for limit
    sqlite3_int64 limit1 = 0;
    if (Size >= sizeof(int) + sizeof(sqlite3_int64)) {
        memcpy(&limit1, Data + sizeof(int), sizeof(sqlite3_int64));
    }
    sqlite3_hard_heap_limit64(limit1);
    
    // 4. sqlite3_hard_heap_limit64 - using next 8 bytes for second limit
    sqlite3_int64 limit2 = 0;
    if (Size >= sizeof(int) + 2 * sizeof(sqlite3_int64)) {
        memcpy(&limit2, Data + sizeof(int) + sizeof(sqlite3_int64), sizeof(sqlite3_int64));
    }
    sqlite3_hard_heap_limit64(limit2);
    
    // 5. sqlite3_config - using next 4 bytes for second configuration option
    int config_opt2 = 0;
    if (Size >= sizeof(int) + 2 * sizeof(sqlite3_int64) + sizeof(int)) {
        memcpy(&config_opt2, Data + sizeof(int) + 2 * sizeof(sqlite3_int64), sizeof(int));
    }
    sqlite3_config(config_opt2);
    
    // 6. sqlite3_snprintf - using remaining data for format string
    char buffer[256];
    if (Size > 0) {
        // Create format string from input data
        size_t format_len = Size < 255 ? Size : 255;
        char format_str[256];
        memcpy(format_str, Data, format_len);
        format_str[format_len] = '\0';
        
        // Use a simple integer argument for formatting
        int value = 42;
        sqlite3_snprintf(sizeof(buffer), buffer, format_str, value);
    } else {
        // Default format if no data
        sqlite3_snprintf(sizeof(buffer), buffer, "Test: %d", 123);
    }
    
    return 0;
}
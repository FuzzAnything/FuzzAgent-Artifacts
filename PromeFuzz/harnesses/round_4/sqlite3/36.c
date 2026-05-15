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
#include "code/sqlite3.h"

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Initialize variables for the API calls
    int config_option1, config_option2;
    sqlite3_int64 limit1, limit2, prev_limit;
    char buffer[256];
    const char *format_str = "Test format: %d";
    int test_value = 123;
    
    // Use fuzz data to generate values for the API calls
    if (Size >= sizeof(int) + 2 * sizeof(sqlite3_int64)) {
        // Use first int for config_option1
        config_option1 = *(const int*)Data;
        Data += sizeof(int);
        Size -= sizeof(int);
        
        // Use first sqlite3_int64 for limit1
        limit1 = *(const sqlite3_int64*)Data;
        Data += sizeof(sqlite3_int64);
        Size -= sizeof(sqlite3_int64);
        
        // Use second sqlite3_int64 for limit2
        limit2 = *(const sqlite3_int64*)Data;
        Data += sizeof(sqlite3_int64);
        Size -= sizeof(sqlite3_int64);
        
        // Use remaining data for config_option2 if available
        if (Size >= sizeof(int)) {
            config_option2 = *(const int*)Data;
        } else {
            config_option2 = 0;
        }
    } else {
        // Default values if not enough fuzz data
        config_option1 = 0;
        config_option2 = 0;
        limit1 = 1000000;
        limit2 = 500000;
    }
    
    // Call functions in the specified order
    
    // 1. sqlite3_shutdown
    sqlite3_shutdown();
    
    // 2. sqlite3_config
    sqlite3_config(config_option1);
    
    // 3. sqlite3_hard_heap_limit64
    prev_limit = sqlite3_hard_heap_limit64(limit1);
    
    // 4. sqlite3_hard_heap_limit64 (second call)
    prev_limit = sqlite3_hard_heap_limit64(limit2);
    
    // 5. sqlite3_config (second call)
    sqlite3_config(config_option2);
    
    // 6. sqlite3_snprintf
    sqlite3_snprintf(sizeof(buffer), buffer, format_str, test_value);
    
    return 0;
}
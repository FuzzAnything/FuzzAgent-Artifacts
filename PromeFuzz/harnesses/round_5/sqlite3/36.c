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

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // First call: sqlite3_shutdown
    sqlite3_shutdown();
    
    // Prepare arguments for sqlite3_config
    int config_option1;
    int config_value1;
    
    // Use fuzzer input to determine configuration values
    if (Size >= 2) {
        config_option1 = Data[0] % 20;  // There are about 20 config options
        config_value1 = Data[1];
    } else {
        config_option1 = 1;  // SQLITE_CONFIG_SINGLETHREAD
        config_value1 = 0;
    }
    
    // Second call: sqlite3_config
    sqlite3_config(config_option1, config_value1);
    
    // Prepare arguments for sqlite3_hard_heap_limit64
    sqlite3_int64 limit1, limit2;
    
    if (Size >= 10) {
        // Use bytes 2-9 for first limit (8 bytes for int64)
        memcpy(&limit1, Data + 2, sizeof(sqlite3_int64));
    } else {
        limit1 = 1024 * 1024;  // Default 1MB limit
    }
    
    // Third call: sqlite3_hard_heap_limit64
    sqlite3_hard_heap_limit64(limit1);
    
    // Prepare second limit
    if (Size >= 18) {
        // Use bytes 10-17 for second limit
        memcpy(&limit2, Data + 10, sizeof(sqlite3_int64));
    } else {
        limit2 = 512 * 1024;  // Default 512KB limit
    }
    
    // Fourth call: sqlite3_hard_heap_limit64
    sqlite3_hard_heap_limit64(limit2);
    
    // Prepare arguments for second sqlite3_config call
    int config_option2;
    int config_value2;
    
    if (Size >= 20) {
        config_option2 = Data[18] % 20;
        config_value2 = Data[19];
    } else {
        config_option2 = 2;  // SQLITE_CONFIG_MULTITHREAD
        config_value2 = 0;
    }
    
    // Fifth call: sqlite3_config
    sqlite3_config(config_option2, config_value2);
    
    // Prepare buffer and arguments for sqlite3_snprintf
    char buffer[256];
    const char *format;
    int format_len;
    
    if (Size >= 21) {
        // Use remaining bytes for format string
        format_len = (Size - 20 > 100) ? 100 : (int)(Size - 20);
        char temp_format[101];
        memcpy(temp_format, Data + 20, format_len);
        temp_format[format_len] = '\0';
        format = temp_format;
    } else {
        format = "Test %d: %lld";
    }
    
    // Prepare integer arguments for snprintf
    int int_arg = (Size > 0) ? Data[0] : 42;
    sqlite3_int64 int64_arg = (Size >= 8) ? *((sqlite3_int64*)Data) : 1234567890;
    
    // Sixth call: sqlite3_snprintf
    sqlite3_snprintf(sizeof(buffer), buffer, format, int_arg, int64_arg);
    
    return 0;
}
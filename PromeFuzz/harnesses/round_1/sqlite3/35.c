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
#include <stdint.h>
#include <string.h>

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // First call: sqlite3_shutdown
    sqlite3_shutdown();
    
    // Prepare for sqlite3_config calls
    // We'll use SQLITE_CONFIG_MEMSTATUS which is an anytime option to avoid misuse
    int config_option = SQLITE_CONFIG_MEMSTATUS;
    int memstatus_value = 0;
    
    // Use first byte to determine memstatus value if available
    if (Size > 0) {
        memstatus_value = Data[0] & 1;
    }
    
    // Second call: sqlite3_config with SQLITE_CONFIG_MEMSTATUS
    sqlite3_config(config_option, memstatus_value);
    
    // Prepare for sqlite3_hard_heap_limit64 calls
    sqlite3_int64 limit1 = -1;  // Negative leaves limit unchanged
    sqlite3_int64 limit2 = -1;
    
    // Use up to 16 bytes for the two 64-bit limits if available
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&limit1, Data, sizeof(sqlite3_int64));
    }
    
    // Third call: sqlite3_hard_heap_limit64
    sqlite3_hard_heap_limit64(limit1);
    
    // Prepare second limit
    size_t offset = sizeof(sqlite3_int64);
    if (Size >= offset + sizeof(sqlite3_int64)) {
        memcpy(&limit2, Data + offset, sizeof(sqlite3_int64));
    }
    
    // Fourth call: sqlite3_hard_heap_limit64
    sqlite3_hard_heap_limit64(limit2);
    
    // Fifth call: sqlite3_config again with different memstatus value
    int memstatus_value2 = 1;
    if (Size > 1) {
        memstatus_value2 = Data[1] & 1;
    }
    sqlite3_config(config_option, memstatus_value2);
    
    // Sixth call: sqlite3_snprintf
    // Prepare buffer and format string
    char buffer[256];
    const char* format = "Test string with data";
    
    // Use remaining data for format string if available
    size_t format_offset = offset + sizeof(sqlite3_int64);
    if (format_offset < Size) {
        // Copy up to 128 bytes for format string
        size_t format_len = Size - format_offset;
        if (format_len > 127) format_len = 127;
        
        char dynamic_format[128];
        memcpy(dynamic_format, Data + format_offset, format_len);
        dynamic_format[format_len] = '\0';
        
        // Try to use the dynamic format string
        sqlite3_snprintf(sizeof(buffer), buffer, "%s", dynamic_format);
    } else {
        // Use default format
        sqlite3_snprintf(sizeof(buffer), buffer, "%s: %zu bytes", format, Size);
    }
    
    return 0;
}
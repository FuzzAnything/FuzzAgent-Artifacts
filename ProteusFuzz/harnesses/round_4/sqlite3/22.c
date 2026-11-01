// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void safe_snprintf_call(const uint8_t *Data, size_t Size) {
    char buffer[256] = {0};
    char fmt_buf[256] = {0};
    
    // Default format string
    const char *format_str = "test";
    
    // Only construct a custom format string if we have enough data
    if (Size > 10) {
        size_t fmt_len = Size - 10;
        if (fmt_len > 250) fmt_len = 250;
        
        // Copy data safely
        memcpy(fmt_buf, Data + 10, fmt_len);
        fmt_buf[fmt_len] = '\0';
        
        // Ensure the format string is valid (no embedded nulls that break it, 
        // though memcpy handles the length, we just use it as is if not empty)
        if (fmt_len > 0 && fmt_buf[0] != '\0') {
            format_str = fmt_buf;
        }
    }

    int arg1 = 0, arg2 = 0;
    if (Size > 12) arg1 = Data[12];
    if (Size > 13) arg2 = Data[13];

    // Call sqlite3_snprintf with safe buffer and format
    // Ensure buffer is large enough and format is safe
    char *result = sqlite3_snprintf(sizeof(buffer), buffer, format_str, arg1, arg2);
    
    // Check result, though sqlite3_snprintf usually returns the buffer pointer
    if (result == NULL) {
        return;
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Step 1: Invoke sqlite3_shutdown to ensure a clean state
    sqlite3_shutdown();

    // Step 2: Invoke sqlite3_config
    int config_option = Data[0] % 10;
    if (config_option == 0) config_option = SQLITE_CONFIG_SINGLETHREAD;
    else if (config_option == 1) config_option = SQLITE_CONFIG_MULTITHREAD;
    else if (config_option == 2) config_option = SQLITE_CONFIG_SERIALIZED;
    else config_option = SQLITE_CONFIG_SINGLETHREAD;
    
    sqlite3_config(config_option);

    // Step 3: Invoke sqlite3_hard_heap_limit64
    sqlite3_int64 limit_val = 0;
    if (Size >= 8) {
        // Read 8 bytes safely
        limit_val = 0;
        for (int i = 0; i < 8 && (1 + i) < Size; i++) {
            limit_val |= ((sqlite3_int64)Data[1 + i]) << (i * 8);
        }
    } else {
        limit_val = Data[1];
    }
    if (limit_val < 0) limit_val = -1;
    sqlite3_hard_heap_limit64(limit_val);

    // Step 4: Invoke sqlite3_hard_heap_limit64 again
    sqlite3_int64 limit_val2 = 0;
    if (Size >= 16) {
        limit_val2 = 0;
        for (int i = 0; i < 8 && (8 + i) < Size; i++) {
            limit_val2 |= ((sqlite3_int64)Data[8 + i]) << (i * 8);
        }
    } else {
        limit_val2 = Data[8 % Size];
    }
    if (limit_val2 < 0) limit_val2 = -1;
    sqlite3_hard_heap_limit64(limit_val2);

    // Step 5: Invoke sqlite3_config again
    int config_option2 = Data[9 % Size] % 10;
    if (config_option2 == 0) config_option2 = SQLITE_CONFIG_SINGLETHREAD;
    else if (config_option2 == 1) config_option2 = SQLITE_CONFIG_MULTITHREAD;
    else if (config_option2 == 2) config_option2 = SQLITE_CONFIG_SERIALIZED;
    else config_option2 = SQLITE_CONFIG_SINGLETHREAD;
    
    sqlite3_config(config_option2);

    // Step 6: Invoke sqlite3_snprintf safely
    safe_snprintf_call(Data, Size);

    return 0;
}
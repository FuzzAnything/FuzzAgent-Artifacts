// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:33787:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Use the fuzz input to determine which functions to call and in what order
    // We'll use the first few bytes to decide on configuration and sequence
    
    if (Size < 1) {
        return 0;
    }
    
    // Use first byte to determine the sequence pattern
    uint8_t pattern = Data[0];
    
    // Initialize OS interface
    sqlite3_os_init();
    
    // Initialize SQLite library
    sqlite3_initialize();
    
    // Use second byte (if available) to determine shared cache setting
    if (Size > 1) {
        int enable_cache = Data[1] % 2;
        sqlite3_enable_shared_cache(enable_cache);
    }
    
    // Use third byte (if available) for configuration options
    if (Size > 2) {
        int config_option = Data[2] % 20; // There are many config options, limit to 20
        int config_value = (Size > 3) ? Data[3] : 0;
        
        // Try different configuration options based on the byte value
        switch (config_option) {
            case 0:
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
            case 1:
                sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                break;
            case 2:
                sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                break;
            case 3:
                sqlite3_config(SQLITE_CONFIG_MEMSTATUS, config_value);
                break;
            case 4:
                sqlite3_config(SQLITE_CONFIG_SCRATCH, NULL, 0, 0);
                break;
            case 5:
                sqlite3_config(SQLITE_CONFIG_PAGECACHE, NULL, 0, 0);
                break;
            case 6:
                sqlite3_config(SQLITE_CONFIG_HEAP, NULL, 0, 0);
                break;
            case 7:
                sqlite3_config(SQLITE_CONFIG_MUTEX, NULL);
                break;
            case 8:
                sqlite3_config(SQLITE_CONFIG_GETMUTEX, NULL);
                break;
            case 9:
                sqlite3_config(SQLITE_CONFIG_LOOKASIDE, 100, 128);
                break;
            default:
                // Try with the raw values
                sqlite3_config(config_option, config_value);
                break;
        }
    }
    
    // Shutdown sequence based on pattern
    if (pattern % 2 == 0) {
        // Normal shutdown sequence
        sqlite3_shutdown();
        sqlite3_os_end();
    } else if (pattern % 3 == 0) {
        // Only shutdown
        sqlite3_shutdown();
    } else if (pattern % 5 == 0) {
        // Only os_end
        sqlite3_os_end();
    } else {
        // Reinitialize after shutdown
        sqlite3_shutdown();
        sqlite3_initialize();
        sqlite3_shutdown();
        sqlite3_os_end();
    }
    
    // Try calling initialize again (should be safe)
    sqlite3_initialize();
    
    // Try enabling shared cache with different values
    for (int i = 0; i < 2 && Size > i + 4; i++) {
        int cache_val = Data[4 + i] % 2;
        sqlite3_enable_shared_cache(cache_val);
    }
    
    // Final cleanup
    sqlite3_shutdown();
    sqlite3_os_end();
    
    return 0;
}
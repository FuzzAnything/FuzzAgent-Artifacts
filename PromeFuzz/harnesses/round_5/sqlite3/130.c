// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    // Use the first byte to determine which functions to call and with what parameters
    if (Size == 0) {
        return 0;
    }

    uint8_t selector = Data[0];

    // Call sqlite3_threadsafe() - always safe to call
    int thread_safe = sqlite3_threadsafe();

    // Call sqlite3_initialize() - idempotent, safe to call multiple times
    int init_result = sqlite3_initialize();

    // Call sqlite3_enable_shared_cache() based on input
    // Use the second byte if available, otherwise use selector
    int enable_cache = 0;
    if (Size > 1) {
        enable_cache = Data[1] & 1; // Use only the lowest bit
    } else {
        enable_cache = selector & 1;
    }
    int cache_result = sqlite3_enable_shared_cache(enable_cache);

    // Call sqlite3_config() with various options based on input
    // We'll try a few common configuration options
    if (Size > 2) {
        uint8_t config_selector = Data[2] % 4;
        
        switch (config_selector) {
            case 0:
                // SQLITE_CONFIG_SINGLETHREAD
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
            case 1:
                // SQLITE_CONFIG_MULTITHREAD
                sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                break;
            case 2:
                // SQLITE_CONFIG_SERIALIZED
                sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                break;
            case 3:
                // SQLITE_CONFIG_MEMSTATUS - enable/disable memory status reporting
                if (Size > 3) {
                    int memstatus = Data[3] & 1;
                    sqlite3_config(SQLITE_CONFIG_MEMSTATUS, memstatus);
                }
                break;
        }
    }

    // Call sqlite3_thread_cleanup() - no-op but safe to call
    sqlite3_thread_cleanup();

    // Call sqlite3_shutdown() based on selector
    // Only call shutdown if selector indicates (bit 2 set) and we successfully initialized
    if ((selector & 0x04) && init_result == SQLITE_OK) {
        int shutdown_result = sqlite3_shutdown();
        
        // After shutdown, we can try to initialize again
        if (shutdown_result == SQLITE_OK) {
            sqlite3_initialize();
        }
    }

    // Try calling sqlite3_config() again after potential shutdown/initialize cycle
    // This tests if we can call config at appropriate times
    if (Size > 4) {
        uint8_t post_config = Data[4] % 3;
        
        // Try some "anytime" configuration options
        switch (post_config) {
            case 0:
                // SQLITE_CONFIG_LOG - requires a callback, but we'll pass NULL
                // This might fail but that's OK for fuzzing
                sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
                break;
            case 1:
                // SQLITE_CONFIG_URI - enable/disable URI handling
                if (Size > 5) {
                    int uri_enable = Data[5] & 1;
                    sqlite3_config(SQLITE_CONFIG_URI, uri_enable);
                }
                break;
            case 2:
                // SQLITE_CONFIG_COVERING_INDEX_SCAN - enable/disable covering index scan
                if (Size > 6) {
                    int covering_index = Data[6] & 1;
                    sqlite3_config(SQLITE_CONFIG_COVERING_INDEX_SCAN, covering_index);
                }
                break;
        }
    }

    // Final cleanup
    sqlite3_thread_cleanup();

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    // Use the fuzz data to determine which operations to perform
    // We'll use the first few bytes to decide actions and parameters
    
    if (Size == 0) return 0;
    
    // Use first byte to determine primary action
    uint8_t action = Data[0];
    
    // Call various API functions based on fuzz input
    // 1. sqlite3_shutdown - can be called multiple times safely
    if (action & 0x01) {
        sqlite3_shutdown();
    }
    
    // 2. sqlite3_thread_cleanup - no-op, safe to call anytime
    if (action & 0x02) {
        sqlite3_thread_cleanup();
    }
    
    // 3. sqlite3_threadsafe - returns compile-time setting
    if (action & 0x04) {
        int thread_safe = sqlite3_threadsafe();
        (void)thread_safe; // Use result to avoid unused variable warning
    }
    
    // 4. sqlite3_enable_shared_cache - enable/disable shared cache
    if (action & 0x08 && Size > 1) {
        int enable = Data[1] & 0x01; // Use second byte as boolean
        sqlite3_enable_shared_cache(enable);
    }
    
    // 5. sqlite3_config - configure SQLite globally
    // We need to be careful with this function as it can cause crashes
    // if called with invalid arguments or at wrong times
    if (action & 0x10 && Size > 2) {
        // Use second and third bytes to determine config option and value
        // Limit to safe configuration options that don't require complex arguments
        int config_option = Data[1] % 10; // Reduced range for safety
        
        // Only try configuration options that are safe to call
        // with simple integer arguments or no arguments
        switch (config_option) {
            case 0:
                // SQLITE_CONFIG_SINGLETHREAD - no additional args
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
            case 1:
                // SQLITE_CONFIG_MULTITHREAD - no additional args
                sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                break;
            case 2:
                // SQLITE_CONFIG_SERIALIZED - no additional args
                sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                break;
            case 3:
                // SQLITE_CONFIG_MEMSTATUS - requires one int
                if (Size > 3) {
                    sqlite3_config(SQLITE_CONFIG_MEMSTATUS, Data[2] & 0x01);
                }
                break;
            case 4:
                // SQLITE_CONFIG_LOOKASIDE - requires two ints
                if (Size > 4) {
                    // Use safe, small values for lookaside buffer
                    int slot_size = (Data[2] % 128) + 64;  // 64-191 bytes
                    int slot_count = (Data[3] % 32) + 8;   // 8-39 slots
                    sqlite3_config(SQLITE_CONFIG_LOOKASIDE, slot_size, slot_count);
                }
                break;
            case 5:
                // SQLITE_CONFIG_PCACHE2 - requires pointer, skip
                break;
            case 6:
                // SQLITE_CONFIG_GETPCACHE2 - requires pointer, skip
                break;
            case 7:
                // SQLITE_CONFIG_LOG - requires function pointer, skip
                break;
            case 8:
                // SQLITE_CONFIG_URI - requires int
                if (Size > 3) {
                    sqlite3_config(SQLITE_CONFIG_URI, Data[2] & 0x01);
                }
                break;
            case 9:
                // SQLITE_CONFIG_COVERING_INDEX_SCAN - requires int
                if (Size > 3) {
                    sqlite3_config(SQLITE_CONFIG_COVERING_INDEX_SCAN, Data[2] & 0x01);
                }
                break;
            default:
                // Do nothing for unknown options
                break;
        }
    }
    
    // 6. sqlite3_initialize - initialize SQLite library
    if (action & 0x20) {
        sqlite3_initialize();
    }
    
    // Try calling functions in different orders to explore state transitions
    if (Size > 3) {
        uint8_t secondary_action = Data[3] % 8; // Limit to safe sequences
        
        // Based on secondary action, try different sequences
        switch (secondary_action) {
            case 0:
                // Initialize -> use -> shutdown sequence
                sqlite3_initialize();
                sqlite3_threadsafe();
                sqlite3_shutdown();
                break;
            case 1:
                // Shutdown -> initialize -> config sequence
                sqlite3_shutdown();
                sqlite3_initialize();
                if (Size > 4) {
                    // Use a safe config option
                    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                }
                break;
            case 2:
                // Multiple initialize/shutdown calls
                sqlite3_initialize();
                sqlite3_initialize(); // Should be no-op
                sqlite3_shutdown();
                sqlite3_shutdown(); // Should be no-op
                break;
            case 3:
                // Thread-related calls
                sqlite3_thread_cleanup();
                sqlite3_threadsafe();
                if (Size > 4) {
                    sqlite3_enable_shared_cache(Data[4] & 0x01);
                }
                break;
            case 4:
                // Simple initialize and shutdown
                sqlite3_initialize();
                sqlite3_shutdown();
                break;
            case 5:
                // Just thread-safe check
                sqlite3_threadsafe();
                break;
            case 6:
                // Initialize, check thread safety, shutdown
                sqlite3_initialize();
                (void)sqlite3_threadsafe();
                sqlite3_shutdown();
                break;
            case 7:
                // Shutdown, thread cleanup, initialize
                sqlite3_shutdown();
                sqlite3_thread_cleanup();
                sqlite3_initialize();
                break;
        }
    }
    
    // Always end with a shutdown to clean up resources
    sqlite3_shutdown();
    
    return 0;
}
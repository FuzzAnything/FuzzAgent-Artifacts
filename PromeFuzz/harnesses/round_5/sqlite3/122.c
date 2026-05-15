// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    // Use fuzzer input to determine various parameters
    int param = 0;
    if (Size > 0) {
        param = Data[0] % 256;
    }

    // Call sqlite3_initialize - safe to call multiple times
    sqlite3_initialize();

    // Call sqlite3_enable_shared_cache with parameter from fuzzer input
    // Use modulo 2 to get 0 or 1 for enable/disable
    sqlite3_enable_shared_cache(param % 2);

    // Call sqlite3_threadsafe - just returns compile-time setting
    sqlite3_threadsafe();

    // Call sqlite3_config with various options based on fuzzer input
    // We'll try a few different configuration options that are safe to use
    int config_param = param;
    
    // Try different configuration options based on input
    switch (param % 5) {
        case 0:
            // SQLITE_CONFIG_SINGLETHREAD
            sqlite3_config(2001);  // 2001 = SQLITE_CONFIG_SINGLETHREAD
            break;
        case 1:
            // SQLITE_CONFIG_MULTITHREAD  
            sqlite3_config(2002);  // 2002 = SQLITE_CONFIG_MULTITHREAD
            break;
        case 2:
            // SQLITE_CONFIG_SERIALIZED
            sqlite3_config(2003);  // 2003 = SQLITE_CONFIG_SERIALIZED
            break;
        case 3:
            // SQLITE_CONFIG_MEMSTATUS - takes int parameter
            sqlite3_config(2004, param % 2);  // 2004 = SQLITE_CONFIG_MEMSTATUS
            break;
        case 4:
            // SQLITE_CONFIG_LOOKASIDE - takes two int parameters
            sqlite3_config(2005, 128, 8);  // 2005 = SQLITE_CONFIG_LOOKASIDE
            break;
    }

    // Call sqlite3_thread_cleanup - no-op stub
    sqlite3_thread_cleanup();

    // Call sqlite3_shutdown - idempotent, can be called multiple times
    sqlite3_shutdown();

    // Re-initialize to leave library in usable state for next iteration
    sqlite3_initialize();

    return 0;
}
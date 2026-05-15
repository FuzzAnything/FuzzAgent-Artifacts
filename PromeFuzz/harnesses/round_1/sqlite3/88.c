// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_os_init at sqlite3.c:33787:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:33787:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    // Use the fuzzer input to influence behavior
    // We'll use the first few bytes to decide which functions to call and in what order
    
    // Initialize OS interface
    sqlite3_os_init();
    
    // Try different configuration options based on fuzzer input
    if (Size > 0) {
        int config_option = Data[0] % 10;
        
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
                // Try with an integer parameter
                if (Size > 1) {
                    int value = Data[1] % 2;
                    sqlite3_config(SQLITE_CONFIG_MEMSTATUS, value);
                }
                break;
            case 4:
                // Try with an integer parameter
                if (Size > 1) {
                    int value = Data[1] % 2;
                    sqlite3_config(SQLITE_CONFIG_URI, value);
                }
                break;
            // Other cases will just use default configuration
        }
    }
    
    // Initialize SQLite library
    sqlite3_initialize();
    
    // Call thread cleanup (no-op but part of API)
    sqlite3_thread_cleanup();
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    // Call OS end
    sqlite3_os_end();
    
    // Try calling initialize/shutdown multiple times with different patterns
    if (Size > 2) {
        int pattern = Data[2] % 4;
        
        for (int i = 0; i < (pattern + 1); i++) {
            sqlite3_initialize();
            sqlite3_shutdown();
        }
    }
    
    // Try calling os_init/os_end multiple times
    if (Size > 3) {
        int os_pattern = Data[3] % 3;
        
        for (int i = 0; i < os_pattern; i++) {
            sqlite3_os_init();
            sqlite3_os_end();
        }
    }
    
    // Final cleanup - ensure we leave library in shutdown state
    sqlite3_shutdown();
    sqlite3_os_end();
    
    return 0;
}
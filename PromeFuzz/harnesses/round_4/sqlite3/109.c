// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <unistd.h>

static void dummy_log(void* pArg, int iErrCode, const char* zMsg) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    // Use first byte to determine which functions to call and in what order
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    
    // Try different sequences based on selector bits
    if (selector & 0x01) {
        sqlite3_initialize();
    }
    
    if (selector & 0x02) {
        sqlite3_thread_cleanup();
    }
    
    if (selector & 0x04) {
        // Use second byte (if available) as enable flag, otherwise use 1
        int enable = 1;
        if (Size > 1) {
            enable = Data[1] & 0x01;
        }
        sqlite3_enable_shared_cache(enable);
    }
    
    if (selector & 0x08) {
        sqlite3_shutdown();
    }
    
    if (selector & 0x10) {
        sqlite3_threadsafe();
    }
    
    if (selector & 0x20) {
        // Try different sqlite3_config options based on input data
        if (Size > 2) {
            int option = Data[2] % 6;  // Reduced to 6 safe option values
            
            switch (option) {
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
                    sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 1);
                    break;
                case 4:
                    sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 0);
                    break;
                case 5:
                    // Use the dummy_log function defined above
                    sqlite3_config(SQLITE_CONFIG_LOG, dummy_log, NULL);
                    break;
            }
        }
    }
    
    // Try calling initialize/shutdown multiple times with different patterns
    if (selector & 0x40) {
        for (int i = 0; i < 3; i++) {
            sqlite3_initialize();
            sqlite3_shutdown();
        }
    }
    
    if (selector & 0x80) {
        // Call thread_cleanup multiple times
        for (int i = 0; i < 5; i++) {
            sqlite3_thread_cleanup();
        }
    }
    
    return 0;
}
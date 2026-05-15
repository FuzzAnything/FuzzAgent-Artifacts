// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
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
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:33787:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_sqlite3_config(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    int op = data[0] % 10;  // Use modulo to get a limited set of options
    int result;
    
    switch (op) {
        case 0:
            result = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            break;
        case 1:
            result = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
            break;
        case 2:
            result = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
            break;
        case 3:
            result = sqlite3_config(SQLITE_CONFIG_MEMSTATUS, data[0] & 1);
            break;
        case 4:
            result = sqlite3_config(SQLITE_CONFIG_SCRATCH, NULL, 0, 0);
            break;
        case 5:
            result = sqlite3_config(SQLITE_CONFIG_PAGECACHE, NULL, 0, 0);
            break;
        case 6:
            result = sqlite3_config(SQLITE_CONFIG_HEAP, NULL, 0, 0);
            break;
        case 7:
            result = sqlite3_config(SQLITE_CONFIG_MUTEX, NULL);
            break;
        case 8:
            result = sqlite3_config(SQLITE_CONFIG_GETMUTEX, NULL);
            break;
        case 9:
            result = sqlite3_config(SQLITE_CONFIG_LOOKASIDE, 0, 0);
            break;
        default:
            result = sqlite3_config(op);
            break;
    }
    (void)result;  // Ignore return value
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine the sequence of operations
    uint8_t sequence = Data[0];
    size_t data_offset = 1;
    
    // Try different sequences based on the first byte
    if (sequence & 0x01) {
        // Initialize sequence
        sqlite3_initialize();
        
        if (sequence & 0x02) {
            // Try some config operations after initialization (may fail)
            fuzz_sqlite3_config(Data + data_offset, 
                               Size > data_offset ? Size - data_offset : 0);
            data_offset += 1;
        }
        
        if (sequence & 0x04) {
            // Reset auto extensions
            sqlite3_reset_auto_extension();
        }
        
        if (sequence & 0x08) {
            // Shutdown
            sqlite3_shutdown();
            
            if (sequence & 0x10) {
                // Try config after shutdown
                fuzz_sqlite3_config(Data + data_offset, 
                                   Size > data_offset ? Size - data_offset : 0);
                data_offset += 1;
            }
            
            if (sequence & 0x20) {
                // OS end
                sqlite3_os_end();
            }
        }
    } else {
        // Alternative sequence starting with OS init
        sqlite3_os_init();
        
        if (sequence & 0x40) {
            // Try config before initialization
            fuzz_sqlite3_config(Data + data_offset, 
                               Size > data_offset ? Size - data_offset : 0);
            data_offset += 1;
        }
        
        if (sequence & 0x80) {
            sqlite3_initialize();
            sqlite3_shutdown();
            sqlite3_os_end();
        }
    }
    
    // Try calling initialize/shutdown multiple times (idempotent)
    if (Size > 1 && (Data[1] & 0x01)) {
        for (int i = 0; i < 3; i++) {
            sqlite3_initialize();
        }
    }
    
    if (Size > 1 && (Data[1] & 0x02)) {
        for (int i = 0; i < 3; i++) {
            sqlite3_shutdown();
        }
    }
    
    // Try reset_auto_extension multiple times
    if (Size > 1 && (Data[1] & 0x04)) {
        for (int i = 0; i < 2; i++) {
            sqlite3_reset_auto_extension();
        }
    }
    
    // Final cleanup to leave library in a known state
    sqlite3_shutdown();
    
    return 0;
}
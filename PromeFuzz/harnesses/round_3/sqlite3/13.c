// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_libversion at sqlite3.c:170987:24 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:252014:24 in sqlite3.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Call sqlite3_libversion
    const char *lib_version = sqlite3_libversion();
    (void)lib_version; // Suppress unused variable warning
    
    // Call sqlite3_sourceid
    const char *source_id = sqlite3_sourceid();
    (void)source_id; // Suppress unused variable warning
    
    // Call sqlite3_config 7 times with various configuration options
    // Use fuzzer data to determine which options to try
    
    // First sqlite3_config call
    if (Size > 0) {
        int option1 = Data[0] % 10; // Use first byte to choose option
        switch (option1) {
            case 0:
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
            case 1:
                sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                break;
            case 2:
                sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                break;
            default:
                // Try some other safe options
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
        }
    }
    
    // Second sqlite3_config call
    if (Size > 1) {
        int option2 = Data[1] % 10;
        switch (option2) {
            case 0:
                sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                break;
            case 1:
                sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                break;
            case 2:
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
            default:
                sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                break;
        }
    }
    
    // Third sqlite3_config call
    if (Size > 2) {
        int option3 = Data[2] % 10;
        switch (option3) {
            case 0:
                sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                break;
            case 1:
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
            case 2:
                sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                break;
            default:
                sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                break;
        }
    }
    
    // Fourth sqlite3_config call - try memstatus
    if (Size > 3) {
        int memstatus = Data[3] & 1; // 0 or 1
        sqlite3_config(SQLITE_CONFIG_MEMSTATUS, memstatus);
    }
    
    // Fifth sqlite3_config call - try URI
    if (Size > 4) {
        int uri = Data[4] & 1; // 0 or 1
        sqlite3_config(SQLITE_CONFIG_URI, uri);
    }
    
    // Sixth sqlite3_config call - try covering index scan
    if (Size > 5) {
        int covering_index = Data[5] & 1; // 0 or 1
        sqlite3_config(SQLITE_CONFIG_COVERING_INDEX_SCAN, covering_index);
    }
    
    // Seventh sqlite3_config call - try lookaside
    if (Size > 7) {
        int slot_size = Data[6] % 256;
        int slot_count = Data[7] % 256;
        if (slot_size > 0 && slot_count > 0) {
            sqlite3_config(SQLITE_CONFIG_LOOKASIDE, slot_size, slot_count);
        }
    }
    
    // Call sqlite3_initialize
    int init_result = sqlite3_initialize();
    (void)init_result; // Suppress unused variable warning
    
    // Call sqlite3_vfs_find
    // Use fuzzer data to create VFS name
    static char vfs_name[256];
    if (Size > 0) {
        size_t name_len = Size < 255 ? Size : 255;
        memcpy(vfs_name, Data, name_len);
        vfs_name[name_len] = '\0';
        
        // Also try NULL to get default VFS
        if (Size % 2 == 0) {
            sqlite3_vfs_find(NULL);
        }
        
        // Try with our generated name
        sqlite3_vfs_find(vfs_name);
    } else {
        // Try with NULL for default VFS
        sqlite3_vfs_find(NULL);
    }
    
    return 0;
}
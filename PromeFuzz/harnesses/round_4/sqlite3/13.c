// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Call sqlite3_libversion and sqlite3_sourceid (no arguments needed)
    const char *version = sqlite3_libversion();
    const char *source_id = sqlite3_sourceid();
    
    // Use fuzzer input to generate configuration options for sqlite3_config
    // We'll use bytes from Data to determine which config options to set
    // We need to call sqlite3_config 7 times as specified
    
    // First, ensure we have at least some data to work with
    size_t data_index = 0;
    
    // Call sqlite3_config 7 times with different options based on fuzzer input
    for (int i = 0; i < 7; i++) {
        if (data_index >= Size) {
            // If we run out of data, use default values
            sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            continue;
        }
        
        // Use a byte from the fuzzer input to determine which config option to use
        uint8_t config_byte = Data[data_index++];
        
        // Map the byte to different configuration options
        switch (config_byte % 10) {
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
                // SQLITE_CONFIG_MEMSTATUS takes an int argument
                sqlite3_config(SQLITE_CONFIG_MEMSTATUS, (config_byte >> 1) & 1);
                break;
            case 4:
                // SQLITE_CONFIG_LOOKASIDE takes two int arguments
                sqlite3_config(SQLITE_CONFIG_LOOKASIDE, 128, 500);
                break;
            case 5:
                // SQLITE_CONFIG_LOG - we need a callback, skip for safety
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
            case 6:
                // SQLITE_CONFIG_URI - takes an int argument
                sqlite3_config(SQLITE_CONFIG_URI, (config_byte >> 2) & 1);
                break;
            case 7:
                // SQLITE_CONFIG_COVERING_INDEX_SCAN - takes an int argument
                sqlite3_config(SQLITE_CONFIG_COVERING_INDEX_SCAN, (config_byte >> 3) & 1);
                break;
            case 8:
                // SQLITE_CONFIG_MEMSTATUS again with different value
                sqlite3_config(SQLITE_CONFIG_MEMSTATUS, (config_byte >> 4) & 1);
                break;
            case 9:
                // Default to single-threaded mode
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
        }
    }
    
    // Initialize SQLite library
    int init_result = sqlite3_initialize();
    
    // Call sqlite3_vfs_find with a VFS name derived from fuzzer input
    const char *vfs_name = NULL;
    
    // If we have enough data, create a null-terminated string for VFS name
    if (Size - data_index > 0) {
        // Limit the VFS name length to prevent buffer overflow
        size_t name_len = Size - data_index;
        if (name_len > 255) {
            name_len = 255;
        }
        
        // Allocate buffer for VFS name
        char *name_buf = (char *)malloc(name_len + 1);
        if (name_buf) {
            memcpy(name_buf, Data + data_index, name_len);
            name_buf[name_len] = '\0';
            vfs_name = name_buf;
        }
    }
    
    // Find VFS by name (or default if NULL)
    sqlite3_vfs *vfs = sqlite3_vfs_find(vfs_name);
    
    // Clean up allocated VFS name buffer
    if (vfs_name && vfs_name != (const char *)Data) {
        free((void *)vfs_name);
    }
    
    // Return 0 as required by libFuzzer
    return 0;
}
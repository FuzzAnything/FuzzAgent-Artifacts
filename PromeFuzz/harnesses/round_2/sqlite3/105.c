// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_config(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    int safe_options[] = {
        SQLITE_CONFIG_SINGLETHREAD,
        SQLITE_CONFIG_MULTITHREAD,
        SQLITE_CONFIG_SERIALIZED,
        SQLITE_CONFIG_MEMSTATUS,
        SQLITE_CONFIG_LOOKASIDE,
        SQLITE_CONFIG_URI,
        SQLITE_CONFIG_COVERING_INDEX_SCAN,
        SQLITE_CONFIG_MMAP_SIZE
    };
    int num_safe_options = sizeof(safe_options) / sizeof(safe_options[0]);
    
    int option = safe_options[data[0] % num_safe_options];
    int result;
    
    switch (option) {
        case SQLITE_CONFIG_SINGLETHREAD:
        case SQLITE_CONFIG_MULTITHREAD:
        case SQLITE_CONFIG_SERIALIZED:
            result = sqlite3_config(option);
            break;
        case SQLITE_CONFIG_MEMSTATUS:
            if (size >= 2) {
                int enable = data[1] % 2;
                result = sqlite3_config(option, enable);
            }
            break;
        case SQLITE_CONFIG_LOOKASIDE:
            if (size >= 5) {
                int slot_size = data[1] * 256 + data[2];
                int slot_count = data[3] * 256 + data[4];
                result = sqlite3_config(option, slot_size, slot_count);
            }
            break;
        case SQLITE_CONFIG_URI:
            if (size >= 2) {
                int enable = data[1] % 2;
                result = sqlite3_config(option, enable);
            }
            break;
        case SQLITE_CONFIG_COVERING_INDEX_SCAN:
            if (size >= 2) {
                int enable = data[1] % 2;
                result = sqlite3_config(option, enable);
            }
            break;
        case SQLITE_CONFIG_MMAP_SIZE:
            if (size >= 17) {
                sqlite3_int64 default_mmap = 0;
                sqlite3_int64 max_mmap = 0;
                for (int i = 0; i < 8 && (1 + i) < (int)size; i++) {
                    default_mmap = (default_mmap << 8) | data[1 + i];
                }
                for (int i = 0; i < 8 && (9 + i) < (int)size; i++) {
                    max_mmap = (max_mmap << 8) | data[9 + i];
                }
                result = sqlite3_config(option, default_mmap, max_mmap);
            }
            break;
        default:
            break;
    }
}

int LLVMFuzzerTestOneInput_105(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    uint8_t operation_selector = data[0];
    
    if (operation_selector % 4 == 0) {
        sqlite3_initialize();
        if (size > 1) {
            fuzz_config(data + 1, size - 1);
        }
        sqlite3_shutdown();
    } else if (operation_selector % 4 == 1) {
        sqlite3_shutdown();
        if (size > 1) {
            fuzz_config(data + 1, size - 1);
        }
        sqlite3_initialize();
        sqlite3_shutdown();
    } else if (operation_selector % 4 == 2) {
        for (int i = 0; i < 3 && (i + 1) < (int)size; i++) {
            if (data[i + 1] % 2 == 0) {
                sqlite3_initialize();
            } else {
                sqlite3_shutdown();
            }
        }
        
        if (size > 4) {
            fuzz_config(data + 4, size - 4);
        }
        
        sqlite3_shutdown();
    } else {
        sqlite3_threadsafe();
        
        if (size > 1 && data[1] % 2 == 0) {
            sqlite3_initialize();
        }
        
        if (size > 2) {
            int enable = data[2] % 2;
            sqlite3_enable_shared_cache(enable);
        }
        
        if (size > 3) {
            fuzz_config(data + 3, size - 3);
        }
        
        sqlite3_thread_cleanup();
        
        sqlite3_shutdown();
    }
    
    return 0;
}
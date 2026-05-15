// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:33787:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void fuzz_sqlite3_config(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    int option = data[0] % 8;  // Only use valid options
    
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
            if (size > 1) {
                sqlite3_config(SQLITE_CONFIG_MEMSTATUS, data[1] & 1);
            }
            break;
        case 4:
            // Use a small static buffer for scratch
            {
                static char scratch_buffer[1024];
                sqlite3_config(SQLITE_CONFIG_SCRATCH, scratch_buffer, 256, 4);
            }
            break;
        case 5:
            // Use a small static buffer for pagecache
            {
                static char pagecache_buffer[4096];
                sqlite3_config(SQLITE_CONFIG_PAGECACHE, pagecache_buffer, 1024, 4);
            }
            break;
        case 6:
            // Use a small static buffer for heap
            {
                static char heap_buffer[8192];
                sqlite3_config(SQLITE_CONFIG_HEAP, heap_buffer, sizeof(heap_buffer), 1);
            }
            break;
        case 7:
            sqlite3_config(SQLITE_CONFIG_MEMDB_MAXSIZE, (sqlite3_int64)(size % 1000000));
            break;
        default:
            // Do nothing for invalid options
            break;
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *data, size_t size) {
    // Initialize OS interface
    sqlite3_os_init();
    
    // Try various configurations before initialization
    if (size > 0) {
        fuzz_sqlite3_config(data, size);
    }
    
    // Initialize SQLite
    sqlite3_initialize();
    
    // Enable/disable shared cache based on input
    if (size > 0) {
        sqlite3_enable_shared_cache(data[0] & 1);
    }
    
    // Try more configurations after initialization (some will fail, but that's okay)
    if (size > 1) {
        fuzz_sqlite3_config(data + 1, size - 1);
    }
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    // End OS interface
    sqlite3_os_end();
    
    // Try to re-initialize and shutdown again
    sqlite3_initialize();
    sqlite3_shutdown();
    
    // Try shared cache again after shutdown
    if (size > 2) {
        sqlite3_enable_shared_cache(data[2] & 1);
    }
    
    // Final cleanup
    sqlite3_os_end();
    
    return 0;
}
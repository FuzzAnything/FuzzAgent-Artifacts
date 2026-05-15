// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_libversion_number at sqlite3.c:171000:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
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
// sqlite3_global_recover at sqlite3.c:174799:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    // Use Data to generate parameters for the API calls
    // We'll use the first few bytes to generate integer parameters
    
    // For sqlite3_compileoption_get - use first 4 bytes as integer N
    int n = 0;
    if (Size >= sizeof(int)) {
        n = *(const int*)Data;
    } else if (Size > 0) {
        n = (int)Data[0];
    }
    const char* compile_option = sqlite3_compileoption_get(n);
    (void)compile_option; // Use result to avoid unused variable warning
    
    // For sqlite3_enable_shared_cache - use next byte as boolean
    int enable_cache = 0;
    if (Size >= sizeof(int) + 1) {
        enable_cache = Data[sizeof(int)] & 1;
    } else if (Size > sizeof(int)) {
        enable_cache = Data[sizeof(int)] & 1;
    }
    int cache_result = sqlite3_enable_shared_cache(enable_cache);
    (void)cache_result;
    
    // For sqlite3_libversion_number - no parameters needed
    int version = sqlite3_libversion_number();
    (void)version;
    
    // For sqlite3_threadsafe - no parameters needed
    int thread_safe = sqlite3_threadsafe();
    (void)thread_safe;
    
    // For sqlite3_config - try different configuration options
    // We'll try a few common options based on the input data
    if (Size > 0) {
        int option = Data[0] % 10; // Use first byte to select option
        
        switch (option) {
            case 0:
                // SQLITE_CONFIG_SINGLETHREAD (1) - single-threaded mode
                sqlite3_config(1);
                break;
            case 1:
                // SQLITE_CONFIG_MULTITHREAD (2) - multi-threaded mode
                sqlite3_config(2);
                break;
            case 2:
                // SQLITE_CONFIG_SERIALIZED (3) - serialized mode
                sqlite3_config(3);
                break;
            case 3:
                // SQLITE_CONFIG_MEMSTATUS (9) - enable/disable memory status
                sqlite3_config(9, 1);
                break;
            case 4:
                // SQLITE_CONFIG_URI (17) - enable/disable URI handling
                sqlite3_config(17, 1);
                break;
            case 5:
                // SQLITE_CONFIG_COVERING_INDEX_SCAN (20) - enable/disable covering index scan
                sqlite3_config(20, 1);
                break;
            case 6:
                // SQLITE_CONFIG_PCACHE2 (18) - custom page cache
                // Pass NULL as we don't have a valid cache implementation
                sqlite3_config(18, NULL);
                break;
            case 7:
                // SQLITE_CONFIG_GETPCACHE2 (19) - get page cache
                // Pass NULL pointer to receive current cache
                sqlite3_config(19, NULL);
                break;
            case 8:
                // SQLITE_CONFIG_LOG (16) - set logging callback
                // Pass NULL for both function and data
                sqlite3_config(16, NULL, NULL);
                break;
            case 9:
                // SQLITE_CONFIG_PMASZ (22) - set PMA size
                sqlite3_config(22, 250);
                break;
            default:
                // Default to single-threaded mode
                sqlite3_config(1);
                break;
        }
    }
    
    // For sqlite3_global_recover - no parameters needed
    int recover_result = sqlite3_global_recover();
    (void)recover_result;
    
    return 0;
}
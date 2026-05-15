// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Use the input data to generate configuration values
    int config_val1 = 0;
    int config_val2 = 0;
    int config_val3 = 0;
    int enable_cache = 0;
    
    // Extract values from fuzzer input if available
    if (Size >= 1) {
        config_val1 = Data[0] % 10;  // Limit to reasonable range
    }
    if (Size >= 2) {
        config_val2 = Data[1] % 10;
    }
    if (Size >= 3) {
        config_val3 = Data[2] % 10;
    }
    if (Size >= 4) {
        enable_cache = Data[3] % 2;  // 0 or 1
    }
    
    // Call sqlite3_config three times with different configuration options
    // Using safe configuration options that can be called before initialization
    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);  // Set single-thread mode
    sqlite3_config(SQLITE_CONFIG_MULTITHREAD);   // Set multi-thread mode
    sqlite3_config(SQLITE_CONFIG_SERIALIZED);    // Set serialized mode
    
    // Call sqlite3_threadsafe to check thread safety
    int thread_safe = sqlite3_threadsafe();
    
    // Initialize SQLite library
    int init_result = sqlite3_initialize();
    
    // Enable or disable shared cache based on fuzzer input
    int cache_result = sqlite3_enable_shared_cache(enable_cache);
    
    // Create a dummy file for database operations
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (fp) {
        // Write some initial data to the file if we have fuzzer input
        if (Size > 0) {
            size_t write_size = Size > 100 ? 100 : Size;  // Limit write size
            fwrite(Data, 1, write_size, fp);
        }
        fclose(fp);
    }
    
    // Open a database connection
    sqlite3* db = NULL;
    int open_result = sqlite3_open(filename, &db);
    
    // Close the database connection if opened successfully
    if (db) {
        int close_result = sqlite3_close(db);
    }
    
    // Clean up the dummy file
    remove(filename);
    
    // Allocate some memory using SQLite's allocator and free it
    // Use a small allocation to avoid excessive memory usage
    size_t alloc_size = Size > 0 ? (Data[0] % 256) + 1 : 16;
    void* mem = sqlite3_malloc(alloc_size);
    if (mem) {
        // Initialize the memory with some data if we have fuzzer input
        if (Size > 0 && alloc_size > 0) {
            size_t copy_size = alloc_size < Size ? alloc_size : Size;
            memcpy(mem, Data, copy_size);
        }
        sqlite3_free(mem);
    }
    
    // Also test sqlite3_free with NULL pointer (should be safe)
    sqlite3_free(NULL);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Use the fuzz data to influence our parameters
    int config_op1 = Size > 0 ? Data[0] % 10 : 0;
    int config_op2 = Size > 1 ? Data[1] % 10 : 0;
    int config_op3 = Size > 2 ? Data[2] % 10 : 0;
    int shared_cache_enable = Size > 3 ? Data[3] % 2 : 0;
    
    // Declare variables that might be needed for sqlite3_config calls
    int dummy_int = 0;
    void *dummy_ptr = NULL;
    
    // Call sqlite3_config three times with different options
    // We'll use some safe configuration options that are allowed before initialization
    
    // First config call
    if (config_op1 == 0) {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    } else if (config_op1 == 1) {
        sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    } else if (config_op1 == 2) {
        sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    } else if (config_op1 == 3) {
        sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 0);
    } else if (config_op1 == 4) {
        sqlite3_config(SQLITE_CONFIG_URI, 1);
    } else if (config_op1 == 5) {
        sqlite3_config(SQLITE_CONFIG_COVERING_INDEX_SCAN, 1);
    } else if (config_op1 == 6) {
        sqlite3_config(SQLITE_CONFIG_SQLLOG, NULL, NULL);
    } else if (config_op1 == 7) {
        sqlite3_config(SQLITE_CONFIG_MMAP_SIZE, (sqlite3_int64)0, (sqlite3_int64)0);
    } else if (config_op1 == 8) {
        sqlite3_config(SQLITE_CONFIG_WIN32_HEAPSIZE, 0);
    } else {
        sqlite3_config(SQLITE_CONFIG_PCACHE2, NULL);
    }
    
    // Second config call
    if (config_op2 == 0) {
        sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
    } else if (config_op2 == 1) {
        sqlite3_config(SQLITE_CONFIG_LOOKASIDE, 100, 1200);
    } else if (config_op2 == 2) {
        sqlite3_config(SQLITE_CONFIG_PAGECACHE, NULL, 0, 0);
    } else if (config_op2 == 3) {
        sqlite3_config(SQLITE_CONFIG_HEAP, NULL, 0, 0);
    } else if (config_op2 == 4) {
        sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 1);
    } else if (config_op2 == 5) {
        sqlite3_config(SQLITE_CONFIG_MUTEX, NULL);
    } else if (config_op2 == 6) {
        // SQLITE_CONFIG_GETMUTEX requires a valid pointer
        sqlite3_mutex_methods dummy_methods;
        sqlite3_config(SQLITE_CONFIG_GETMUTEX, &dummy_methods);
    } else if (config_op2 == 7) {
        sqlite3_config(SQLITE_CONFIG_SCRATCH, NULL, 0, 0);
    } else if (config_op2 == 8) {
        // SQLITE_CONFIG_PCACHE_HDRSZ requires a valid pointer
        sqlite3_config(SQLITE_CONFIG_PCACHE_HDRSZ, &dummy_int);
    } else {
        sqlite3_config(SQLITE_CONFIG_PMASZ, (unsigned int)0);
    }
    
    // Third config call
    if (config_op3 == 0) {
        sqlite3_config(SQLITE_CONFIG_STMTJRNL_SPILL, 0);
    } else if (config_op3 == 1) {
        sqlite3_config(SQLITE_CONFIG_SMALL_MALLOC, 0);
    } else if (config_op3 == 2) {
        sqlite3_config(SQLITE_CONFIG_SORTERREF_SIZE, 0);
    } else if (config_op3 == 3) {
        sqlite3_config(SQLITE_CONFIG_MEMDB_MAXSIZE, 0);
    } else if (config_op3 == 4) {
        sqlite3_config(SQLITE_CONFIG_ROWID_IN_VIEW, 0);
    } else if (config_op3 == 5) {
        sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    } else if (config_op3 == 6) {
        sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    } else if (config_op3 == 7) {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    } else if (config_op3 == 8) {
        sqlite3_config(SQLITE_CONFIG_COVERING_INDEX_SCAN, 0);
    } else {
        sqlite3_config(SQLITE_CONFIG_URI, 0);
    }
    
    // Check thread safety
    int threadsafe = sqlite3_threadsafe();
    
    // Initialize SQLite
    int init_result = sqlite3_initialize();
    
    // Enable or disable shared cache
    int shared_cache_result = sqlite3_enable_shared_cache(shared_cache_enable);
    
    // Create a database connection
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    
    // Write fuzz data to file if we have any
    if (Size > 0) {
        FILE *fp = fopen(filename, "wb");
        if (fp) {
            size_t write_size = Size > 1024 ? 1024 : Size; // Limit write size
            fwrite(Data, 1, write_size, fp);
            fclose(fp);
        }
    }
    
    // Open the database
    int open_result = sqlite3_open(filename, &db);
    
    // Close the database if opened successfully
    if (db) {
        int close_result = sqlite3_close(db);
    } else {
        // Even if db is NULL, sqlite3_close should handle it safely
        sqlite3_close(db);
    }
    
    // Test sqlite3_free with some allocations
    if (Size > 10) {
        // Allocate some memory using sqlite3_malloc (which sqlite3_free can free)
        size_t alloc_size = Size > 100 ? 100 : Size;
        void *mem = sqlite3_malloc(alloc_size);
        if (mem) {
            // Copy some fuzz data into the allocated memory
            size_t copy_size = alloc_size < Size ? alloc_size : Size;
            memcpy(mem, Data, copy_size);
            
            // Free the memory
            sqlite3_free(mem);
        }
        
        // Test with NULL pointer (should be safe)
        sqlite3_free(NULL);
        
        // Test with another allocation
        if (Size > 20) {
            void *mem2 = sqlite3_malloc(50);
            sqlite3_free(mem2);
        }
    }
    
    return 0;
}
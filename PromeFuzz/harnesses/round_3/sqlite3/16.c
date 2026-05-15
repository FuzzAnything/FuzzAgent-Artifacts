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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Use the fuzzer input to derive configuration values
    int config_vals[3] = {0, 0, 0};
    for (int i = 0; i < 3 && i * sizeof(int) < Size; i++) {
        memcpy(&config_vals[i], Data + i * sizeof(int), sizeof(int));
    }
    
    // First three sqlite3_config calls
    sqlite3_config(config_vals[0] % 100); // Use modulo to get reasonable values
    sqlite3_config(config_vals[1] % 100);
    sqlite3_config(config_vals[2] % 100);
    
    // Check thread safety
    int thread_safe = sqlite3_threadsafe();
    (void)thread_safe; // Use result to avoid unused variable warning
    
    // Initialize SQLite
    int init_result = sqlite3_initialize();
    if (init_result != SQLITE_OK) {
        // If initialization fails, we can't proceed with other calls
        return 0;
    }
    
    // Enable/disable shared cache based on input
    int enable_cache = (Size > 0) ? (Data[0] % 2) : 0;
    sqlite3_enable_shared_cache(enable_cache);
    
    // Open a database
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    
    // Write fuzzer data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    int open_result = sqlite3_open(filename, &db);
    
    // Close the database if opened successfully
    if (open_result == SQLITE_OK && db) {
        sqlite3_close(db);
    }
    
    // Free some memory (safe to call with NULL)
    void *mem_to_free = NULL;
    if (Size > sizeof(void*)) {
        // Allocate a small buffer to free
        mem_to_free = sqlite3_malloc(10);
        if (mem_to_free) {
            sqlite3_free(mem_to_free);
        }
    }
    
    // Also free NULL pointer (should be safe)
    sqlite3_free(NULL);
    
    // Clean up the dummy file
    remove(filename);
    
    return 0;
}
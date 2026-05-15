// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:171006:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Use the first few bytes to influence behavior
    int config_op1 = (Size > 0) ? (Data[0] % 10) : 0;
    int config_op2 = (Size > 1) ? (Data[1] % 10) : 0;
    int config_op3 = (Size > 2) ? (Data[2] % 10) : 0;
    int enable_cache = (Size > 3) ? (Data[3] % 2) : 0;
    int open_flags = (Size > 4) ? (Data[4] % 4) : 0;

    // First three sqlite3_config calls (must be before initialization)
    // We'll use different configuration options based on fuzzer input
    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD + (config_op1 % 3));
    sqlite3_config(SQLITE_CONFIG_MULTITHREAD + (config_op2 % 3));
    sqlite3_config(SQLITE_CONFIG_SERIALIZED + (config_op3 % 3));

    // Check thread safety (compile-time setting)
    int threadsafe = sqlite3_threadsafe();

    // Initialize SQLite library
    int init_result = sqlite3_initialize();
    if (init_result != SQLITE_OK) {
        // If initialization fails, we can't proceed
        return 0;
    }

    // Enable or disable shared cache
    sqlite3_enable_shared_cache(enable_cache);

    // Open a database connection
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    
    // Write fuzzer data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write minimal data if empty input
            const char *minimal = "CREATE TABLE t(x);";
            fwrite(minimal, 1, strlen(minimal), fp);
        }
        fclose(fp);
    }

    // Try to open the database with different flags based on fuzzer input
    int open_result = sqlite3_open(filename, &db);
    
    if (open_result == SQLITE_OK && db != NULL) {
        // Close the database connection
        sqlite3_close(db);
    } else if (db != NULL) {
        // Even if open failed, we should close if db was allocated
        sqlite3_close(db);
    }

    // Test sqlite3_free with some allocated memory
    if (Size > 10) {
        void *mem = sqlite3_malloc(Size);
        if (mem != NULL) {
            // Copy some data to the allocated memory
            size_t copy_size = (Size < 100) ? Size : 100;
            memcpy(mem, Data, copy_size);
            sqlite3_free(mem);
        }
    }

    // Clean up the dummy file
    remove(filename);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sqlite3.h"

static int dummy_busy_handler(void *data, int count) {
    // Return non-zero to retry, zero to abort
    // For fuzzing, we can vary behavior based on input
    return (count < 5) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    // Initialize random seed based on input data
    unsigned int seed = 0;
    if (Size >= sizeof(seed)) {
        memcpy(&seed, Data, sizeof(seed));
    } else {
        seed = (unsigned int)Size;
    }
    srand(seed);

    // 1. Test sqlite3_sleep with various inputs
    int sleep_ms = 0;
    if (Size >= sizeof(sleep_ms)) {
        memcpy(&sleep_ms, Data, sizeof(sleep_ms));
    } else if (Size > 0) {
        sleep_ms = (int)Data[0];
    }
    
    // Test with positive, negative, and zero values
    sqlite3_sleep(sleep_ms);
    sqlite3_sleep(-1);
    sqlite3_sleep(0);
    sqlite3_sleep(1);
    sqlite3_sleep(100);

    // 2. Test sqlite3_open with various inputs
    sqlite3 *db = NULL;
    int rc;
    
    // Try to open a database with fuzzer data as filename (limited to safe chars)
    char filename[256] = "./dummy_file";
    
    // Write fuzzer data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        size_t write_size = Size;
        if (write_size > 1024) write_size = 1024; // Limit to 1KB
        if (write_size > 0) {
            fwrite(Data, 1, write_size, fp);
        }
        fclose(fp);
    }
    
    // Try to open the database
    rc = sqlite3_open(filename, &db);
    
    if (rc == SQLITE_OK && db != NULL) {
        // 3. Test sqlite3_busy_handler
        void *busy_data = (void *)(uintptr_t)seed;
        sqlite3_busy_handler(db, dummy_busy_handler, busy_data);
        
        // Test with NULL handler (should clear existing handler)
        sqlite3_busy_handler(db, NULL, NULL);
        
        // Test with our handler again
        sqlite3_busy_handler(db, dummy_busy_handler, busy_data);
        
        // 4. Test sqlite3_busy_timeout
        int timeout_ms = 0;
        if (Size >= sizeof(timeout_ms)) {
            memcpy(&timeout_ms, Data + sizeof(sleep_ms), sizeof(timeout_ms));
        }
        
        // Test various timeout values
        sqlite3_busy_timeout(db, timeout_ms);
        sqlite3_busy_timeout(db, -1);
        sqlite3_busy_timeout(db, 0);
        sqlite3_busy_timeout(db, 100);
        sqlite3_busy_timeout(db, 5000);
        
        // 5. Test sqlite3_setlk_timeout (if available)
        int setlk_timeout_ms = 0;
        int setlk_flags = 0;
        
        if (Size >= sizeof(setlk_timeout_ms) + sizeof(setlk_flags)) {
            memcpy(&setlk_timeout_ms, Data + sizeof(sleep_ms) + sizeof(timeout_ms), 
                   sizeof(setlk_timeout_ms));
            memcpy(&setlk_flags, Data + sizeof(sleep_ms) + sizeof(timeout_ms) + sizeof(setlk_timeout_ms),
                   sizeof(setlk_flags));
        }
        
        // Test various values
        sqlite3_setlk_timeout(db, setlk_timeout_ms, setlk_flags);
        sqlite3_setlk_timeout(db, -1, 0);
        sqlite3_setlk_timeout(db, 0, 0);
        sqlite3_setlk_timeout(db, 100, 0);
        sqlite3_setlk_timeout(db, 1000, 1);
        
        // 6. Test sqlite3_is_interrupted
        int is_interrupted = sqlite3_is_interrupted(db);
        (void)is_interrupted; // Use result to avoid unused variable warning
        
        // Clean up
        sqlite3_close(db);
    } else {
        // If open failed, still try to close if db was partially created
        if (db) {
            sqlite3_close(db);
        }
    }
    
    // Remove the dummy file
    remove(filename);
    
    // Create another database connection to test more states
    sqlite3 *db2 = NULL;
    rc = sqlite3_open(":memory:", &db2);
    if (rc == SQLITE_OK && db2 != NULL) {
        // Test all functions again with in-memory database
        sqlite3_busy_handler(db2, dummy_busy_handler, (void *)(uintptr_t)(seed + 1));
        sqlite3_busy_timeout(db2, 100);
        sqlite3_setlk_timeout(db2, 500, 0);
        sqlite3_is_interrupted(db2);
        sqlite3_close(db2);
    }
    
    // Test with NULL database pointer for sqlite3_is_interrupted
    // (should handle gracefully according to SQLite's robustness)
    sqlite3_is_interrupted(NULL);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int dummy_busy_handler(void *arg, int count) {
    // Return non-zero to retry, zero to abort
    // Use the count to decide - for fuzzing, we can use a simple heuristic
    return (count < 5) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Initialize database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }

    // Initialize prepared statement
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE IF NOT EXISTS t1(a, b); INSERT INTO t1 VALUES(1, 2); SELECT * FROM t1;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    // Use the fuzzer input to influence parameters
    int sleep_ms = 0;
    int busy_timeout = 0;
    int setlk_timeout = 0;
    int setlk_flags = 0;
    int busy_handler_arg = 0;
    
    // Extract parameters from fuzzer input if available
    if (Size >= 1) sleep_ms = Data[0] % 1000;  // Limit to reasonable sleep time
    if (Size >= 2) busy_timeout = (int8_t)Data[1];  // Can be negative
    if (Size >= 3) setlk_timeout = (int8_t)Data[2];  // Can be negative
    if (Size >= 4) setlk_flags = Data[3] & 1;  // Only use lowest bit for flags
    if (Size >= 5) busy_handler_arg = Data[4];
    
    // Call target API functions
    
    // 1. sqlite3_sleep
    int actual_sleep = sqlite3_sleep(sleep_ms);
    (void)actual_sleep;  // Ignore return value
    
    // 2. sqlite3_busy_timeout
    sqlite3_busy_timeout(db, busy_timeout);
    
    // 3. sqlite3_setlk_timeout
    sqlite3_setlk_timeout(db, setlk_timeout, setlk_flags);
    
    // 4. sqlite3_is_interrupted
    int interrupted = sqlite3_is_interrupted(db);
    (void)interrupted;  // Ignore return value
    
    // 5. sqlite3_busy_handler
    sqlite3_busy_handler(db, dummy_busy_handler, (void*)(uintptr_t)busy_handler_arg);
    
    // 6. sqlite3_step - execute prepared statement if available
    if (stmt != NULL) {
        // Execute the statement multiple times to explore different states
        for (int i = 0; i < 3; i++) {
            rc = sqlite3_step(stmt);
            while (rc == SQLITE_ROW) {
                // Process row if needed
                rc = sqlite3_step(stmt);
            }
            
            if (rc == SQLITE_DONE || rc == SQLITE_ERROR || rc == SQLITE_BUSY) {
                sqlite3_reset(stmt);
            }
            
            // Break early if we get an error
            if (rc != SQLITE_ROW && rc != SQLITE_DONE && rc != SQLITE_BUSY) {
                break;
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Create a dummy file if needed (not used in this fuzzer but following instructions)
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        }
        fclose(fp);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc;

    // Initialize database connection for functions requiring it
    // Using a temporary file in memory or a dummy file as per instructions
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // If opening fails, we can still test global functions
        // but we won't test db-specific ones.
    }

    // Extract fuzzing parameters from input data
    int op = (Size > 0) ? (int)Data[0] : 0;
    int resetFlag = (Size > 1) ? (Data[1] % 2) : 0;
    sqlite3_int64 limitVal = (Size > 2) ? ((sqlite3_int64)Data[2] << 16) | (Size > 3 ? Data[3] : 0) : 0;
    
    // Randomize limitVal to include negative values for querying
    if (Size > 4) {
        if (Data[4] & 1) {
            limitVal = -limitVal;
        }
    }

    // Variables to hold status results
    sqlite3_int64 pCurrent64 = 0;
    sqlite3_int64 pHighwater64 = 0;
    int pCurrent32 = 0;
    int pHighwater32 = 0;

    // 1. Test sqlite3_status64 (Global metrics)
    // Valid ops are typically SQLITE_STATUS_MEMORY_USED, SQLITE_STATUS_PAGECACHE_USED, etc.
    // We fuzz the op code to trigger potential invalid input handling (SQLITE_MISUSE)
    rc = sqlite3_status64(op, &pCurrent64, &pHighwater64, resetFlag);
    (void)rc;

    // 2. Test sqlite3_soft_heap_limit64
    // Negative values query, positive set limit, 0 disables.
    sqlite3_int64 prevLimit = sqlite3_soft_heap_limit64(limitVal);
    (void)prevLimit;

    // 3. Test sqlite3_memory_used
    sqlite3_int64 memUsed = sqlite3_memory_used();
    (void)memUsed;

    // 4. Test sqlite3_status (32-bit version)
    rc = sqlite3_status(op, &pCurrent32, &pHighwater32, resetFlag);
    (void)rc;

    // 5. Test sqlite3_db_status64 (Connection specific)
    if (db) {
        // Fuzz the operation code for db status as well
        int dbOp = (Size > 5) ? (int)Data[5] : 0;
        sqlite3_int64 dbCurrent = 0;
        sqlite3_int64 dbHighwater = 0;
        int dbReset = (Size > 6) ? (Data[6] % 2) : 0;
        
        rc = sqlite3_db_status64(db, dbOp, &dbCurrent, &dbHighwater, dbReset);
        (void)rc;

        // Execute a simple query to generate some internal state for db_status to measure
        const char *sql = "SELECT 1;";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    // 6. Test sqlite3_memory_highwater
    sqlite3_int64 memHighwater = sqlite3_memory_highwater(resetFlag);
    (void)memHighwater;

    // Cleanup
    cleanup_db(db);

    return 0;
}
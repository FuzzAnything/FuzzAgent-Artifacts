// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Helper to safely open a database connection for fuzzing */
static sqlite3 *open_fuzz_db(void) {
    sqlite3 *db = NULL;
    /* Use a dummy file for testing */
    int rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK && rc != SQLITE_IOERR) {
        /* If opening fails for reasons other than IO (which is expected for dummy), return NULL */
        if (db) sqlite3_close(db);
        return NULL;
    }
    return db;
}

/* Helper to safely close a database connection */
static void close_fuzz_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Strategy: 
     * 1. Initialize a database connection to exercise connection-specific status APIs.
     * 2. Use input data to drive parameters for heap limits and status checks.
     * 3. Call global memory status functions.
     * 4. Call connection-specific status functions.
     * 5. Clean up.
     */

    sqlite3 *db = open_fuzz_db();
    
    /* 
     * Extract parameters from input data. 
     * We use modulo arithmetic to ensure values are within reasonable bounds 
     * to avoid immediate crashes from invalid pointer arithmetic, 
     * though the APIs themselves handle invalid values gracefully.
     * CRITICAL: Always check Size before accessing Data[i]
     */
    
    /* 1. Fuzz sqlite3_memory_used (no args, just call it) */
    sqlite3_int64 mem_used = sqlite3_memory_used();
    (void)mem_used;

    /* 2. Fuzz sqlite3_memory_highwater */
    int reset_flag_highwater = (Size > 0) ? (Data[0] % 2) : 0;
    sqlite3_int64 mem_highwater = sqlite3_memory_highwater(reset_flag_highwater);
    (void)mem_highwater;

    /* 3. Fuzz sqlite3_status */
    int op_status = 0;
    int pCurrent_status = 0;
    int pHighwater_status = 0;
    int resetFlag_status = 0;

    if (Size > 2) {
        /* Use input to select a status operation code. 
         * Common codes: SQLITE_STATUS_MEMORY_USED (0), SQLITE_STATUS_PAGECACHE_USED (1), etc.
         * We limit to a small range to stay within defined constants mostly.
         */
        op_status = Data[1] % 10; 
        resetFlag_status = (Data[2] % 2);
    }

    int rc_status = sqlite3_status(op_status, &pCurrent_status, &pHighwater_status, resetFlag_status);
    (void)rc_status;
    (void)pCurrent_status;
    (void)pHighwater_status;

    /* 4. Fuzz sqlite3_soft_heap_limit64 */
    sqlite3_int64 soft_limit = -1;
    if (Size > 3) {
        /* Construct a limit value. 
         * If the value is negative, it queries the limit.
         * If positive, it sets it. 
         * We use a subset of bytes to form a 64-bit integer.
         */
        sqlite3_int64 limit_val = 0;
        if (Size >= 11) {
            memcpy(&limit_val, Data + 3, sizeof(sqlite3_int64));
        } else {
            limit_val = (sqlite3_int64)Data[3];
        }
        
        /* Ensure we don't set an absurdly huge limit that might crash the fuzzer environment,
         * but allow negative values for querying. */
        if (limit_val > 1024 * 1024 * 1024LL) { 
            limit_val = 1024 * 1024 * 1024LL; 
        }
        soft_limit = sqlite3_soft_heap_limit64(limit_val);
    } else {
        soft_limit = sqlite3_soft_heap_limit64(-1);
    }
    (void)soft_limit;

    /* 5. Fuzz sqlite3_hard_heap_limit64 */
    sqlite3_int64 hard_limit = -1;
    if (Size > 4) {
        sqlite3_int64 limit_val = 0;
        if (Size >= 12) {
            memcpy(&limit_val, Data + 4, sizeof(sqlite3_int64));
        } else {
            limit_val = (sqlite3_int64)Data[4];
        }
        
        if (limit_val > 1024 * 1024 * 1024LL) {
            limit_val = 1024 * 1024 * 1024LL;
        }
        hard_limit = sqlite3_hard_heap_limit64(limit_val);
    } else {
        hard_limit = sqlite3_hard_heap_limit64(-1);
    }
    (void)hard_limit;

    /* 6. Fuzz sqlite3_db_status64 (requires valid db connection) */
    if (db) {
        int op_db_status = 0;
        sqlite3_int64 pCurrent_db = 0;
        sqlite3_int64 pHighwater_db = 0;
        int resetFlag_db = 0;

        if (Size > 6) {
            op_db_status = Data[5] % 10; /* Common codes: 0..9 */
            resetFlag_db = (Data[6] % 2);
        }

        int rc_db_status = sqlite3_db_status64(db, op_db_status, &pCurrent_db, &pHighwater_db, resetFlag_db);
        (void)rc_db_status;
        (void)pCurrent_db;
        (void)pHighwater_db;

        /* Perform some basic DB operations to change state before checking status again */
        sqlite3_exec(db, "SELECT 1;", NULL, NULL, NULL);
        sqlite3_exec(db, "SELECT random();", NULL, NULL, NULL);
        
        /* Re-check status after operations */
        sqlite3_db_status64(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &pCurrent_db, &pHighwater_db, 0);
        sqlite3_db_status64(db, SQLITE_DBSTATUS_LOOKASIDE_HIT, &pCurrent_db, &pHighwater_db, 1);
    }

    /* Cleanup */
    close_fuzz_db(db);

    return 0;
}
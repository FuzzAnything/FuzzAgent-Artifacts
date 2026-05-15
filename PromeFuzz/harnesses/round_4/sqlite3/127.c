// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static int dummy_busy_handler(void *ptr, int count) {
    // Return 0 to indicate we don't want to retry
    return 0;
}

static int dummy_progress_handler(void *ptr) {
    return 0;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *zTail = NULL;
    const void *zTail16 = NULL;
    int rc;
    const char *normalized_sql = NULL;
    int idx, op, resetFlg;
    void *pOut;
    int status_val;
    unsigned int prepFlags;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set some handlers to avoid hangs
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    sqlite3_progress_handler(db, 1000, dummy_progress_handler, NULL);
    
    // Create a dummy table to ensure we have something to query
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a,b,c);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare statement using sqlite3_prepare_v3
    // Use Data as SQL string (treat as UTF-8)
    prepFlags = (Size > 0) ? (Data[0] % 4) : 0; // Random prep flags
    rc = sqlite3_prepare_v3(db, 
                           (const char*)Data, 
                           (int)(Size & 0x7FFFFFFF), // Limit to positive int
                           prepFlags,
                           &stmt, 
                           &zTail);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        // Get normalized SQL
        normalized_sql = sqlite3_normalized_sql(stmt);
        
        // Get statement status with various operations
        // Try different ops (0-2 are common SQLITE_STMTSTATUS values)
        for (op = 0; op < 3; op++) {
            resetFlg = (op % 2); // Alternate reset flag
            status_val = sqlite3_stmt_status(stmt, op, resetFlg);
            (void)status_val; // Use result to avoid unused variable warning
        }
        
        // Try scanstatus if compiled with SQLITE_ENABLE_STMT_SCANSTATUS
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        pOut = &status_val; // Use status_val as output buffer
        // Try a few indices
        for (idx = -1; idx < 2; idx++) {
            // Try different scan status operations (0-3 are common)
            for (op = 0; op < 4; op++) {
                sqlite3_stmt_scanstatus(stmt, idx, op, pOut);
            }
        }
        
        // Reset scan status counters
        sqlite3_stmt_scanstatus_reset(stmt);
        #endif
        
        // Finalize the statement
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Now try sqlite3_prepare16_v2
    // For UTF-16, we need at least 2 bytes for a valid string
    if (Size >= 2) {
        // Create a UTF-16 string by repeating the first two bytes
        // This is a simplified approach for fuzzing
        size_t utf16_size = (Size / 2) * 2; // Ensure even number of bytes
        if (utf16_size > 0) {
            rc = sqlite3_prepare16_v2(db,
                                     Data, // Treat as UTF-16
                                     (int)(utf16_size & 0x7FFFFFFF),
                                     &stmt,
                                     &zTail16);
            
            if (rc == SQLITE_OK && stmt != NULL) {
                // Get normalized SQL from UTF-16 prepared statement
                normalized_sql = sqlite3_normalized_sql(stmt);
                
                // Get statement status
                for (op = 0; op < 3; op++) {
                    resetFlg = (op % 2);
                    status_val = sqlite3_stmt_status(stmt, op, resetFlg);
                    (void)status_val;
                }
                
                #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
                // Try scanstatus
                pOut = &status_val;
                for (idx = -1; idx < 2; idx++) {
                    for (op = 0; op < 4; op++) {
                        sqlite3_stmt_scanstatus(stmt, idx, op, pOut);
                    }
                }
                
                sqlite3_stmt_scanstatus_reset(stmt);
                #endif
                
                sqlite3_finalize(stmt);
            }
        }
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?;";
    const char *tail = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Enable extended result codes for better error reporting
    sqlite3_extended_result_codes(db, 1);
    
    // Create a simple table and insert some data
    sqlite3_exec(db, "CREATE TABLE t1(x INTEGER, y TEXT);", dummy_callback, 0, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'test');", dummy_callback, 0, NULL);
    
    // Prepare a statement using prepare_v2 to ensure saved SQL text
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind a parameter to the statement
    if (Size > 0) {
        sqlite3_bind_int(stmt, 1, (int)(Data[0] % 256));
    }
    
    // Use fuzz data to determine which operations to perform
    // We'll use the first few bytes to control behavior
    size_t idx = 0;
    
    // 1. Test sqlite3_stmt_explain with different modes
    if (idx < Size) {
        int explain_mode = Data[idx] % 3; // 0, 1, or 2
        idx++;
        
        // Reset before changing explain mode (as recommended)
        sqlite3_reset(stmt);
        sqlite3_stmt_explain(stmt, explain_mode);
    }
    
    // 2. Test sqlite3_reset
    if (idx < Size) {
        // Step the statement first to make it active
        if (Data[idx] % 2) {
            sqlite3_step(stmt);
        }
        sqlite3_reset(stmt);
        idx++;
    }
    
    // 3. Test sqlite3_stmt_status with various operations
    if (idx < Size) {
        int op = Data[idx] % 5; // Use first 5 status counters
        idx++;
        int resetFlg = 0;
        if (idx < Size) {
            resetFlg = Data[idx] % 2;
            idx++;
        }
        sqlite3_stmt_status(stmt, op, resetFlg);
    }
    
    // 4. Test sqlite3_stmt_busy
    if (idx < Size) {
        // Step the statement to potentially make it busy
        if (Data[idx] % 2) {
            sqlite3_step(stmt);
            // Don't step to completion to keep it busy sometimes
            if (Data[idx] % 3 == 0) {
                // Step multiple times
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    // Process row if needed
                }
            }
        }
        int busy = sqlite3_stmt_busy(stmt);
        (void)busy; // Use result to avoid unused variable warning
        idx++;
    }
    
    // 5. Test sqlite3_stmt_scanstatus_v2 (if available)
    // Note: This requires the statement to have been executed
    if (idx < Size) {
        // Reset and execute the statement first
        sqlite3_reset(stmt);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        int idx_param = 0;
        int iScanStatusOp = 0;
        int flags = 0;
        void *pOut = NULL;
        
        if (idx + 3 < Size) {
            idx_param = Data[idx] % 10;
            iScanStatusOp = Data[idx + 1] % 4; // Assuming 4 scan status ops
            flags = Data[idx + 2] % 2;
            idx += 3;
        }
        
        // Use a dummy buffer for output
        int64_t dummy_buffer = 0;
        pOut = &dummy_buffer;
        
        // Try to call the function (it may fail if not compiled with scanstatus support)
        sqlite3_stmt_scanstatus_v2(stmt, idx_param, iScanStatusOp, flags, pOut);
    }
    
    // 6. Test sqlite3_expired
    if (idx < Size) {
        // Potentially alter the schema to expire the statement
        if (Data[idx] % 4 == 0) {
            sqlite3_exec(db, "ALTER TABLE t1 ADD COLUMN z INTEGER;", dummy_callback, 0, NULL);
        }
        int expired = sqlite3_expired(stmt);
        (void)expired; // Use result to avoid unused variable warning
        idx++;
    }
    
    // Additional fuzzing: Try different SQL statements based on input
    if (Size > 10) {
        // Create a second statement with different SQL
        sqlite3_stmt *stmt2 = NULL;
        const char *sql2 = "SELECT * FROM t1 WHERE x > ?;";
        
        if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, &tail) == SQLITE_OK) {
            if (Size > 11) {
                sqlite3_bind_int(stmt2, 1, (int)(Data[10] % 256));
            }
            
            // Test explain on second statement
            if (Size > 12) {
                sqlite3_stmt_explain(stmt2, Data[11] % 3);
            }
            
            sqlite3_finalize(stmt2);
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
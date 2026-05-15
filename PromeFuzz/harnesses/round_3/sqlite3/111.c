// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_busy_handler(void *ptr, int count) {
    // Return 0 to stop waiting, 1 to retry
    return 0;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    sqlite3_stmt *pStmt = NULL;
    int rc;
    
    // Initialize database once
    if (!initialized) {
        rc = sqlite3_initialize();
        if (rc != SQLITE_OK) {
            return 0;
        }
        initialized = 1;
    }
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Set a busy handler to avoid SQLITE_BUSY errors
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    // Create a simple table and insert some data
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t1(id INTEGER PRIMARY KEY, val TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some dummy data
    const char *insert_sql = "INSERT INTO t1(val) VALUES('test1'), ('test2'), ('test3');";
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement - use a simple SELECT
    const char *select_sql = "SELECT * FROM t1 WHERE id > ?;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &pStmt, NULL);
    if (rc != SQLITE_OK || pStmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind a parameter if we have data
    if (Size > 0) {
        int bind_val = (int)Data[0];
        sqlite3_bind_int(pStmt, 1, bind_val);
    }
    
    // Step the statement to make it "busy"
    rc = sqlite3_step(pStmt);
    // Don't care about the result
    
    // Use the fuzz data to determine which operations to perform
    // We'll use the first few bytes to decide on parameters
    
    // 1. Test sqlite3_stmt_scanstatus if available
    #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
    if (Size > 0) {
        int idx = (Size > 1) ? ((int)Data[1] % 5) - 2 : -1; // Range: -2 to 2
        int iScanStatusOp = (Size > 2) ? ((int)Data[2] % 6) : 0; // Assuming 6 scanstatus options
        sqlite3_int64 out_int64 = 0;
        double out_double = 0.0;
        const char *out_str = NULL;
        
        // Try different output types based on operation
        if (iScanStatusOp == 2) { // SQLITE_SCANSTAT_EST returns double
            sqlite3_stmt_scanstatus(pStmt, idx, iScanStatusOp, &out_double);
        } else if (iScanStatusOp == 3 || iScanStatusOp == 4) { // NAME and EXPLAIN return const char*
            sqlite3_stmt_scanstatus(pStmt, idx, iScanStatusOp, &out_str);
        } else {
            sqlite3_stmt_scanstatus(pStmt, idx, iScanStatusOp, &out_int64);
        }
    }
    #endif
    
    // 2. Test sqlite3_stmt_scanstatus_reset if available
    #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
    sqlite3_stmt_scanstatus_reset(pStmt);
    #endif
    
    // 3. Test sqlite3_stmt_busy
    int busy = sqlite3_stmt_busy(pStmt);
    (void)busy; // Use result to avoid unused variable warning
    
    // 4. Test sqlite3_stmt_scanstatus_v2 if available
    #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
    if (Size > 0) {
        int idx = (Size > 1) ? ((int)Data[1] % 5) - 2 : -1;
        int iScanStatusOp = (Size > 2) ? ((int)Data[2] % 6) : 0;
        int flags = (Size > 3) ? (int)Data[3] : 0;
        sqlite3_int64 out_int64 = 0;
        double out_double = 0.0;
        const char *out_str = NULL;
        
        if (iScanStatusOp == 2) {
            sqlite3_stmt_scanstatus_v2(pStmt, idx, iScanStatusOp, flags, &out_double);
        } else if (iScanStatusOp == 3 || iScanStatusOp == 4) {
            sqlite3_stmt_scanstatus_v2(pStmt, idx, iScanStatusOp, flags, &out_str);
        } else {
            sqlite3_stmt_scanstatus_v2(pStmt, idx, iScanStatusOp, flags, &out_int64);
        }
    }
    #endif
    
    // 5. Test sqlite3_stmt_status
    if (Size > 0) {
        int op = (Size > 1) ? ((int)Data[1] % 10) : 0; // Assuming up to 10 status ops
        int resetFlg = (Size > 2) ? ((int)Data[2] & 1) : 0;
        int status = sqlite3_stmt_status(pStmt, op, resetFlg);
        (void)status;
    }
    
    // 6. Test sqlite3_stmt_explain
    if (Size > 0) {
        int eMode = (Size > 1) ? ((int)Data[1] % 3) : 0;
        // Reset statement before changing explain mode
        sqlite3_reset(pStmt);
        rc = sqlite3_stmt_explain(pStmt, eMode);
        (void)rc;
        
        // If explain mode changed, try to step again
        if (rc == SQLITE_OK) {
            sqlite3_step(pStmt);
        }
    }
    
    // Reset and step again to test different states
    sqlite3_reset(pStmt);
    if (Size > 0) {
        int bind_val = (int)Data[Size - 1];
        sqlite3_bind_int(pStmt, 1, bind_val);
        sqlite3_step(pStmt);
    }
    
    // Finalize the statement
    sqlite3_finalize(pStmt);
    
    // Close the database
    sqlite3_close(db);
    
    return 0;
}
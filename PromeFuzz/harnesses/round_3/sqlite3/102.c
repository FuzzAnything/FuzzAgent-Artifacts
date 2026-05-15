// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_busy_handler(void *data, int count) {
    // Return non-zero to retry, zero to abort
    return 1;
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static const char *dummy_filename = "./dummy_file";
    
    // Initialize sqlite3 once
    if (!initialized) {
        sqlite3_initialize();
        initialized = 1;
    }
    
    // Create a dummy database file
    FILE *fp = fopen(dummy_filename, "wb");
    if (!fp) {
        return 0;
    }
    
    // Write fuzzer data to file (at least 1 byte)
    if (Size > 0) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
    } else {
        // Write minimal data
        const char *min_data = "X";
        fwrite(min_data, 1, 1, fp);
    }
    fclose(fp);
    
    // Open database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(dummy_filename, &db);
    if (rc != SQLITE_OK || db == NULL) {
        // Clean up file
        remove(dummy_filename);
        return 0;
    }
    
    // Set a busy handler to avoid SQLITE_BUSY errors
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    // Create a simple table and insert some data
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a INTEGER, b TEXT);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT OR IGNORE INTO t1 VALUES(1, 'test');", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT OR IGNORE INTO t1 VALUES(2, 'data');", NULL, NULL, NULL);
    
    // Prepare a statement
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT * FROM t1 WHERE a > ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        // Bind a parameter
        if (Size > 0) {
            int bind_val = Data[0] % 10;
            sqlite3_bind_int(stmt, 1, bind_val);
        } else {
            sqlite3_bind_int(stmt, 1, 0);
        }
        
        // Step the statement to make it potentially busy
        rc = sqlite3_step(stmt);
        
        // Use fuzzer data to determine parameters for API calls
        int idx = -1;
        int iScanStatusOp = 0;
        int flags = 0;
        int op = 0;
        int resetFlg = 0;
        int eMode = 0;
        
        if (Size >= 6) {
            idx = (Data[1] % 3) - 1; // -1, 0, or 1
            iScanStatusOp = Data[2] % 10; // Various scan status ops
            flags = Data[3] & 1; // 0 or 1 (SQLITE_SCANSTAT_COMPLEX if defined)
            op = Data[4] % 5; // Various stmt status ops
            resetFlg = Data[5] & 1; // 0 or 1
            eMode = Data[5] % 3; // 0, 1, or 2
        }
        
        // Call sqlite3_stmt_scanstatus if available
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        {
            sqlite3_int64 out_val = 0;
            sqlite3_stmt_scanstatus(stmt, idx, iScanStatusOp, &out_val);
        }
        #endif
        
        // Call sqlite3_stmt_scanstatus_reset if available
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        sqlite3_stmt_scanstatus_reset(stmt);
        #endif
        
        // Call sqlite3_stmt_busy
        int busy = sqlite3_stmt_busy(stmt);
        (void)busy; // Suppress unused variable warning
        
        // Call sqlite3_stmt_scanstatus_v2 if available
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        {
            sqlite3_int64 out_val2 = 0;
            sqlite3_stmt_scanstatus_v2(stmt, idx, iScanStatusOp, flags, &out_val2);
        }
        #endif
        
        // Call sqlite3_stmt_status
        int status_val = sqlite3_stmt_status(stmt, op, resetFlg);
        (void)status_val; // Suppress unused variable warning
        
        // Call sqlite3_stmt_explain
        // Only call if statement is not busy (per documentation)
        if (!sqlite3_stmt_busy(stmt)) {
            sqlite3_stmt_explain(stmt, eMode);
        }
        
        // Reset and step again to explore different states
        sqlite3_reset(stmt);
        
        // Try stepping multiple times with different parameters
        for (int i = 0; i < 2 && rc != SQLITE_DONE; i++) {
            if (Size > i + 10) {
                sqlite3_bind_int(stmt, 1, Data[i + 10] % 20);
            }
            rc = sqlite3_step(stmt);
            
            // Call busy again in different state
            busy = sqlite3_stmt_busy(stmt);
            
            // Call status again with different parameters
            if (Size > i + 15) {
                int new_op = Data[i + 15] % 5;
                int new_reset = Data[i + 16] & 1;
                status_val = sqlite3_stmt_status(stmt, new_op, new_reset);
            }
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Get another statement using sqlite3_next_stmt
    sqlite3_stmt *next_stmt = NULL;
    if (db) {
        next_stmt = sqlite3_next_stmt(db, NULL);
        if (next_stmt) {
            // Test busy on this statement too
            int busy2 = sqlite3_stmt_busy(next_stmt);
            (void)busy2;
        }
    }
    
    // Close database
    sqlite3_close(db);
    
    // Remove dummy file
    remove(dummy_filename);
    
    return 0;
}
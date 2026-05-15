// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_prepare_statement(sqlite3 *db, sqlite3_stmt **stmt, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use a simple SQL statement for fuzzing
    const char *sql = "SELECT ?1, ?2";
    
    // Prepare statement
    sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
    
    if (*stmt) {
        // Bind some parameters using fuzz data
        if (size > 0) {
            sqlite3_bind_int(*stmt, 1, data[0]);
        }
        if (size > 1) {
            sqlite3_bind_int(*stmt, 2, data[1] % 100);
        }
        
        // Step the statement to put it in different states
        if (size > 2) {
            switch (data[2] % 4) {
                case 0:
                    // Leave idle
                    break;
                case 1:
                    // Step once
                    sqlite3_step(*stmt);
                    break;
                case 2:
                    // Step to completion
                    while (sqlite3_step(*stmt) == SQLITE_ROW) {}
                    break;
                case 3:
                    // Step and reset
                    sqlite3_step(*stmt);
                    sqlite3_reset(*stmt);
                    break;
            }
        }
    }
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple table
    sqlite3_exec(db, "CREATE TABLE t1(a INTEGER, b TEXT)", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'test')", NULL, NULL, NULL);
    
    // Prepare and setup statement
    fuzz_prepare_statement(db, &stmt, Data, Size);
    
    if (stmt) {
        // 1. Test sqlite3_stmt_busy
        int busy = sqlite3_stmt_busy(stmt);
        (void)busy; // Use result to avoid unused variable warning
        
        // 2. Test sqlite3_stmt_scanstatus (if compiled with SQLITE_ENABLE_STMT_SCANSTATUS)
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        if (Size > 3) {
            int idx = (int)(Data[3] % 10) - 1; // Range -1 to 8
            int iScanStatusOp = Data[4] % 10; // Various scan status ops
            int out_int;
            double out_double;
            sqlite3_int64 out_int64;
            const char *out_str;
            
            // Try different output types
            switch (Data[5] % 4) {
                case 0:
                    sqlite3_stmt_scanstatus(stmt, idx, iScanStatusOp, &out_int);
                    break;
                case 1:
                    sqlite3_stmt_scanstatus(stmt, idx, iScanStatusOp, &out_double);
                    break;
                case 2:
                    sqlite3_stmt_scanstatus(stmt, idx, iScanStatusOp, &out_int64);
                    break;
                case 3:
                    sqlite3_stmt_scanstatus(stmt, idx, iScanStatusOp, &out_str);
                    break;
            }
        }
        #endif
        
        // 3. Test sqlite3_stmt_status
        if (Size > 6) {
            int op = Data[6] % 10; // Various status operations
            int resetFlg = Data[7] & 1; // 0 or 1
            int status = sqlite3_stmt_status(stmt, op, resetFlg);
            (void)status;
        }
        
        // 4. Test sqlite3_stmt_explain
        if (Size > 8) {
            int eMode = Data[8] % 3; // 0, 1, or 2
            sqlite3_stmt_explain(stmt, eMode);
        }
        
        // 5. Test sqlite3_stmt_scanstatus_v2 (if compiled with SQLITE_ENABLE_STMT_SCANSTATUS)
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        if (Size > 9) {
            int idx = (int)(Data[9] % 10) - 1;
            int iScanStatusOp = Data[10] % 10;
            int flags = Data[11] & 1 ? 0 : SQLITE_SCANSTAT_COMPLEX;
            int out_int;
            double out_double;
            sqlite3_int64 out_int64;
            const char *out_str;
            
            switch (Data[12] % 4) {
                case 0:
                    sqlite3_stmt_scanstatus_v2(stmt, idx, iScanStatusOp, flags, &out_int);
                    break;
                case 1:
                    sqlite3_stmt_scanstatus_v2(stmt, idx, iScanStatusOp, flags, &out_double);
                    break;
                case 2:
                    sqlite3_stmt_scanstatus_v2(stmt, idx, iScanStatusOp, flags, &out_int64);
                    break;
                case 3:
                    sqlite3_stmt_scanstatus_v2(stmt, idx, iScanStatusOp, flags, &out_str);
                    break;
            }
        }
        #endif
        
        // 6. Test sqlite3_expired
        int expired = sqlite3_expired(stmt);
        (void)expired;
        
        // Clean up statement
        sqlite3_finalize(stmt);
    }
    
    // Test with NULL statement
    if (Size > 0) {
        (void)sqlite3_stmt_busy(NULL);
        (void)sqlite3_expired(NULL);
    }
    
    // Clean up database
    sqlite3_close(db);
    
    return 0;
}
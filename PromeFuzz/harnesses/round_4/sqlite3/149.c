// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
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
    return (count < 3) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Create a simple table and insert some data to have a non-empty database
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a, b);", NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK && zErrMsg) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }
    
    rc = sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'test');", NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK && zErrMsg) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }
    
    // Use the fuzzer input to derive parameters for the API calls
    // We'll use the first few bytes to determine which functions to call and with what parameters
    
    if (Size > 0) {
        // Use the first byte to select which API functions to exercise
        uint8_t selector = Data[0];
        size_t offset = 1;
        
        // 1. sqlite3_db_cacheflush
        if (selector & 0x01) {
            // Begin a transaction to have dirty pages
            sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
            sqlite3_exec(db, "INSERT INTO t1 VALUES(2, 'fuzz');", NULL, NULL, NULL);
            
            rc = sqlite3_db_cacheflush(db);
            // Ignore return value - just testing the API call
            
            sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        }
        
        // 2. sqlite3_setlk_timeout
        if (selector & 0x02 && Size - offset >= 8) {
            int ms = *(int*)(Data + offset);
            offset += 4;
            int flags = *(int*)(Data + offset);
            offset += 4;
            
            rc = sqlite3_setlk_timeout(db, ms, flags);
            // Ignore return value
        }
        
        // 3. sqlite3_busy_handler
        if (selector & 0x04 && Size - offset >= 4) {
            void *arg = (void*)(Data + offset);
            offset += 4;
            
            rc = sqlite3_busy_handler(db, dummy_busy_handler, arg);
            // Ignore return value
        }
        
        // 4. sqlite3_wal_checkpoint_v2
        if (selector & 0x08 && Size - offset >= 12) {
            int pnLog = 0, pnCkpt = 0;
            const char *zDb = NULL;
            
            // Use next byte to determine database name
            uint8_t db_selector = (offset < Size) ? Data[offset] : 0;
            offset++;
            
            if (db_selector & 0x01) {
                zDb = "main";
            } else if (db_selector & 0x02) {
                zDb = "temp";
            }
            // else NULL
            
            int eMode = *(int*)(Data + offset);
            offset += 4;
            
            // Ensure mode is valid
            if (eMode < SQLITE_CHECKPOINT_PASSIVE || eMode > SQLITE_CHECKPOINT_TRUNCATE) {
                eMode = SQLITE_CHECKPOINT_PASSIVE;
            }
            
            rc = sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
            // Ignore return value
        }
        
        // 5. sqlite3_db_status
        if (selector & 0x10 && Size - offset >= 8) {
            int op = *(int*)(Data + offset);
            offset += 4;
            int resetFlg = *(int*)(Data + offset);
            offset += 4;
            int pCur = 0, pHiwtr = 0;
            
            // Ensure op is within reasonable bounds (there are about 20 SQLITE_DBSTATUS options)
            if (op < 0) op = 0;
            if (op > 20) op = 20;
            
            rc = sqlite3_db_status(db, op, &pCur, &pHiwtr, resetFlg);
            // Ignore return value
        }
        
        // 6. sqlite3_busy_timeout
        if (selector & 0x20 && Size - offset >= 4) {
            int ms = *(int*)(Data + offset);
            offset += 4;
            
            rc = sqlite3_busy_timeout(db, ms);
            // Ignore return value
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
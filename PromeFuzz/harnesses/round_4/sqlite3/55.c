// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static unsigned int autovacuum_callback(void *pArg, const char *zSchema,
                                       unsigned int nDbPage,
                                       unsigned int nFreePage,
                                       unsigned int nBytePerPage) {
    return nFreePage;
}

static void autovacuum_destructor(void *pArg) {
    // Do nothing
}

static int commit_hook_callback(void *pArg) {
    return 0;
}

static void rollback_hook_callback(void *pArg) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int cur, hiwtr;
    void *prev_arg;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Use input data to influence behavior
    size_t data_index = 0;
    
    // Test sqlite3_get_autocommit
    int autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;
    
    // Test sqlite3_autovacuum_pages
    if (Size > data_index) {
        // Use first byte to decide whether to set callback or clear it
        if (Data[data_index] & 0x01) {
            rc = sqlite3_autovacuum_pages(db, autovacuum_callback, 
                                         (void*)(uintptr_t)Data[data_index], 
                                         autovacuum_destructor);
        } else {
            rc = sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
        }
        data_index++;
    }
    
    // Test sqlite3_db_status with different opcodes
    if (Size > data_index) {
        // Use data to select opcode (limit to valid range)
        int op = Data[data_index] % 20; // There are less than 20 SQLITE_DBSTATUS options
        rc = sqlite3_db_status(db, op, &cur, &hiwtr, 0);
        data_index++;
        
        // Test with reset flag
        if (Size > data_index) {
            int reset = Data[data_index] & 0x01;
            rc = sqlite3_db_status(db, op, &cur, &hiwtr, reset);
            data_index++;
        }
    }
    
    // Test sqlite3_commit_hook
    if (Size > data_index) {
        prev_arg = sqlite3_commit_hook(db, commit_hook_callback, 
                                      (void*)(uintptr_t)Data[data_index]);
        data_index++;
        
        // Test with NULL callback to disable
        if (Size > data_index && (Data[data_index] & 0x01)) {
            prev_arg = sqlite3_commit_hook(db, NULL, NULL);
        }
        data_index++;
    }
    
    // Test sqlite3_txn_state
    if (Size > data_index) {
        const char *schema = NULL;
        // Use data to select schema
        uint8_t schema_choice = Data[data_index] & 0x03;
        switch (schema_choice) {
            case 0: schema = NULL; break;
            case 1: schema = "main"; break;
            case 2: schema = "temp"; break;
            case 3: schema = "invalid_schema"; break;
        }
        int txn_state = sqlite3_txn_state(db, schema);
        (void)txn_state;
        data_index++;
    }
    
    // Test sqlite3_rollback_hook
    if (Size > data_index) {
        prev_arg = sqlite3_rollback_hook(db, rollback_hook_callback,
                                        (void*)(uintptr_t)Data[data_index]);
        data_index++;
        
        // Test with NULL callback to disable
        if (Size > data_index && (Data[data_index] & 0x01)) {
            prev_arg = sqlite3_rollback_hook(db, NULL, NULL);
        }
        data_index++;
    }
    
    // Execute some SQL to change database state and test functions again
    if (Size > data_index) {
        // Use data to decide what SQL to execute
        uint8_t sql_choice = Data[data_index] & 0x07;
        
        switch (sql_choice) {
            case 0:
                sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
                break;
            case 1:
                sqlite3_exec(db, "CREATE TABLE t1(x, y)", 0, 0, 0);
                break;
            case 2:
                sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2)", 0, 0, 0);
                break;
            case 3:
                sqlite3_exec(db, "COMMIT", 0, 0, 0);
                break;
            case 4:
                sqlite3_exec(db, "ROLLBACK", 0, 0, 0);
                break;
            case 5:
                sqlite3_exec(db, "PRAGMA auto_vacuum = 1", 0, 0, 0);
                break;
            default:
                // Do nothing
                break;
        }
        data_index++;
        
        // Test functions again after state change
        (void)sqlite3_get_autocommit(db);
        
        if (Size > data_index) {
            int op = Data[data_index] % 20;
            rc = sqlite3_db_status(db, op, &cur, &hiwtr, 0);
            data_index++;
        }
        
        if (Size > data_index) {
            const char *schema = NULL;
            uint8_t schema_choice = Data[data_index] & 0x03;
            switch (schema_choice) {
                case 0: schema = NULL; break;
                case 1: schema = "main"; break;
                case 2: schema = "temp"; break;
                case 3: schema = "invalid_schema"; break;
            }
            (void)sqlite3_txn_state(db, schema);
            data_index++;
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
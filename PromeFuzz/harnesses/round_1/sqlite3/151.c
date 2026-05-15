// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_rollback_callback(void *arg) {
    // Do nothing
}

static unsigned int dummy_autovacuum_callback(void *arg, const char *zSchema,
                                              unsigned int nDbPage,
                                              unsigned int nFreePage,
                                              unsigned int nBytePerPage) {
    // Return a value based on the input data
    unsigned int *data = (unsigned int *)arg;
    if (data) {
        return *data % (nFreePage + 1);
    }
    return nFreePage;
}

static unsigned int fixed_autovac_callback(void *arg, const char *zSchema,
                                           unsigned int nDbPage,
                                           unsigned int nFreePage,
                                           unsigned int nBytePerPage) {
    return 0;
}

static void dummy_autovacuum_destructor(void *arg) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    void *prev_arg;
    int txn_state;
    int db_status_current, db_status_highwater;
    int db_config_result;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Create a simple table to have some database structure
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(id INTEGER PRIMARY KEY, name TEXT);", 
                 NULL, NULL, NULL);
    
    // Use the fuzz data to create parameters for the API calls
    // We'll use the first few bytes for various parameters
    
    // 1. Test sqlite3_rollback_hook
    if (Size >= 1) {
        uint8_t callback_type = Data[0] % 3;
        void *user_arg = (void *)(Data + (Size > 8 ? 8 : 0));
        
        if (callback_type == 0) {
            prev_arg = sqlite3_rollback_hook(db, NULL, user_arg);
        } else if (callback_type == 1) {
            prev_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, NULL);
        } else {
            prev_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, user_arg);
        }
        
        // Try to trigger the rollback hook by starting and rolling back a transaction
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    
    // 2. Test sqlite3_autovacuum_pages
    if (Size >= 2) {
        uint8_t autovac_type = Data[1] % 4;
        unsigned int *callback_arg = NULL;
        
        if (Size >= sizeof(unsigned int) + 2) {
            callback_arg = (unsigned int *)(Data + 2);
        }
        
        if (autovac_type == 0) {
            rc = sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
        } else if (autovac_type == 1) {
            rc = sqlite3_autovacuum_pages(db, dummy_autovacuum_callback, 
                                         callback_arg, NULL);
        } else if (autovac_type == 2) {
            rc = sqlite3_autovacuum_pages(db, dummy_autovacuum_callback, 
                                         callback_arg, dummy_autovacuum_destructor);
        } else {
            rc = sqlite3_autovacuum_pages(db, fixed_autovac_callback, NULL, NULL);
        }
    }
    
    // 3. Test sqlite3_txn_state
    if (Size >= 3) {
        uint8_t schema_type = Data[2] % 4;
        const char *zSchema = NULL;
        
        if (schema_type == 0) {
            zSchema = NULL;
        } else if (schema_type == 1) {
            zSchema = "main";
        } else if (schema_type == 2) {
            zSchema = "temp";
        } else {
            // Try an invalid schema name
            zSchema = "invalid_schema";
        }
        
        txn_state = sqlite3_txn_state(db, zSchema);
        
        // Test with different transaction states
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        txn_state = sqlite3_txn_state(db, "main");
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    
    // 4. Test sqlite3_db_status
    if (Size >= 4) {
        uint8_t op_type = Data[3] % 20; // There are many op codes, use modulo
        uint8_t reset_flag = (Size >= 5) ? (Data[4] % 2) : 0;
        
        rc = sqlite3_db_status(db, op_type, &db_status_current, 
                              &db_status_highwater, reset_flag);
        
        // Test a few specific op codes
        sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, 
                         &db_status_current, &db_status_highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, 
                         &db_status_current, &db_status_highwater, 1);
    }
    
    // 5. Test sqlite3_db_config
    if (Size >= 6) {
        uint8_t config_op = Data[5] % 20; // There are many config options
        
        // Try different configurations based on the op code
        if (config_op == SQLITE_DBCONFIG_ENABLE_FKEY) {
            uint8_t enable = (Size >= 7) ? (Data[6] % 2) : 0;
            db_config_result = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 
                                                enable, &rc);
        } else if (config_op == SQLITE_DBCONFIG_ENABLE_TRIGGER) {
            uint8_t enable = (Size >= 7) ? (Data[6] % 2) : 0;
            db_config_result = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, 
                                                enable, &rc);
        } else {
            // Try the config op with default arguments
            db_config_result = sqlite3_db_config(db, config_op, 0, &rc);
        }
    }
    
    // 6. Test sqlite3_close
    // First create a statement to test SQLITE_BUSY condition
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT * FROM t1";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        // Don't finalize to test SQLITE_BUSY
        // sqlite3_finalize(stmt); // Commented to test busy condition
    }
    
    // Try to close (may return SQLITE_BUSY)
    rc = sqlite3_close(db);
    
    // If busy, finalize statement and try again
    if (rc == SQLITE_BUSY && stmt != NULL) {
        sqlite3_finalize(stmt);
        rc = sqlite3_close(db);
    }
    
    // If we closed successfully, reopen for more tests
    if (rc == SQLITE_OK) {
        rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK && db != NULL) {
            // Test closing a NULL pointer (should be harmless)
            sqlite3_close(NULL);
            
            // Test closing again
            sqlite3_close(db);
        }
    }
    
    return 0;
}
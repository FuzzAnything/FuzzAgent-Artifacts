// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_rollback_callback(void *arg) {
    (void)arg;
}

static int dummy_commit_callback(void *arg) {
    (void)arg;
    return 0;
}

static void dummy_unlock_notify_callback(void **apArg, int nArg) {
    (void)apArg;
    (void)nArg;
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table to have some database structure
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, NULL);
    
    // Extract parameters from fuzzer input
    const char *db_name = NULL;
    const char *schema_name = NULL;
    
    if (Size > 0) {
        // Use first byte to decide which database name to use
        uint8_t db_selector = Data[0];
        if (db_selector % 3 == 0) {
            db_name = "main";
        } else if (db_selector % 3 == 1) {
            db_name = "temp";
        } else {
            db_name = NULL;  // Test with NULL
        }
        
        // Use second byte to decide which schema name to use
        if (Size > 1) {
            uint8_t schema_selector = Data[1];
            if (schema_selector % 3 == 0) {
                schema_name = "main";
            } else if (schema_selector % 3 == 1) {
                schema_name = "temp";
            } else {
                schema_name = NULL;  // Test with NULL
            }
        }
    }
    
    // Test sqlite3_rollback_hook
    void *prev_rollback_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, (void*)Data);
    (void)prev_rollback_arg;
    
    // Test sqlite3_db_readonly
    int readonly = sqlite3_db_readonly(db, db_name);
    (void)readonly;
    
    // Test sqlite3_txn_state
    int txn_state = sqlite3_txn_state(db, schema_name);
    (void)txn_state;
    
    // Test sqlite3_get_autocommit
    int autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;
    
    // Test sqlite3_unlock_notify
    // Note: This function is only available if SQLITE_ENABLE_UNLOCK_NOTIFY is defined
    // We'll call it but the callback may not be invoked
    int unlock_notify_rc = sqlite3_unlock_notify(db, dummy_unlock_notify_callback, (void*)Data);
    (void)unlock_notify_rc;
    
    // Test sqlite3_commit_hook
    void *prev_commit_arg = sqlite3_commit_hook(db, dummy_commit_callback, (void*)Data);
    (void)prev_commit_arg;
    
    // Try to exercise different states by executing some SQL
    if (Size > 2) {
        uint8_t action = Data[2] % 4;
        switch (action) {
            case 0:
                sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
                break;
            case 1:
                sqlite3_exec(db, "INSERT INTO t1(data) VALUES('test');", NULL, NULL, NULL);
                break;
            case 2:
                sqlite3_exec(db, "SELECT * FROM t1;", NULL, NULL, NULL);
                break;
            case 3:
                sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
                break;
        }
    }
    
    // Call the functions again after state changes
    readonly = sqlite3_db_readonly(db, db_name);
    txn_state = sqlite3_txn_state(db, schema_name);
    autocommit = sqlite3_get_autocommit(db);
    
    // Test with NULL callbacks to disable hooks
    sqlite3_rollback_hook(db, NULL, NULL);
    sqlite3_commit_hook(db, NULL, NULL);
    sqlite3_unlock_notify(db, NULL, NULL);
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
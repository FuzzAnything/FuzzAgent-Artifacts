// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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

static int commit_hook_callback(void *arg) {
    // Return value based on arg to test commit-to-rollback conversion
    return arg ? 1 : 0;
}

static void rollback_hook_callback(void *arg) {
    // Empty callback for rollback hook
    (void)arg;
}

static void unlock_notify_callback(void **apArg, int nArg) {
    // Empty callback for unlock notify
    (void)apArg;
    (void)nArg;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Use the fuzz input to determine various parameters
    int use_commit_hook = (Size > 0) ? (Data[0] & 1) : 0;
    int commit_hook_returns_nonzero = (Size > 0) ? ((Data[0] >> 1) & 1) : 0;
    int use_rollback_hook = (Size > 0) ? ((Data[0] >> 2) & 1) : 0;
    int db_status_op = (Size > 1) ? (Data[1] % 20) : 0; // There are ~20 SQLITE_DBSTATUS options
    int db_status_reset = (Size > 1) ? ((Data[1] >> 7) & 1) : 0;
    int use_unlock_notify = (Size > 2) ? (Data[2] & 1) : 0;
    int txn_state_schema_idx = (Size > 2) ? ((Data[2] >> 1) & 3) : 0;
    
    // Test sqlite3_commit_hook
    void *prev_commit_arg = sqlite3_commit_hook(db, 
        use_commit_hook ? commit_hook_callback : NULL,
        commit_hook_returns_nonzero ? (void*)1 : NULL);
    (void)prev_commit_arg;
    
    // Test sqlite3_rollback_hook
    void *prev_rollback_arg = sqlite3_rollback_hook(db,
        use_rollback_hook ? rollback_hook_callback : NULL,
        (void*)(uintptr_t)use_rollback_hook);
    (void)prev_rollback_arg;
    
    // Test sqlite3_get_autocommit
    int autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;
    
    // Test sqlite3_db_status
    int current = 0;
    int highwater = 0;
    rc = sqlite3_db_status(db, db_status_op, &current, &highwater, db_status_reset);
    (void)rc;
    
    // Test sqlite3_unlock_notify
    // Note: This requires SQLITE_ENABLE_UNLOCK_NOTIFY to be defined at compile time
    // We'll call it but it may not do anything if not compiled with that flag
    rc = sqlite3_unlock_notify(db,
        use_unlock_notify ? unlock_notify_callback : NULL,
        (void*)(uintptr_t)use_unlock_notify);
    (void)rc;
    
    // Test sqlite3_txn_state with different schema names
    const char *schemas[] = {NULL, "main", "temp", "invalid_schema"};
    const char *schema = schemas[txn_state_schema_idx];
    int txn_state = sqlite3_txn_state(db, schema);
    (void)txn_state;
    
    // Create a simple table and perform some operations to trigger hooks
    char *errmsg = NULL;
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(id INTEGER PRIMARY KEY, data TEXT)", NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // Begin a transaction to change autocommit state
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // Insert some data
    sqlite3_exec(db, "INSERT INTO t1(data) VALUES('test')", NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // Check autocommit state again (should be 0 now)
    autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;
    
    // Check txn_state again
    txn_state = sqlite3_txn_state(db, "main");
    (void)txn_state;
    
    // Check db_status again
    rc = sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &current, &highwater, 0);
    (void)rc;
    
    // Commit or rollback based on input
    int should_commit = (Size > 3) ? (Data[3] & 1) : 1;
    if (should_commit) {
        sqlite3_exec(db, "COMMIT", NULL, NULL, &errmsg);
    } else {
        sqlite3_exec(db, "ROLLBACK", NULL, NULL, &errmsg);
    }
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // Test hooks again after transaction
    prev_commit_arg = sqlite3_commit_hook(db, NULL, NULL);
    prev_rollback_arg = sqlite3_rollback_hook(db, NULL, NULL);
    
    // Test unlock_notify with NULL callback to cancel
    rc = sqlite3_unlock_notify(db, NULL, NULL);
    (void)rc;
    
    // Close the database
    sqlite3_close(db);
    
    return 0;
}
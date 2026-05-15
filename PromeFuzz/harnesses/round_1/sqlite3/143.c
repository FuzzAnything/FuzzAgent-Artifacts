// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

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

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Use first byte to determine which database name to use
    uint8_t db_name_selector = Data[0];
    const char *db_name = NULL;
    
    switch (db_name_selector % 4) {
        case 0: db_name = "main"; break;
        case 1: db_name = "temp"; break;
        case 2: db_name = NULL; break;
        case 3: {
            // Try to use some data as database name (null-terminated)
            if (Size > 1) {
                size_t name_len = (Size - 1) < 100 ? (Size - 1) : 100;
                char *temp_name = malloc(name_len + 1);
                if (temp_name) {
                    memcpy(temp_name, Data + 1, name_len);
                    temp_name[name_len] = '\0';
                    db_name = temp_name;
                }
            }
            break;
        }
    }
    
    // Test sqlite3_rollback_hook
    void *prev_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, (void*)Data);
    (void)prev_arg;
    
    // Test sqlite3_db_readonly
    int readonly = sqlite3_db_readonly(db, db_name);
    (void)readonly;
    
    // Test sqlite3_txn_state
    int txn_state = sqlite3_txn_state(db, db_name);
    (void)txn_state;
    
    // Test sqlite3_get_autocommit
    int autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;
    
    // Test sqlite3_unlock_notify (if compiled with SQLITE_ENABLE_UNLOCK_NOTIFY)
#ifdef SQLITE_ENABLE_UNLOCK_NOTIFY
    rc = sqlite3_unlock_notify(db, dummy_unlock_notify_callback, (void*)Data);
    (void)rc;
#endif
    
    // Test sqlite3_commit_hook
    prev_arg = sqlite3_commit_hook(db, dummy_commit_callback, (void*)Data);
    (void)prev_arg;
    
    // Create a simple table and do some operations to change state
    char *err_msg = NULL;
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(id INTEGER PRIMARY KEY, data TEXT)", NULL, NULL, &err_msg);
    if (err_msg) sqlite3_free(err_msg);
    
    // Use some data to decide whether to start a transaction
    if (Size > 2 && (Data[1] % 2)) {
        sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);
        
        // Maybe insert some data
        if (Size > 10) {
            char insert_sql[256];
            snprintf(insert_sql, sizeof(insert_sql), 
                    "INSERT INTO t1(data) VALUES('fuzz_data_%u')", 
                    (unsigned int)Data[2]);
            sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
        }
        
        // Maybe commit or rollback based on data
        if (Size > 3 && (Data[3] % 2)) {
            sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
        } else {
            sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
        }
    }
    
    // Test functions again with different state
    readonly = sqlite3_db_readonly(db, db_name);
    txn_state = sqlite3_txn_state(db, db_name);
    autocommit = sqlite3_get_autocommit(db);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
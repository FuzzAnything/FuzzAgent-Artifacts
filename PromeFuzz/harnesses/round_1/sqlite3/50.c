// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callback functions for hooks */
static void dummy_rollback_callback(void *arg) {
    (void)arg;
}

static int dummy_wal_callback(void *arg, sqlite3 *db, const char *dbname, int pages) {
    (void)arg;
    (void)db;
    (void)dbname;
    (void)pages;
    return SQLITE_OK;
}

static void dummy_update_callback(void *arg, int op, const char *dbname, 
                                  const char *tablename, sqlite3_int64 rowid) {
    (void)arg;
    (void)op;
    (void)dbname;
    (void)tablename;
    (void)rowid;
}

static int dummy_commit_callback(void *arg) {
    (void)arg;
    return 0;
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    void *prev_arg;
    int txn_state;
    int autocommit;
    
    /* Use first byte to determine behavior */
    uint8_t mode = Size > 0 ? Data[0] : 0;
    
    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use data to create dummy user argument */
    void *user_arg = Size > 0 ? (void*)(intptr_t)Data[0] : NULL;
    
    /* Test sqlite3_rollback_hook */
    prev_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, user_arg);
    /* Call again with NULL to disable */
    sqlite3_rollback_hook(db, NULL, NULL);
    /* Call again with callback */
    sqlite3_rollback_hook(db, dummy_rollback_callback, user_arg);
    
    /* Test sqlite3_wal_hook */
    prev_arg = sqlite3_wal_hook(db, dummy_wal_callback, user_arg);
    /* Call again with NULL to disable */
    sqlite3_wal_hook(db, NULL, NULL);
    /* Call again with callback */
    sqlite3_wal_hook(db, dummy_wal_callback, user_arg);
    
    /* Test sqlite3_txn_state with various schema names */
    txn_state = sqlite3_txn_state(db, NULL); /* Highest transaction state */
    txn_state = sqlite3_txn_state(db, "main");
    txn_state = sqlite3_txn_state(db, "temp");
    
    /* Try schema name from input if available */
    if (Size > 1) {
        /* Create null-terminated string from input */
        char schema[256];
        size_t len = Size - 1 < 255 ? Size - 1 : 255;
        memcpy(schema, Data + 1, len);
        schema[len] = '\0';
        txn_state = sqlite3_txn_state(db, schema);
    }
    
    /* Test sqlite3_update_hook */
    prev_arg = sqlite3_update_hook(db, dummy_update_callback, user_arg);
    /* Call again with NULL to disable */
    sqlite3_update_hook(db, NULL, NULL);
    /* Call again with callback */
    sqlite3_update_hook(db, dummy_update_callback, user_arg);
    
    /* Test sqlite3_get_autocommit */
    autocommit = sqlite3_get_autocommit(db);
    
    /* Test sqlite3_commit_hook */
    prev_arg = sqlite3_commit_hook(db, dummy_commit_callback, user_arg);
    /* Call again with NULL to disable */
    sqlite3_commit_hook(db, NULL, NULL);
    /* Call again with callback */
    sqlite3_commit_hook(db, dummy_commit_callback, user_arg);
    
    /* Try to create a simple table and transaction to trigger hooks */
    if (mode & 0x01) {
        char *errmsg = NULL;
        sqlite3_exec(db, "CREATE TABLE t1(a, b)", NULL, NULL, &errmsg);
        if (errmsg) sqlite3_free(errmsg);
        
        sqlite3_exec(db, "BEGIN", NULL, NULL, &errmsg);
        if (errmsg) sqlite3_free(errmsg);
        
        /* Check autocommit again after BEGIN */
        autocommit = sqlite3_get_autocommit(db);
        
        sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2)", NULL, NULL, &errmsg);
        if (errmsg) sqlite3_free(errmsg);
        
        if (mode & 0x02) {
            sqlite3_exec(db, "COMMIT", NULL, NULL, &errmsg);
        } else {
            sqlite3_exec(db, "ROLLBACK", NULL, NULL, &errmsg);
        }
        if (errmsg) sqlite3_free(errmsg);
        
        /* Check transaction state after commit/rollback */
        txn_state = sqlite3_txn_state(db, "main");
        autocommit = sqlite3_get_autocommit(db);
    }
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}
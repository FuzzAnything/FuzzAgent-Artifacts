// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    void *prev_arg;
    int readonly_result;
    int txn_state_result;
    int autocommit_result;
    int unlock_notify_result;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Create a dummy table to have some schema */
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(x);", NULL, NULL, NULL);
    
    /* Use the fuzzer input to generate strings for database/schema names */
    char *db_name = NULL;
    char *schema_name = NULL;
    
    if (Size > 0) {
        /* Use first half of input for db_name, second half for schema_name */
        size_t half = Size / 2;
        if (half > 0) {
            db_name = (char *)malloc(half + 1);
            if (db_name) {
                memcpy(db_name, Data, half);
                db_name[half] = '\0';
            }
        }
        if (Size - half > 0) {
            schema_name = (char *)malloc(Size - half + 1);
            if (schema_name) {
                memcpy(schema_name, Data + half, Size - half);
                schema_name[Size - half] = '\0';
            }
        }
    } else {
        /* Use default names if no input */
        db_name = strdup("main");
        schema_name = strdup("main");
    }
    
    /* Test sqlite3_rollback_hook */
    prev_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, (void *)Data);
    
    /* Test sqlite3_db_readonly */
    if (db_name) {
        readonly_result = sqlite3_db_readonly(db, db_name);
        (void)readonly_result; /* Use result to avoid unused variable warning */
    }
    
    /* Test sqlite3_txn_state */
    if (schema_name) {
        txn_state_result = sqlite3_txn_state(db, schema_name);
        (void)txn_state_result;
    }
    
    /* Test sqlite3_get_autocommit */
    autocommit_result = sqlite3_get_autocommit(db);
    (void)autocommit_result;
    
    /* Test sqlite3_unlock_notify */
    unlock_notify_result = sqlite3_unlock_notify(db, dummy_unlock_notify_callback, (void *)Data);
    (void)unlock_notify_result;
    
    /* Test sqlite3_commit_hook */
    prev_arg = sqlite3_commit_hook(db, dummy_commit_callback, (void *)Data);
    
    /* Clean up */
    if (db_name) free(db_name);
    if (schema_name) free(schema_name);
    
    /* Close database */
    sqlite3_close(db);
    
    return 0;
}
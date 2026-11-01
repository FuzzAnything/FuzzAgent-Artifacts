// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:186766:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Static helper to safely execute SQL and ignore errors for fuzzing purposes */
static void exec_sql_ignore_errors(sqlite3 *db, const char *sql) {
    sqlite3_stmt *stmt;
    if (!db || !sql) return;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

/* Static update hook callback for sqlite3_update_hook */
static void update_hook_callback(void *pArg, int op, const char *db, const char *table, sqlite3_int64 rowid) {
    (void)pArg;
    (void)op;
    (void)db;
    (void)table;
    (void)rowid;
    /* Do nothing to avoid modifying the connection inside the hook */
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Use an in-memory database to avoid file system dependencies and speed up fuzzing */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a simple table to allow INSERT/UPDATE/DELETE operations */
    exec_sql_ignore_errors(db, "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);");

    /* 2. Execute diverse operations to exercise the target APIs */
    
    /* Fuzz SQL generation based on input data to trigger various states */
    if (Size > 0) {
        /* Use input data to construct SQL statements safely */
        /* We limit the size to prevent huge allocations and ensure null termination */
        size_t sql_len = Size < 100 ? Size : 100;
        char *sql_buf = malloc(sql_len + 2);
        if (sql_buf) {
            memcpy(sql_buf, Data, sql_len);
            sql_buf[sql_len] = '\0';
            
            /* Attempt to parse and execute as a statement if it looks like SQL */
            /* We wrap in a transaction to ensure changes happen */
            exec_sql_ignore_errors(db, "BEGIN TRANSACTION;");
            
            /* Try to insert data derived from fuzz input */
            /* We use a simple pattern to avoid syntax errors mostly */
            char insert_sql[256];
            snprintf(insert_sql, sizeof(insert_sql), "INSERT INTO fuzz_test (data) VALUES (X'%s');", 
                     (Size > 0) ? "01" : ""); /* Fallback to simple hex if input is weird */
            
            /* Execute a mix of operations based on input bytes */
            if (Data[0] % 3 == 0) {
                exec_sql_ignore_errors(db, "INSERT INTO fuzz_test (data) VALUES (randomblob(100));");
            } else if (Data[0] % 3 == 1) {
                exec_sql_ignore_errors(db, "UPDATE fuzz_test SET data = randomblob(100) WHERE id = 1;");
            } else {
                exec_sql_ignore_errors(db, "DELETE FROM fuzz_test WHERE id = 1;");
            }
            
            exec_sql_ignore_errors(db, "COMMIT;");
            free(sql_buf);
        }
    }

    /* 3. Invoke Target API Functions */

    /* sqlite3_total_changes: Returns cumulative count of rows modified */
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes; /* Suppress unused variable warning */

    /* sqlite3_last_insert_rowid: Returns ROWID of most recent insert */
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    (void)last_rowid;

    /* sqlite3_changes: Returns rows modified by the last statement */
    int changes = sqlite3_changes(db);
    (void)changes;

    /* sqlite3_changes64: Returns 64-bit count of rows modified by last statement */
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    (void)changes64;

    /* sqlite3_update_hook: Register and unregister a callback */
    void *old_hook_arg = sqlite3_update_hook(db, update_hook_callback, (void *)0x12345);
    (void)old_hook_arg;
    
    /* Trigger the hook by performing an update */
    exec_sql_ignore_errors(db, "UPDATE fuzz_test SET data = 'test' WHERE id = 1;");
    
    /* Disable the hook */
    sqlite3_update_hook(db, NULL, NULL);

    /* 4. Cleanup */
    /* Use sqlite3_close_v2 for safe closing */
    sqlite3_close_v2(db);

    return 0;
}
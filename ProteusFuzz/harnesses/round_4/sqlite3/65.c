// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:186766:26 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:186766:26 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Static callback function for sqlite3_update_hook.
 * This function is invoked when a row is updated, inserted, or deleted.
 */
static void update_hook_callback(void *pArg, int op, const char *db, const char *table, sqlite3_int64 rowid) {
    (void)pArg;
    (void)op;
    (void)db;
    (void)table;
    (void)rowid;
    /* Intentionally empty to avoid side effects during fuzzing */
}

/*
 * Helper function to create a table and perform various operations to trigger state changes.
 * This allows the target APIs to return non-zero or varied values.
 */
static int execute_test_operations(sqlite3 *db, const uint8_t *data, size_t size) {
    char *sql;
    char *err_msg = NULL;
    int rc;
    sqlite3_stmt *stmt = NULL;

    /* Ensure data is null-terminated for string operations if used as SQL */
    if (size == 0) return SQLITE_OK;

    /* Create a table if it doesn't exist to ensure a valid schema for operations */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        /* If we can't create the table, we can't test much, but we continue to test the APIs */
    }

    /* Prepare an INSERT statement to trigger rowid changes */
    const char *insert_sql = "INSERT INTO fuzz_test (data) VALUES (randomblob(100));";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Execute the statement multiple times to increase rowid and change counts */
        int iterations = (size > 0) ? (data[0] % 100) + 1 : 1;
        for (int i = 0; i < iterations; i++) {
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
                /* Stop on error but don't crash */
                break;
            }
        }
        sqlite3_finalize(stmt);
    }

    /* Prepare an UPDATE statement */
    const char *update_sql = "UPDATE fuzz_test SET data = randomblob(50) WHERE id = 1;";
    rc = sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Prepare a DELETE statement */
    const char *delete_sql = "DELETE FROM fuzz_test WHERE id = 1;";
    rc = sqlite3_prepare_v2(db, delete_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Attempt to execute raw SQL from fuzz input if it looks like text (optional exploration) */
    /* We limit this to avoid hanging on large inputs or infinite loops */
    if (size > 10 && size < 1000) {
        char *safe_sql = malloc(size + 1);
        if (safe_sql) {
            memcpy(safe_sql, data, size);
            safe_sql[size] = '\0';
            /* Only attempt if it starts with a known keyword to avoid syntax errors crashing */
            if (strncmp(safe_sql, "INSERT", 6) == 0 || 
                strncmp(safe_sql, "UPDATE", 6) == 0 || 
                strncmp(safe_sql, "DELETE", 6) == 0) {
                sqlite3_exec(db, safe_sql, NULL, NULL, NULL);
            }
            free(safe_sql);
        }
    }

    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    char *err_msg = NULL;

    /* 
     * Step 1: Prepare the environment.
     * Open an in-memory database to avoid file system dependencies and ensure clean state.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* 
     * Step 2: Execute test operations to generate state (rows modified, rowids, etc.)
     * This ensures that the subsequent API calls return meaningful data rather than just 0.
     */
    execute_test_operations(db, Data, Size);

    /*
     * Step 3: Invoke target API functions diversely.
     * We call the functions multiple times and in different orders to explore states.
     */

    /* 1. sqlite3_changes64 */
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    (void)changes64; /* Suppress unused variable warning */

    /* 2. sqlite3_total_changes */
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;

    /* 3. sqlite3_last_insert_rowid */
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    (void)last_rowid;

    /* 4. sqlite3_changes */
    int changes = sqlite3_changes(db);
    (void)changes;

    /* 5. sqlite3_update_hook */
    /* Register a hook */
    void *prev_hook = sqlite3_update_hook(db, update_hook_callback, (void *)0x1234);
    (void)prev_hook;

    /* Perform an operation to trigger the hook */
    const char *trigger_sql = "INSERT INTO fuzz_test (data) VALUES (1);";
    sqlite3_exec(db, trigger_sql, NULL, NULL, NULL);

    /* Unregister the hook */
    prev_hook = sqlite3_update_hook(db, NULL, (void *)0x5678);
    (void)prev_hook;

    /* Re-check values after hook operations */
    changes64 = sqlite3_changes64(db);
    total_changes = sqlite3_total_changes(db);
    last_rowid = sqlite3_last_insert_rowid(db);
    changes = sqlite3_changes(db);

    /* 
     * Step 4: Cleanup.
     * Finalize any open statements and close the database connection.
     */
    if (stmt) {
        sqlite3_finalize(stmt);
    }

    /* Use sqlite3_close_v2 as per requirements */
    rc = sqlite3_close_v2(db);
    if (rc != SQLITE_OK) {
        /* 
         * If SQLITE_BUSY is returned, it means there are active statements.
         * We already finalized stmt, so this is unexpected but handled gracefully.
         */
    }

    return 0;
}
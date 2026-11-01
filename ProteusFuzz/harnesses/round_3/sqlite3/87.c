// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
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

/*
 * Static helper to safely open a database connection.
 * Uses a dummy file path as requested.
 */
static sqlite3 *safe_open_db(void) {
    sqlite3 *db = NULL;
    /* Open in memory or a dummy file to ensure isolation and avoid file system issues */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

/*
 * Static helper to execute a simple SQL statement to trigger state changes.
 * Returns 0 on success, non-zero on failure.
 */
static int execute_statement(sqlite3 *db, const char *sql) {
    if (!db || !sql) return 1;
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        /* Ignore errors for fuzzing purposes, just clean up */
        if (err_msg) sqlite3_free(err_msg);
    }
    return (rc == SQLITE_OK) ? 0 : 1;
}

/*
 * Callback function for sqlite3_update_hook
 */
static void update_callback(void *arg, int op, const char *db_name, const char *table_name, sqlite3_int64 rowid) {
    (void)arg;
    (void)op;
    (void)db_name;
    (void)table_name;
    (void)rowid;
    /* Do nothing to avoid modifying the connection inside the callback */
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = safe_open_db();
    if (!db) {
        return 0;
    }

    /*
     * Step 1: Prepare environment.
     * Create a table to ensure we have a rowid table to work with.
     */
    execute_statement(db, "CREATE TABLE IF NOT EXISTS fuzz_table (id INTEGER PRIMARY KEY, data BLOB);");

    /*
     * Step 2: Diversify program states by performing various operations
     * based on the input data.
     */

    /* Use the first byte to decide operation type */
    int op_type = Data[0] % 10;

    if (op_type == 0) {
        /* Test sqlite3_last_insert_rowid without any insert */
        sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
        (void)rowid;
    } else if (op_type == 1) {
        /* Perform an INSERT then check last_insert_rowid */
        execute_statement(db, "INSERT INTO fuzz_table (data) VALUES (X'00');");
        sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
        (void)rowid;
    } else if (op_type == 2) {
        /* Perform multiple inserts */
        for (size_t i = 1; i < Size && i < 100; i++) {
            char sql[64];
            snprintf(sql, sizeof(sql), "INSERT INTO fuzz_table (data) VALUES (X'%02x');", Data[i]);
            execute_statement(db, sql);
        }
        sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
        (void)rowid;
    } else if (op_type == 3) {
        /* Test sqlite3_changes and sqlite3_changes64 */
        execute_statement(db, "INSERT INTO fuzz_table (data) VALUES (X'00');");
        int changes = sqlite3_changes(db);
        sqlite3_int64 changes64 = sqlite3_changes64(db);
        (void)changes;
        (void)changes64;
    } else if (op_type == 4) {
        /* Test sqlite3_total_changes */
        execute_statement(db, "INSERT INTO fuzz_table (data) VALUES (X'00');");
        execute_statement(db, "UPDATE fuzz_table SET data = X'01' WHERE id = 1;");
        int total = sqlite3_total_changes(db);
        (void)total;
    } else if (op_type == 5) {
        /* Test sqlite3_update_hook registration */
        void *prev_arg = sqlite3_update_hook(db, update_callback, (void *)(intptr_t)0x1234);
        (void)prev_arg;
        execute_statement(db, "INSERT INTO fuzz_table (data) VALUES (X'00');");
        /* Disable hook */
        sqlite3_update_hook(db, NULL, NULL);
    } else if (op_type == 6) {
        /* Test update hook with different user data */
        for (size_t i = 1; i < Size && i < 10; i++) {
            void *arg = (void *)(intptr_t)Data[i];
            sqlite3_update_hook(db, update_callback, arg);
            execute_statement(db, "INSERT INTO fuzz_table (data) VALUES (X'00');");
        }
    } else if (op_type == 7) {
        /* Test DELETE and UPDATE to trigger changes */
        execute_statement(db, "DELETE FROM fuzz_table;");
        int changes = sqlite3_changes(db);
        (void)changes;
    } else if (op_type == 8) {
        /* Test REPLACE (INSERT OR REPLACE) which affects rowid */
        execute_statement(db, "INSERT OR REPLACE INTO fuzz_table (id, data) VALUES (1, X'00');");
        sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
        (void)rowid;
    } else if (op_type == 9) {
        /* Test transaction rollback effect on rowid (rowid persists across rollback) */
        execute_statement(db, "BEGIN;");
        execute_statement(db, "INSERT INTO fuzz_table (data) VALUES (X'00');");
        sqlite3_int64 rowid_before = sqlite3_last_insert_rowid(db);
        execute_statement(db, "ROLLBACK;");
        sqlite3_int64 rowid_after = sqlite3_last_insert_rowid(db);
        /* rowid should persist */
        (void)rowid_before;
        (void)rowid_after;
    }

    /*
     * Step 3: Cleanup.
     * Use sqlite3_close_v2 to safely close the connection.
     */
    int close_rc = sqlite3_close_v2(db);
    (void)close_rc;

    return 0;
}
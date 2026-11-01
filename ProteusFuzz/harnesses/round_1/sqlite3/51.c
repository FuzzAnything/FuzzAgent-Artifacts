// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:187522:17 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
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

/* Static helper to create a database connection from fuzz input */
static sqlite3 *create_connection_from_input(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Ensure we have at least one byte to avoid empty string issues */
    if (Size == 0) {
        return NULL;
    }

    /* Try to open a database file. Use ":memory:" for in-memory DB or a dummy file.
     * We construct a filename from the input if it looks like a path, otherwise use memory.
     * For fuzzing, ":memory:" is safer and faster, but we can also try to interpret
     * the input as a filename if it doesn't contain nulls early on.
     * Here we stick to ":memory:" to ensure consistent behavior and avoid file system issues.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return NULL;
    }

    /* Optionally, execute some SQL to create a table if the input suggests it.
     * This helps trigger more code paths for the target functions.
     */
    if (Size > 0) {
        /* Create a simple table to allow updates/inserts for update_hook testing */
        const char *sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
        sqlite3_exec(db, sql, NULL, NULL, NULL);
        
        /* Insert some data based on input to ensure rows exist for updates */
        char insert_sql[256];
        int len = (Size < 250) ? Size : 250;
        /* Sanitize input for SQL string literal */
        char *safe_data = malloc(len * 2 + 1);
        if (safe_data) {
            int j = 0;
            for (int i = 0; i < len; i++) {
                if (Data[i] == '\'') {
                    safe_data[j++] = '\'';
                    safe_data[j++] = '\'';
                } else if (Data[i] >= 32 && Data[i] < 127) {
                    safe_data[j++] = Data[i];
                } else {
                    safe_data[j++] = '?';
                }
            }
            safe_data[j] = '\0';
            
            snprintf(insert_sql, sizeof(insert_sql), "INSERT OR IGNORE INTO test_table (data) VALUES ('%s');", safe_data);
            sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
            free(safe_data);
        }
    }

    return db;
}

/* Update hook callback for sqlite3_update_hook testing */
static void update_hook_callback(void *arg, int op, const char *db_name, const char *table_name, sqlite3_int64 rowid) {
    /* Do nothing, just a placeholder to satisfy the API */
    (void)arg;
    (void)op;
    (void)db_name;
    (void)table_name;
    (void)rowid;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    db = create_connection_from_input(Data, Size);
    if (!db) {
        return 0;
    }

    /* 2. Conduct required cleanup and handle exceptions */
    /* We will perform cleanup at the end of the function */

    /* 3. Explore more program states by invoking target functions diversely */

    /* Target 1: sqlite3_busy_timeout */
    /* Use input data to determine timeout value */
    int timeout_ms = 0;
    if (Size > 0) {
        timeout_ms = Data[0];
    }
    if (Size > 1) {
        timeout_ms |= ((int)Data[1] << 8);
    }
    /* Clamp to reasonable values to avoid hanging the fuzzer */
    if (timeout_ms > 1000) timeout_ms = 1000;
    if (timeout_ms < 0) timeout_ms = 0;
    
    rc = sqlite3_busy_timeout(db, timeout_ms);
    (void)rc; /* Ignore return for fuzzing, just exercise the path */

    /* Target 2: sqlite3_update_hook */
    /* Register a hook */
    void *prev_hook = sqlite3_update_hook(db, update_hook_callback, (void*)(intptr_t)Size);
    (void)prev_hook;
    
    /* Try to trigger the hook by inserting/updating */
    if (Size > 0) {
        char update_sql[256];
        int val = Data[0] % 100;
        snprintf(update_sql, sizeof(update_sql), "UPDATE test_table SET data = '%d' WHERE id = 1;", val);
        sqlite3_exec(db, update_sql, NULL, NULL, NULL);
        
        /* Insert more data */
        char insert_sql[256];
        snprintf(insert_sql, sizeof(insert_sql), "INSERT INTO test_table (data) VALUES ('%d');", val);
        sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    }

    /* Unregister the hook */
    sqlite3_update_hook(db, NULL, NULL);

    /* Target 3: sqlite3_db_cacheflush */
    /* Start a transaction to make this meaningful */
    sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
    rc = sqlite3_db_cacheflush(db);
    (void)rc;
    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

    /* Target 4: sqlite3_interrupt */
    /* Prepare a statement that might run long or just exists */
    const char *sql = "SELECT * FROM test_table;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Call interrupt on the connection */
        sqlite3_interrupt(db);
        
        /* Try to step the statement */
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Consume rows */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Target 5: sqlite3_db_handle */
    /* Prepare a statement to test this */
    rc = sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        sqlite3 *handle = sqlite3_db_handle(stmt);
        if (handle != db) {
            /* This should not happen, but we check for robustness */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Target 6: sqlite3_close_v2 */
    /* Close the database */
    rc = sqlite3_close_v2(db);
    (void)rc;
    db = NULL;

    /* 2. Cleanup (already done via close_v2, but ensure no leaks if close failed) */
    if (db) {
        sqlite3_close_v2(db);
    }
    if (stmt) {
        sqlite3_finalize(stmt);
    }

    return 0;
}
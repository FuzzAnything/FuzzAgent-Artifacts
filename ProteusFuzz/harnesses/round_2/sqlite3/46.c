// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:186782:26 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:186766:26 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

/*
 * Static helper to open a database connection safely for fuzzing.
 * Uses an in-memory database to avoid file system dependencies and race conditions.
 */
static sqlite3 *fuzz_open_db(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return NULL;
    }
    return db;
}

/*
 * Static helper to close a database connection.
 */
static void fuzz_close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Fuzz driver entry point.
 * Tests sqlite3_changes, sqlite3_errcode, sqlite3_total_changes64,
 * sqlite3_changes64, sqlite3_db_status64, and sqlite3_total_changes.
 */
int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = fuzz_open_db();
    if (!db) {
        return 0;
    }

    /*
     * Prepare a dummy SQL statement to trigger state changes.
     * We use the fuzzer input as part of the SQL string to explore different code paths,
     * but we sanitize it to ensure valid SQL syntax for the test.
     */
    const char *sql_template = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB); "
                               "INSERT INTO test_table (data) VALUES (X'%s'); "
                               "UPDATE test_table SET data = X'%s' WHERE id = 1; "
                               "DELETE FROM test_table WHERE id = 1;";

    /*
     * Create a safe hex string from input data to use in SQL.
     * Limit length to prevent buffer overflow or excessive memory usage.
     */
    size_t max_hex_len = 100;
    if (Size > max_hex_len) {
        Size = max_hex_len;
    }
    
    char hex_data[256];
    hex_data[0] = '\0';
    size_t hex_len = 0;
    
    for (size_t i = 0; i < Size && hex_len < sizeof(hex_data) - 2; i++) {
        hex_len += snprintf(hex_data + hex_len, sizeof(hex_data) - hex_len, "%02x", Data[i]);
    }

    /*
     * Execute the SQL to generate changes.
     * We ignore the return code of prepare/step as we are fuzzing the status functions,
     * but we check for NULL to avoid crashes.
     */
    sqlite3_stmt *stmt = NULL;
    const char *full_sql = malloc(strlen(sql_template) + 2 * hex_len + 1);
    if (full_sql) {
        snprintf((char *)full_sql, strlen(sql_template) + 2 * hex_len + 1, sql_template, hex_data, hex_data);
        int rc = sqlite3_prepare_v2(db, full_sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            while (sqlite3_step(stmt) == SQLITE_ROW);
        }
        sqlite3_finalize(stmt);
        free((void *)full_sql);
    }

    /*
     * Call the target API functions.
     * These calls are safe even if no changes were made or if the DB is in an error state.
     */
    
    /* 1. sqlite3_changes */
    int changes = sqlite3_changes(db);
    (void)changes;

    /* 2. sqlite3_errcode */
    int errcode = sqlite3_errcode(db);
    (void)errcode;

    /* 3. sqlite3_total_changes64 */
    sqlite3_int64 total_changes64 = sqlite3_total_changes64(db);
    (void)total_changes64;

    /* 4. sqlite3_changes64 */
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    (void)changes64;

    /* 5. sqlite3_db_status64 */
    /* Test with a valid status code and output pointers */
    sqlite3_int64 cur_val = 0, high_water = 0;
    int db_status_rc = sqlite3_db_status64(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &cur_val, &high_water, 0);
    (void)db_status_rc;

    /* Test with reset flag */
    db_status_rc = sqlite3_db_status64(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &cur_val, &high_water, 1);
    (void)db_status_rc;

    /* 6. sqlite3_total_changes */
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;

    /*
     * Cleanup
     */
    fuzz_close_db(db);

    return 0;
}
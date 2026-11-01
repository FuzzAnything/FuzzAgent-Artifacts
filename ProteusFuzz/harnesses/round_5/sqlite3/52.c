// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:93531:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int fuzz_sqlite3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to ensure we have a schema
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Use the fuzzer input as SQL if it's non-empty, otherwise use a safe default
    const char *sql;
    char *sql_buf = NULL;

    if (Size > 0 && Size < 10000) {
        // Create a null-terminated string from the input
        sql_buf = (char *)malloc(Size + 1);
        if (!sql_buf) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql_buf, Data, Size);
        sql_buf[Size] = '\0';
        sql = sql_buf;
    } else {
        // Fallback to a safe query if input is too large or empty
        sql = "SELECT 1;";
    }

    // Prepare the statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // If preparation failed, we cannot test statement-specific functions safely with NULL
    // because some implementations might crash on NULL. We skip the NULL tests for safety.
    if (rc != SQLITE_OK) {
        // Free the allocated buffer if we allocated one
        if (sql_buf) {
            free(sql_buf);
        }
        sqlite3_close(db);
        return 0;
    }

    // Free the allocated buffer if we allocated one
    if (sql_buf) {
        free(sql_buf);
    }

    // Get column count
    int col_count = sqlite3_column_count(stmt);

    // Step the statement to potentially get data
    rc = sqlite3_step(stmt);

    // Test sqlite3_stmt_busy
    int is_busy = sqlite3_stmt_busy(stmt);
    (void)is_busy;

    // Test sqlite3_expired
    int is_expired = sqlite3_expired(stmt);
    (void)is_expired;

    // Iterate through columns to test column functions
    for (int i = 0; i < col_count; i++) {
        // Test sqlite3_column_name16
        const void *name = sqlite3_column_name16(stmt, i);
        (void)name;

        // Test sqlite3_column_bytes16
        int bytes16 = sqlite3_column_bytes16(stmt, i);
        (void)bytes16;
    }

    // Test sqlite3_data_count
    int data_count = sqlite3_data_count(stmt);
    (void)data_count;

    // Reset the statement
    sqlite3_reset(stmt);

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    return fuzz_sqlite3(Data, Size);
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:93531:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:93531:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    if (Size < 2) {
        return 0;
    }

    // Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a table to ensure we have columns to query
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, value BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Use the fuzzer input as a SQL query
    // We limit the size to avoid extremely large queries that might cause OOM
    size_t query_len = Size;
    if (query_len > 10000) {
        query_len = 10000;
    }

    const char *sql = (const char *)Data;
    rc = sqlite3_prepare_v2(db, sql, query_len, &stmt, NULL);

    if (rc == SQLITE_OK) {
        // Execute the statement
        rc = sqlite3_step(stmt);

        // Test sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        (void)col_count;

        // Test sqlite3_data_count
        int data_count = sqlite3_data_count(stmt);
        (void)data_count;

        // Test sqlite3_expired
        int expired = sqlite3_expired(stmt);
        (void)expired;

        // If we have a row, test column accessors
        if (rc == SQLITE_ROW) {
            int num_cols = sqlite3_column_count(stmt);
            for (int i = 0; i < num_cols; i++) {
                // Test sqlite3_column_bytes16
                int bytes16 = sqlite3_column_bytes16(stmt, i);
                (void)bytes16;

                // Test sqlite3_column_name16
                const void *name16 = sqlite3_column_name16(stmt, i);
                (void)name16;

                // Test sqlite3_column_text16
                const void *text16 = sqlite3_column_text16(stmt, i);
                (void)text16;
            }
        }

        // Finalize the statement
        sqlite3_finalize(stmt);
    } else {
        // Test functions with NULL stmt if preparation failed
        int col_count_null = sqlite3_column_count(NULL);
        (void)col_count_null;

        int data_count_null = sqlite3_data_count(NULL);
        (void)data_count_null;

        int expired_null = sqlite3_expired(NULL);
        (void)expired_null;
    }

    // Cleanup
    sqlite3_close(db);

    return 0;
}
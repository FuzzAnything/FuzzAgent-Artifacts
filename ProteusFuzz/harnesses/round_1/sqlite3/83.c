// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:93832:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static int run_fuzz_logic(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a table to ensure we have columns to query
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Insert some dummy data
    const char *insert_sql = "INSERT INTO test_table (name, value) VALUES ('fuzz', 3.14);";
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a SELECT statement
    // We use a fixed SQL query for stability, but the fuzzing input will drive
    // the column indices and logic flow.
    const char *select_sql = "SELECT id, name, value FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step through the result set
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        // If no rows, we can still test column_count and column_name16
        // but text/double/type require a valid row.
    }

    // Determine max column count for safe iteration
    int col_count = sqlite3_column_count(stmt);
    if (col_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Use fuzzing input to determine which column to inspect
    int col_idx = 0;
    if (Size > 0) {
        col_idx = Data[0] % (col_count + 2); // +2 to allow out-of-bounds testing
    }

    // Test sqlite3_column_name16
    const void *name16 = sqlite3_column_name16(stmt, col_idx);
    (void)name16; // Suppress unused warning

    // Test sqlite3_column_decltype16
    const void *decltype16 = sqlite3_column_decltype16(stmt, col_idx);
    (void)decltype16;

    // Test sqlite3_column_type
    int col_type = sqlite3_column_type(stmt, col_idx);
    (void)col_type;

    // Test sqlite3_column_text16 (only if we have a row)
    if (rc == SQLITE_ROW) {
        const void *text16 = sqlite3_column_text16(stmt, col_idx);
        (void)text16;

        // Test sqlite3_column_double
        double val = sqlite3_column_double(stmt, col_idx);
        (void)val;
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Basic sanity check to avoid crashing on extremely small inputs if logic changes
    // For this specific logic, we just need at least 1 byte to determine column index.
    
    run_fuzz_logic(Data, Size);
    
    return 0;
}
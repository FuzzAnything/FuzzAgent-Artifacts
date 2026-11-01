// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int timeout_ms = 0;
    int sleep_ms = 0;
    int col_idx = 0;
    int col_val = 0;

    if (Size < 1) {
        return 0;
    }

    // Extract parameters from fuzz input if available
    if (Size > 0) {
        timeout_ms = Data[0];
    }
    if (Size > 1) {
        sleep_ms = Data[1];
    }
    if (Size > 2) {
        col_idx = Data[2] % 10; // Limit column index to a reasonable range
    }

    // 1. sqlite3_open
    // Use ":memory:" to avoid file system dependencies and ensure cleanup
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        // If open fails, we can't proceed with other operations safely
        sqlite3_close(db);
        return 0;
    }

    // 2. sqlite3_busy_timeout
    // Set timeout based on fuzz input. Negative values are handled by SQLite (treated as 0 in newer versions)
    sqlite3_busy_timeout(db, timeout_ms);

    // 3. sqlite3_step
    // We need a prepared statement to step. Create a simple table and insert/select.
    // We use a static SQL string to ensure valid syntax, as fuzzing SQL parsing itself
    // is a different target. The fuzz input drives the execution state.
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, value INTEGER);";
    rc = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    const char *insert_sql = "INSERT INTO test_table (value) VALUES (12345);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // Prepare a SELECT statement to test sqlite3_step and sqlite3_column_int
    const char *select_sql = "SELECT value FROM test_table LIMIT 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        // Execute step
        rc = sqlite3_step(stmt);
        
        // 4. sqlite3_column_int
        // Only retrieve if we have a row
        if (rc == SQLITE_ROW) {
            col_val = sqlite3_column_int(stmt, col_idx);
            (void)col_val; // Use variable to avoid unused warning
        }
        
        // 5. sqlite3_finalize
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // 6. sqlite3_sleep
    // Sleep for a duration derived from fuzz input. 
    // Ensure it's not excessively large to avoid hanging the fuzzer.
    int safe_sleep = sleep_ms % 100; // Cap at 100ms
    sqlite3_sleep(safe_sleep);

    // 7. sqlite3_close
    sqlite3_close(db);

    return 0;
}
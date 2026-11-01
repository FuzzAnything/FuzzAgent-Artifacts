// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:187522:17 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:187522:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;

    // Initialize database connection using an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Ensure the input data is treated as a null-terminated string for SQL
    // We create a buffer to hold the SQL string, ensuring it is null-terminated
    // even if the fuzz input doesn't contain a null byte.
    char *sql_buf = (char *)malloc(Size + 1);
    if (!sql_buf) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';

    // Fuzzing Logic:
    // 1. Prepare a statement.
    // 2. If preparation succeeds, step through results.
    // 3. Call target APIs: sqlite3_column_type, sqlite3_column_int64, sqlite3_reset.
    // 4. Call sqlite3_interrupt to test interruption logic.
    // 5. Call sqlite3_db_handle to retrieve the connection.
    // 6. Cleanup.

    // Try to prepare the statement. We limit the length to avoid huge allocations if input is massive
    // but allow the full input size for SQL parsing.
    rc = sqlite3_prepare_v2(db, sql_buf, (int)Size, &stmt, &tail);

    if (rc == SQLITE_OK && stmt) {
        // Execute the statement
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Get column count to avoid out-of-bounds access
            int col_count = sqlite3_column_count(stmt);
            if (col_count <= 0) break;

            // Fuzz column index: use modulo to stay within bounds, or test invalid indices
            int idx = (Size % (col_count + 2)) - 1; // Range: -1 to col_count

            // Target API 1: sqlite3_column_type
            int col_type = sqlite3_column_type(stmt, idx);
            (void)col_type; // Suppress unused warning

            // Target API 2: sqlite3_column_int64
            sqlite3_int64 col_val = sqlite3_column_int64(stmt, idx);
            (void)col_val;

            // Target API 3: sqlite3_interrupt
            // Calling interrupt while stepping might cause SQLITE_INTERRUPT on next step
            sqlite3_interrupt(db);

            // Target API 4: sqlite3_reset
            // Resetting inside the loop is unusual but valid to test state transitions
            int reset_rc = sqlite3_reset(stmt);
            (void)reset_rc;
            
            // Break after one row to avoid infinite loops if logic is weird, 
            // or continue if we want to test multiple rows with interrupt flags set.
            // Here we break to ensure we test the reset logic effectively.
            break;
        }
        
        // Additional test: Reset again after loop finishes
        if (sqlite3_reset(stmt) != SQLITE_OK) {
            // Error handling logic (ignored for fuzzing coverage)
        }

        // Target API 5: sqlite3_db_handle
        sqlite3 *handle = sqlite3_db_handle(stmt);
        if (handle != db) {
            // Should be equal, but fuzzing checks for unexpected behavior
        }
    } else {
        // If preparation fails, we still want to test sqlite3_interrupt on the connection
        sqlite3_interrupt(db);
    }

    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    free(sql_buf);
    sqlite3_close(db);

    return 0;
}
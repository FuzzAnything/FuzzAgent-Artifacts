// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a table to ensure valid schema for the test
    const char *createSQL = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, value TEXT);";
    char *err_msg = NULL;
    rc = sqlite3_exec(db, createSQL, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement with parameters
    const char *sql = "INSERT INTO test_table (id, value) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind parameters based on input data if available
    if (Size > 0) {
        int id_val = (int)Data[0];
        // Use the rest of the data as the text value, ensuring null termination is handled by length
        int text_len = (Size > 1) ? (int)(Size - 1) : 0;
        const char *text_val = (Size > 1) ? (const char *)(Data + 1) : "";
        
        sqlite3_bind_int(stmt, 1, id_val);
        sqlite3_bind_text(stmt, 2, text_val, text_len, SQLITE_TRANSIENT);
    }

    // 1. sqlite3_reset
    // Resetting before step is safe (returns SQLITE_OK)
    sqlite3_reset(stmt);

    // 2. sqlite3_bind_text
    // Re-binding or ensuring bindings are set (already done above, but logic flow follows request)
    // If we need to re-bind to explore state, we could do it here, but initial bind is sufficient for step.
    
    // 3. sqlite3_step
    rc = sqlite3_step(stmt);

    // 4. sqlite3_errmsg
    const char *err = sqlite3_errmsg(db);
    (void)err; // Use the error message to suppress warnings

    // 5. sqlite3_reset
    sqlite3_reset(stmt);

    // 6. sqlite3_changes
    int changes = sqlite3_changes(db);
    (void)changes;

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
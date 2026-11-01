// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT); INSERT INTO test (data) VALUES (?);";
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare the SQL statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step 1: sqlite3_reset
    // Resetting before any step is valid and returns SQLITE_OK
    rc = sqlite3_reset(stmt);

    // Step 2: sqlite3_bind_text
    // Bind fuzzed data to the first parameter (index 1)
    // Use SQLITE_TRANSIENT to let SQLite copy the data
    if (Size > 0) {
        rc = sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);
    }

    // Step 3: sqlite3_step
    // Execute the statement
    rc = sqlite3_step(stmt);

    // Step 4: sqlite3_errmsg
    // Retrieve error message if an error occurred
    if (rc != SQLITE_OK && rc != SQLITE_ROW && rc != SQLITE_DONE) {
        const char *errmsg = sqlite3_errmsg(db);
        // Use errmsg to prevent compiler warnings about unused variable
        if (errmsg) {
            volatile const char *unused = errmsg;
            (void)unused;
        }
    }

    // Step 5: sqlite3_reset
    // Reset the statement for potential reuse or cleanup
    rc = sqlite3_reset(stmt);

    // Step 6: sqlite3_changes
    // Get the number of rows modified by the last INSERT/UPDATE/DELETE
    int changes = sqlite3_changes(db);
    (void)changes;

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
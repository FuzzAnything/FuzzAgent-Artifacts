// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
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
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int create_in_memory_db(sqlite3 **ppDb) {
    int rc = sqlite3_open(":memory:", ppDb);
    if (rc != SQLITE_OK) {
        sqlite3_close(*ppDb);
        *ppDb = NULL;
        return 0;
    }
    
    // Create a simple table to ensure we have something to work with
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(*ppDb, create_sql, NULL, NULL, NULL);
    
    return 1;
}

static int prepare_statement(sqlite3 *db, sqlite3_stmt **ppStmt, const char *sql) {
    int rc = sqlite3_prepare_v2(db, sql, -1, ppStmt, NULL);
    if (rc != SQLITE_OK) {
        *ppStmt = NULL;
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // 1. Prepare the environment
    if (!create_in_memory_db(&db)) {
        return 0;
    }
    
    // 2. Prepare a statement that can accept parameters
    // Use a simple INSERT statement that can be parameterized
    const char *sql = "INSERT INTO fuzz_test (data) VALUES (?);";
    if (!prepare_statement(db, &stmt, sql)) {
        sqlite3_close(db);
        return 0;
    }
    
    // 3. Execute the target function sequence
    // Sequence: sqlite3_reset -> sqlite3_bind_text -> sqlite3_step -> sqlite3_errmsg -> sqlite3_reset -> sqlite3_changes
    
    // First reset (safe even if never stepped)
    sqlite3_reset(stmt);
    
    // Bind text from fuzz input if available
    if (Size > 0) {
        // Use the fuzz data as text to bind
        // Limit the size to avoid potential issues, but allow large inputs for fuzzing
        int bind_len = (Size < 10000) ? (int)Size : 10000;
        sqlite3_bind_text(stmt, 1, (const char*)Data, bind_len, SQLITE_TRANSIENT);
    } else {
        // Bind an empty string if no data
        sqlite3_bind_text(stmt, 1, "", 0, SQLITE_TRANSIENT);
    }
    
    // Step the statement
    sqlite3_step(stmt);
    
    // Get error message (safe to call even if no error)
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg; // Suppress unused warning
    
    // Second reset
    sqlite3_reset(stmt);
    
    // Get changes count
    int changes = sqlite3_changes(db);
    (void)changes; // Suppress unused warning
    
    // 4. Cleanup
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    
    if (db != NULL) {
        sqlite3_close(db);
    }
    
    return 0;
}
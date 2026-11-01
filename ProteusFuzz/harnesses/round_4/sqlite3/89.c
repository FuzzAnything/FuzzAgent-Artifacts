// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int create_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return 0;
    }
    // Create a simple table to allow binding and execution
    const char *sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(*db, sql, NULL, NULL, NULL);
    return 1;
}

static int prepare_statements(sqlite3 *db, sqlite3_stmt **stmt1, sqlite3_stmt **stmt2) {
    const char *sql1 = "INSERT INTO test_table (data) VALUES (?)";
    const char *sql2 = "INSERT INTO test_table (data) VALUES (?)";
    
    int rc1 = sqlite3_prepare_v2(db, sql1, -1, stmt1, NULL);
    if (rc1 != SQLITE_OK) return 0;
    
    int rc2 = sqlite3_prepare_v2(db, sql2, -1, stmt2, NULL);
    if (rc2 != SQLITE_OK) {
        sqlite3_finalize(*stmt1);
        *stmt1 = NULL;
        return 0;
    }
    
    return 1;
}

static void cleanup(sqlite3 *db, sqlite3_stmt *stmt1, sqlite3_stmt *stmt2) {
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (db) sqlite3_close(db);
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;

    if (!create_db(&db)) return 0;
    if (!prepare_statements(db, &stmt1, &stmt2)) {
        cleanup(db, NULL, NULL);
        return 0;
    }

    // Parse input data to generate diverse test cases
    double val_double = 0.0;
    int val_int = 0;
    int val_len = 0;
    int idx = 1;

    if (Size >= 8) {
        // Interpret first 8 bytes as double
        memcpy(&val_double, Data, 8);
        // Ensure it's a valid double (avoid NaN/Inf if desired, but fuzzers like them)
    }
    
    if (Size >= 12) {
        // Next 4 bytes as int for length or index
        memcpy(&val_int, Data + 8, 4);
        val_len = abs(val_int);
        if (val_len > 1000000) val_len = 1000000; // Cap length for safety
    }

    if (Size >= 16) {
        // Next 4 bytes as index
        memcpy(&idx, Data + 12, 4);
        // Ensure index is positive and reasonable
        if (idx <= 0) idx = 1;
        if (idx > 10) idx = 1; // Cap index to avoid out-of-bounds on simple statements
    }

    // 1. Test sqlite3_bind_double
    sqlite3_bind_double(stmt1, idx, val_double);
    
    // 2. Test sqlite3_bind_zeroblob
    sqlite3_bind_zeroblob(stmt1, idx, val_len);

    // 3. Test sqlite3_clear_bindings
    sqlite3_clear_bindings(stmt1);

    // 4. Test sqlite3_reset
    sqlite3_reset(stmt1);

    // 5. Test sqlite3_stmt_readonly
    int is_readonly = sqlite3_stmt_readonly(stmt1);
    (void)is_readonly; // Suppress unused warning

    // 6. Test sqlite3_transfer_bindings
    // Re-bind to stmt1 first to have something to transfer
    sqlite3_bind_double(stmt1, 1, val_double);
    sqlite3_transfer_bindings(stmt1, stmt2);

    // Cleanup
    cleanup(db, stmt1, stmt2);

    return 0;
}
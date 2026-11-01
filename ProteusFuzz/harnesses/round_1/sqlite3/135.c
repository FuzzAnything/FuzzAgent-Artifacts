// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:94152:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
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

static int setup_db_and_stmt(sqlite3 **db_out, sqlite3_stmt **stmt_out, const uint8_t *data, size_t size) {
    int rc;
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return -1;
    }

    // Create a simple table to allow binding
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB); INSERT INTO test VALUES (1, ?);";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }

    *db_out = db;
    *stmt_out = stmt;
    return 0;
}

static void cleanup_db_and_stmt(sqlite3 *db, sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;

    // Setup
    if (setup_db_and_stmt(&db, &stmt1, Data, Size) != 0) {
        return 0;
    }

    // Prepare a second statement for transfer_bindings
    const char *sql2 = "INSERT INTO test VALUES (2, ?);";
    rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db_and_stmt(db, stmt1);
        return 0;
    }

    // Extract fuzzing parameters
    int param_idx = (Data[0] % 10) + 1; // 1 to 10
    int small_len = Data[1] % 1000;
    
    // Use a portion of the input as blob data if available
    const void *blob_data = (Size > 10) ? (const void *)(Data + 2) : NULL;
    size_t blob_len = (Size > 10) ? (Size - 2) : 0;
    
    // Construct a large length for 64-bit tests
    sqlite3_uint64 large_len = ((sqlite3_uint64)Data[0] << 56) | ((sqlite3_uint64)Data[1] << 48) | 
                               ((sqlite3_uint64)Data[2] << 40) | ((sqlite3_uint64)Data[3] << 32) |
                               ((sqlite3_uint64)Data[4] << 24) | ((sqlite3_uint64)Data[5] << 16) |
                               ((sqlite3_uint64)Data[6] << 8) | ((sqlite3_uint64)Data[7]);

    // 1. Test sqlite3_bind_blob64
    // We use SQLITE_TRANSIENT to let SQLite copy the data
    rc = sqlite3_bind_blob64(stmt1, param_idx, blob_data, blob_len, SQLITE_TRANSIENT);
    (void)rc; // Ignore return for fuzzing, just trigger code path

    // 2. Test sqlite3_bind_zeroblob64
    // Use a potentially large length, but cap it to avoid immediate OOM in some configs if needed
    // The API handles SQLITE_LIMIT_LENGTH checks internally
    rc = sqlite3_bind_zeroblob64(stmt1, param_idx, large_len);
    (void)rc;

    // 3. Test sqlite3_bind_zeroblob (32-bit version)
    rc = sqlite3_bind_zeroblob(stmt1, param_idx, small_len);
    (void)rc;

    // 4. Test sqlite3_bind_null
    rc = sqlite3_bind_null(stmt1, param_idx);
    (void)rc;

    // 5. Test sqlite3_clear_bindings
    rc = sqlite3_clear_bindings(stmt1);
    (void)rc;

    // Re-bind to enable transfer test
    rc = sqlite3_bind_blob64(stmt1, param_idx, blob_data, blob_len, SQLITE_TRANSIENT);
    (void)rc;

    // 6. Test sqlite3_transfer_bindings
    // This requires both statements to have the same number of parameters (both have 1 here)
    rc = sqlite3_transfer_bindings(stmt1, stmt2);
    (void)rc;

    // Cleanup
    cleanup_db_and_stmt(db, stmt1);
    sqlite3_finalize(stmt2); // stmt2 is not in the main cleanup function scope
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
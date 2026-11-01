// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:94152:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fuzzer_initialize(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    return 1;
}

static void fuzzer_cleanup(sqlite3 *db, sqlite3_stmt *stmt1, sqlite3_stmt *stmt2) {
    if (stmt1) {
        sqlite3_finalize(stmt1);
    }
    if (stmt2) {
        sqlite3_finalize(stmt2);
    }
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;

    if (!fuzzer_initialize(&db)) {
        return 0;
    }

    // Create a simple SQL statement with parameters to bind to
    const char *sql = "CREATE TABLE test (id INTEGER, data BLOB); "
                      "INSERT INTO test VALUES (?, ?); "
                      "SELECT * FROM test WHERE id = ?;";
    
    // Prepare statement 1
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt1, NULL);
    if (rc != SQLITE_OK) {
        fuzzer_cleanup(db, stmt1, stmt2);
        return 0;
    }

    // Prepare statement 2 (same SQL structure for transfer_bindings)
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt2, NULL);
    if (rc != SQLITE_OK) {
        fuzzer_cleanup(db, stmt1, stmt2);
        return 0;
    }

    // Extract parameters from input data
    int param_idx = (Data[0] % 3) + 1; // 1-based index, max 3 parameters
    int blob_len = (Data[1] % 1000) + 1;
    sqlite3_uint64 large_len = ((sqlite3_uint64)(Data[1]) << 32) | (Data[0]);
    
    // Ensure we have enough data for blob operations if needed
    const void *blob_data = NULL;
    if (Size > 2 && blob_len > 0) {
        blob_data = Data + 2;
        if ((size_t)blob_len > Size - 2) {
            blob_len = Size - 2;
        }
    }

    // Test sqlite3_bind_blob64
    if (blob_data && blob_len > 0) {
        sqlite3_bind_blob64(stmt1, param_idx, blob_data, (sqlite3_uint64)blob_len, SQLITE_STATIC);
    }

    // Test sqlite3_bind_zeroblob
    sqlite3_bind_zeroblob(stmt1, param_idx, blob_len);

    // Test sqlite3_bind_zeroblob64
    sqlite3_bind_zeroblob64(stmt1, param_idx, large_len);

    // Test sqlite3_bind_null
    sqlite3_bind_null(stmt1, param_idx);

    // Test sqlite3_clear_bindings
    sqlite3_clear_bindings(stmt1);

    // Re-bind to prepare for transfer
    if (blob_data && blob_len > 0) {
        sqlite3_bind_blob64(stmt1, param_idx, blob_data, (sqlite3_uint64)blob_len, SQLITE_STATIC);
    }

    // Test sqlite3_transfer_bindings
    // This requires both statements to have the same number of parameters
    sqlite3_transfer_bindings(stmt1, stmt2);

    // Cleanup
    fuzzer_cleanup(db, stmt1, stmt2);

    return 0;
}
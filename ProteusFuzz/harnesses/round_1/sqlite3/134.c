// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:94152:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
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

static int fuzzer_init_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return 0;
    }
    return 1;
}

static void fuzzer_cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

static int fuzzer_prepare_stmt(sqlite3 *db, const char *sql, sqlite3_stmt **stmt) {
    if (!db || !sql || !stmt) return 0;
    int rc = sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        *stmt = NULL;
        return 0;
    }
    return 1;
}

static void fuzzer_finalize_stmt(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    sqlite3 *db = NULL;
    if (!fuzzer_init_db(&db)) return 0;

    // Create a simple table to work with
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    // Prepare two statements for testing transfer_bindings
    const char *insert_sql = "INSERT INTO test (data) VALUES (?1);";
    const char *select_sql = "SELECT data FROM test WHERE id = ?1;";

    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;

    if (!fuzzer_prepare_stmt(db, insert_sql, &stmt1)) goto cleanup;
    if (!fuzzer_prepare_stmt(db, select_sql, &stmt2)) goto cleanup;

    // Use input data to drive fuzzing logic
    int param_idx = (Data[0] % 5) + 1; // 1 to 5
    if (param_idx > sqlite3_bind_parameter_count(stmt1)) param_idx = 1;
    if (param_idx > sqlite3_bind_parameter_count(stmt2)) param_idx = 1;

    // Fuzz sqlite3_bind_blob64
    if (Size > 10) {
        sqlite3_uint64 blob_len = (sqlite3_uint64)Data[1] | ((sqlite3_uint64)Data[2] << 8);
        blob_len %= (Size - 10);
        if (blob_len > 0 && blob_len < Size - 10) {
            const void *blob_data = &Data[10];
            sqlite3_bind_blob64(stmt1, param_idx, blob_data, blob_len, SQLITE_TRANSIENT);
        }
    }

    // Fuzz sqlite3_bind_zeroblob64
    if (Size > 12) {
        sqlite3_uint64 zero_len = (sqlite3_uint64)Data[11] | ((sqlite3_uint64)Data[12] << 8);
        zero_len %= 1000000; // Reasonable limit
        sqlite3_bind_zeroblob64(stmt1, param_idx, zero_len);
    }

    // Fuzz sqlite3_bind_zeroblob
    if (Size > 14) {
        int zero_len32 = (int)(Data[13] | (Data[14] << 8));
        zero_len32 %= 100000;
        sqlite3_bind_zeroblob(stmt1, param_idx, zero_len32);
    }

    // Fuzz sqlite3_bind_null
    sqlite3_bind_null(stmt1, param_idx);

    // Fuzz sqlite3_transfer_bindings
    // Ensure both statements have same parameter count for valid transfer
    if (sqlite3_bind_parameter_count(stmt1) == sqlite3_bind_parameter_count(stmt2)) {
        sqlite3_transfer_bindings(stmt1, stmt2);
        sqlite3_transfer_bindings(stmt2, stmt1);
    } else {
        // Try transfer anyway to test error handling
        sqlite3_transfer_bindings(stmt1, stmt2);
    }

    // Fuzz sqlite3_clear_bindings
    sqlite3_clear_bindings(stmt1);
    sqlite3_clear_bindings(stmt2);

    // Re-bind after clear to test state
    if (Size > 16) {
        int blob_len = (int)(Data[15] | (Data[16] << 8));
        blob_len %= (Size - 16);
        if (blob_len > 0) {
            sqlite3_bind_blob64(stmt1, param_idx, &Data[16], blob_len, SQLITE_TRANSIENT);
        }
    }

cleanup:
    fuzzer_finalize_stmt(stmt1);
    fuzzer_finalize_stmt(stmt2);
    fuzzer_cleanup_db(db);

    return 0;
}
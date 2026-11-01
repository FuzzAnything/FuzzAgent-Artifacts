// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fuzzer_init(const uint8_t *Data, size_t Size, sqlite3 **db, sqlite3_stmt **stmt1, sqlite3_stmt **stmt2) {
    int rc;
    *db = NULL;
    *stmt1 = NULL;
    *stmt2 = NULL;

    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to ensure statements can be prepared
    rc = sqlite3_exec(*db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, value REAL, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return 0;
    }

    // Prepare two statements based on input data or defaults
    const char *sql1 = "INSERT INTO test_table (value, data) VALUES (?, ?);";
    const char *sql2 = "SELECT * FROM test_table WHERE value = ? AND data = ?;";

    if (Size > 0 && Data[0] & 1) {
        // Use dynamic SQL if possible, but keep it safe for fuzzing
        // We'll stick to fixed SQL for safety in this driver to avoid syntax errors crashing immediately
        // but we can vary the logic flow based on input
    }

    rc = sqlite3_prepare_v2(*db, sql1, -1, stmt1, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return 0;
    }

    rc = sqlite3_prepare_v2(*db, sql2, -1, stmt2, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(*stmt1);
        sqlite3_close(*db);
        return 0;
    }

    return 1;
}

static void fuzzer_cleanup(sqlite3 **db, sqlite3_stmt **stmt1, sqlite3_stmt **stmt2) {
    if (*stmt1) {
        sqlite3_finalize(*stmt1);
        *stmt1 = NULL;
    }
    if (*stmt2) {
        sqlite3_finalize(*stmt2);
        *stmt2 = NULL;
    }
    if (*db) {
        sqlite3_close(*db);
        *db = NULL;
    }
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;

    if (!fuzzer_init(Data, Size, &db, &stmt1, &stmt2)) {
        return 0;
    }

    // Helper to safely extract integers from fuzz data
    int idx1 = (Size > 0) ? (Data[0] % 10) + 1 : 1; // 1-based index, max 10
    int idx2 = (Size > 1) ? (Data[1] % 10) + 1 : 1;
    int len = (Size > 2) ? (Data[2] % 1000) : 100;

    // 1. sqlite3_bind_double
    double val_double = 0.0;
    if (Size >= 8) {
        memcpy(&val_double, Data, 8);
    } else {
        val_double = (double)(Size * 1.5);
    }
    sqlite3_bind_double(stmt1, idx1, val_double);

    // 2. sqlite3_bind_zeroblob
    if (len > 0) {
        sqlite3_bind_zeroblob(stmt1, idx2, len);
    }

    // 3. sqlite3_clear_bindings
    sqlite3_clear_bindings(stmt1);

    // Re-bind for transfer test
    sqlite3_bind_double(stmt1, 1, val_double);
    sqlite3_bind_zeroblob(stmt1, 2, len);

    // 4. sqlite3_transfer_bindings
    // This requires compatible parameter counts. Both our statements have 2 params.
    sqlite3_transfer_bindings(stmt1, stmt2);

    // 5. sqlite3_stmt_readonly
    // Check both statements
    int readonly1 = sqlite3_stmt_readonly(stmt1);
    int readonly2 = sqlite3_stmt_readonly(stmt2);
    (void)readonly1; // Suppress unused warning
    (void)readonly2;

    // 6. sqlite3_reset
    // Try to step first to ensure state changes, then reset
    sqlite3_step(stmt2);
    sqlite3_reset(stmt2);

    // Additional exploration: Bind again after reset and clear
    sqlite3_bind_double(stmt2, 1, val_double);
    sqlite3_clear_bindings(stmt2);
    sqlite3_reset(stmt1);

    // Cleanup
    fuzzer_cleanup(&db, &stmt1, &stmt2);

    return 0;
}
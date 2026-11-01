// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:94152:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_test_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return rc;
    }
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(*db, sql, NULL, NULL, NULL);
    return SQLITE_OK;
}

static void cleanup_stmt(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;

    if (create_test_db(&db) != SQLITE_OK) {
        return 0;
    }

    // Prepare two statements with the same parameter count for transfer_bindings
    const char *sql1 = "INSERT INTO test (id, data) VALUES (?, ?);";
    const char *sql2 = "INSERT INTO test (id, data) VALUES (?, ?);";

    rc = sqlite3_prepare_v2(db, sql1, -1, &stmt1, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL);
    if (rc != SQLITE_OK) {
        cleanup_stmt(stmt1);
        cleanup_db(db);
        return 0;
    }

    // Use input data to determine operations
    uint8_t op = Data[0] % 6;
    int param_idx = (Data[1] % 2) + 1; // 1 or 2
    sqlite3_uint64 blob_len = 0;
    if (Size >= 8) {
        blob_len = *(const sqlite3_uint64 *)(Data + 2);
        // Cap length to avoid excessive allocation or limits
        if (blob_len > 1000000000ULL) {
            blob_len = 1000000000ULL;
        }
    }

    // Bind some initial data to stmt1 for transfer test
    sqlite3_bind_int(stmt1, 1, 12345);
    if (Size > 10) {
        sqlite3_bind_blob(stmt1, 2, Data + 10, Size - 10, SQLITE_TRANSIENT);
    }

    switch (op) {
        case 0: {
            // Test sqlite3_transfer_bindings
            // Ensure stmt2 has no bindings first
            sqlite3_clear_bindings(stmt2);
            rc = sqlite3_transfer_bindings(stmt1, stmt2);
            // Execute stmt2 to use the transferred bindings
            sqlite3_reset(stmt2);
            sqlite3_step(stmt2);
            break;
        }
        case 1: {
            // Test sqlite3_bind_zeroblob64
            rc = sqlite3_bind_zeroblob64(stmt1, param_idx, blob_len);
            if (rc == SQLITE_OK) {
                sqlite3_reset(stmt1);
                sqlite3_step(stmt1);
            }
            break;
        }
        case 2: {
            // Test sqlite3_bind_blob64
            const void *blob_data = (Size > 10) ? (const void *)(Data + 10) : NULL;
            int blob_size = (Size > 10) ? (int)(Size - 10) : 0;
            if (blob_size > 1000000) blob_size = 1000000; // Cap for safety
            rc = sqlite3_bind_blob64(stmt1, param_idx, blob_data, blob_size, SQLITE_TRANSIENT);
            if (rc == SQLITE_OK) {
                sqlite3_reset(stmt1);
                sqlite3_step(stmt1);
            }
            break;
        }
        case 3: {
            // Test sqlite3_bind_zeroblob
            int len = (blob_len > 2147483647) ? 2147483647 : (int)blob_len;
            rc = sqlite3_bind_zeroblob(stmt1, param_idx, len);
            if (rc == SQLITE_OK) {
                sqlite3_reset(stmt1);
                sqlite3_step(stmt1);
            }
            break;
        }
        case 4: {
            // Test sqlite3_clear_bindings
            // Bind something first
            sqlite3_bind_int(stmt1, 1, 999);
            sqlite3_bind_text(stmt1, 2, "test", -1, SQLITE_STATIC);
            rc = sqlite3_clear_bindings(stmt1);
            // Try to execute with cleared bindings (should fail or behave as NULL)
            sqlite3_reset(stmt1);
            sqlite3_step(stmt1);
            break;
        }
        case 5: {
            // Test sqlite3_bind_null
            rc = sqlite3_bind_null(stmt1, param_idx);
            if (rc == SQLITE_OK) {
                // Bind other param if needed to execute
                if (param_idx == 1) {
                    sqlite3_bind_text(stmt1, 2, "data", -1, SQLITE_STATIC);
                } else {
                    sqlite3_bind_int(stmt1, 1, 1);
                }
                sqlite3_reset(stmt1);
                sqlite3_step(stmt1);
            }
            break;
        }
    }

    cleanup_stmt(stmt1);
    cleanup_stmt(stmt2);
    cleanup_db(db);

    return 0;
}
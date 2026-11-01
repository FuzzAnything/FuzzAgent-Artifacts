// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_zeroblob64 at sqlite3.c:94152:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
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

static int create_db(sqlite3 **db) {
    return sqlite3_open(":memory:", db);
}

static int prepare_statement(sqlite3 *db, const char *sql, sqlite3_stmt **stmt) {
    int rc = sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        *stmt = NULL;
        return rc;
    }
    return SQLITE_OK;
}

static void cleanup_statement(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

static void dummy_destructor(void *p) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;

    if (Size < 2) {
        return 0;
    }

    rc = create_db(&db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a table to ensure statements are valid
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);

    // Prepare two statements for testing
    const char *sql1 = "INSERT INTO test_table (data) VALUES (?)";
    const char *sql2 = "INSERT INTO test_table (data) VALUES (?)";

    rc = prepare_statement(db, sql1, &stmt1);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    rc = prepare_statement(db, sql2, &stmt2);
    if (rc != SQLITE_OK) {
        cleanup_statement(stmt1);
        cleanup_db(db);
        return 0;
    }

    // Extract parameters from fuzz input
    int param_index = (Data[0] % 10) + 1; // 1 to 10
    int blob_len_32 = (Data[1] % 1000) + 1; // 1 to 1000
    sqlite3_uint64 blob_len_64 = ((sqlite3_uint64)Data[1] << 32) | (Data[0] * 1000);
    
    // Ensure blob_len_64 is not too large to avoid SQLITE_TOOBIG immediately
    if (blob_len_64 > 1000000000ULL) {
        blob_len_64 = 1000000000ULL;
    }

    // Test sqlite3_bind_null
    rc = sqlite3_bind_null(stmt1, param_index);
    if (rc == SQLITE_OK) {
        // Execute to see if it works
        sqlite3_step(stmt1);
        sqlite3_reset(stmt1);
    }

    // Test sqlite3_bind_zeroblob
    rc = sqlite3_bind_zeroblob(stmt1, param_index, blob_len_32);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt1);
        sqlite3_reset(stmt1);
    }

    // Test sqlite3_bind_zeroblob64
    rc = sqlite3_bind_zeroblob64(stmt1, param_index, blob_len_64);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt1);
        sqlite3_reset(stmt1);
    }

    // Test sqlite3_bind_blob64 with fuzz data
    if (Size > 2) {
        const void *blob_data = (const void *)(Data + 2);
        size_t blob_size = Size - 2;
        if (blob_size > 1000000) {
            blob_size = 1000000; // Limit size to avoid memory issues
        }
        rc = sqlite3_bind_blob64(stmt1, param_index, blob_data, (sqlite3_uint64)blob_size, SQLITE_TRANSIENT);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt1);
            sqlite3_reset(stmt1);
        }
    }

    // Test sqlite3_clear_bindings
    rc = sqlite3_clear_bindings(stmt1);
    if (rc == SQLITE_OK) {
        // Re-bind and execute
        rc = sqlite3_bind_null(stmt1, param_index);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt1);
        }
    }

    // Test sqlite3_transfer_bindings
    // First bind something to stmt1
    rc = sqlite3_bind_null(stmt1, param_index);
    if (rc == SQLITE_OK) {
        rc = sqlite3_transfer_bindings(stmt1, stmt2);
        if (rc == SQLITE_OK) {
            // Execute stmt2
            sqlite3_step(stmt2);
            sqlite3_reset(stmt2);
        }
    }

    // Cleanup
    cleanup_statement(stmt1);
    cleanup_statement(stmt2);
    cleanup_db(db);

    return 0;
}
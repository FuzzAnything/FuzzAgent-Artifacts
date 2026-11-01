// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
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

static int fuzz_sqlite3_value_functions(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to work with
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data TEXT, val REAL);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare an insert statement
    const char *insert_sql = "INSERT INTO test (data, val) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a select statement to retrieve values
    const char *select_sql = "SELECT data, val FROM test WHERE id = 1;";
    sqlite3_stmt *select_stmt = NULL;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Use input data to bind values
    if (Size > 0) {
        sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);
        if (Size > 8) {
            double d_val = 0.0;
            memcpy(&d_val, Data + 8, sizeof(double));
            sqlite3_bind_double(stmt, 2, d_val);
        }
        rc = sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }

    // Execute select to get a value to test
    rc = sqlite3_step(select_stmt);
    if (rc == SQLITE_ROW) {
        // Get the sqlite3_value from column 0 (data)
        sqlite3_value *val = sqlite3_column_value(select_stmt, 0);
        if (val) {
            // Test sqlite3_value_dup
            sqlite3_value *dup_val = sqlite3_value_dup(val);
            if (dup_val) {
                // Test retrieval functions on the duplicated value
                // These are safe to call even if the value type doesn't match, they just return NULL/0
                const void *text16 = sqlite3_value_text16(dup_val);
                (void)text16; // Suppress unused warning

                const void *text16le = sqlite3_value_text16le(dup_val);
                (void)text16le;

                const void *text16be = sqlite3_value_text16be(dup_val);
                (void)text16be;

                double d = sqlite3_value_double(dup_val);
                (void)d;

                // Free the duplicated value
                sqlite3_value_free(dup_val);
            }
        }
    }

    // Cleanup
    sqlite3_finalize(select_stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Call the fuzzing logic
    fuzz_sqlite3_value_functions(Data, Size);

    return 0;
}
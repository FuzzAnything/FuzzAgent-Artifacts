// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a dummy table first
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (col1 INT, col2 INT, col3 REAL, col4 TEXT, col5 INT);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    // Prepare a simple SQL statement with placeholders
    const char *sql = "INSERT INTO test_table (col1, col2, col3, col4, col5) VALUES (?, ?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Extract data from the input buffer safely
    int32_t int_val1 = 0;
    sqlite3_int64 int64_val = 0;
    double double_val = 0.0;
    const char *text_val = "";
    size_t text_len = 0;
    int32_t int_val2 = 0;

    if (Size >= 4) {
        int_val1 = *(const int32_t *)(Data);
    }
    if (Size >= 8) {
        int64_val = *(const sqlite3_int64 *)(Data + 4);
    }
    if (Size >= 16) {
        double_val = *(const double *)(Data + 8);
    }
    if (Size >= 20) {
        text_len = *(const size_t *)(Data + 16);
        // Ensure text_len doesn't exceed available data and is reasonable
        if (text_len > 0 && Size > 20) {
            if (text_len > Size - 20) {
                text_len = Size - 20;
            }
            text_val = (const char *)(Data + 20);
        }
    }
    if (Size >= 24) {
        int_val2 = *(const int32_t *)(Data + 24);
    }

    // 1. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 1, int_val1);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // 2. sqlite3_bind_int64
    rc = sqlite3_bind_int64(stmt, 2, int64_val);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // 3. sqlite3_bind_double
    rc = sqlite3_bind_double(stmt, 3, double_val);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // 4. sqlite3_bind_text64
    // Use SQLITE_TRANSIENT to let SQLite copy the data, avoiding ownership issues with fuzz input
    rc = sqlite3_bind_text64(stmt, 4, text_val, text_len, SQLITE_TRANSIENT, SQLITE_UTF8);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // 5. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 5, int_val2);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Execute the statement
    rc = sqlite3_step(stmt);
    // We don't strictly need to check rc here for the fuzzer to be valid, 
    // but we ensure the statement is finalized regardless.
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
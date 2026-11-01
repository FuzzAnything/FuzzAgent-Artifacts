// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE test (id INTEGER, name TEXT, value REAL); INSERT INTO test VALUES (?, ?, ?);";
    
    // Initialize in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Prepare the statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Ensure we have enough data to extract values
    if (Size < 24) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Extract values from input data
    int int_val1 = *(const int32_t *)(Data);
    sqlite3_int64 int64_val = *(const sqlite3_int64 *)(Data + 4);
    double double_val = *(const double *)(Data + 12);
    int int_val2 = *(const int32_t *)(Data + 20);
    
    // Create a text buffer from input
    const char *text_data = (const char *)(Data + 24);
    size_t text_len = Size - 24;
    if (text_len > 1024) text_len = 1024; // Limit text length for safety

    // Invoke target functions in specified order
    sqlite3_bind_int(stmt, 1, int_val1);
    sqlite3_bind_int64(stmt, 2, int64_val);
    sqlite3_bind_double(stmt, 3, double_val);
    sqlite3_bind_text64(stmt, 4, text_data, text_len, SQLITE_STATIC, SQLITE_UTF8);
    sqlite3_bind_int(stmt, 5, int_val2);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
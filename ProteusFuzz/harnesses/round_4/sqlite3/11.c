// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_dummy_db(sqlite3 **ppDb) {
    int rc = sqlite3_open(":memory:", ppDb);
    if (rc != SQLITE_OK) {
        return 1;
    }

    const char *sql = "CREATE TABLE fuzz_table (id INTEGER PRIMARY KEY, data BLOB, name TEXT, val REAL, num INTEGER);";
    rc = sqlite3_exec(*ppDb, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*ppDb);
        *ppDb = NULL;
        return 1;
    }

    // Insert some dummy data to ensure we have rows to query
    const char *insert_sql = "INSERT INTO fuzz_table (data, name, val, num) VALUES (X'0102030405', 'test', 3.14, 42);";
    rc = sqlite3_exec(*ppDb, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*ppDb);
        *ppDb = NULL;
        return 1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // 1. Prepare the environment
    if (create_dummy_db(&db) != 0) {
        return 0;
    }

    // Prepare a SELECT statement to get a result set
    const char *select_sql = "SELECT id, data, name, val, num FROM fuzz_table;";
    int rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Step to the first row
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Determine column count to bound indices
    int col_count = sqlite3_column_count(stmt);
    if (col_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Use input data to select a column index safely
    int col_idx = 0;
    if (Size > 0) {
        col_idx = Data[0] % col_count;
    }

    // 2. Invoke target functions in the specified order
    
    // sqlite3_column_count
    (void)sqlite3_column_count(stmt);

    // sqlite3_column_type
    (void)sqlite3_column_type(stmt, col_idx);

    // sqlite3_column_int64
    (void)sqlite3_column_int64(stmt, col_idx);

    // sqlite3_column_double
    (void)sqlite3_column_double(stmt, col_idx);

    // sqlite3_value_encoding
    sqlite3_value *val = sqlite3_column_value(stmt, col_idx);
    if (val) {
        (void)sqlite3_value_encoding(val);
    }

    // sqlite3_column_value
    (void)sqlite3_column_value(stmt, col_idx);

    // sqlite3_column_bytes
    (void)sqlite3_column_bytes(stmt, col_idx);

    // sqlite3_column_blob
    (void)sqlite3_column_blob(stmt, col_idx);

    // sqlite3_column_bytes16
    (void)sqlite3_column_bytes16(stmt, col_idx);

    // sqlite3_column_blob
    (void)sqlite3_column_blob(stmt, col_idx);

    // sqlite3_column_bytes16
    (void)sqlite3_column_bytes16(stmt, col_idx);

    // sqlite3_column_blob
    (void)sqlite3_column_blob(stmt, col_idx);

    // sqlite3_value_encoding (again)
    val = sqlite3_column_value(stmt, col_idx);
    if (val) {
        (void)sqlite3_value_encoding(val);
    }

    // sqlite3_column_value (again)
    (void)sqlite3_column_value(stmt, col_idx);

    // sqlite3_column_bytes (again)
    (void)sqlite3_column_bytes(stmt, col_idx);

    // sqlite3_column_blob (again)
    (void)sqlite3_column_blob(stmt, col_idx);

    // 3. Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
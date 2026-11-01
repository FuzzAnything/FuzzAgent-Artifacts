// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
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
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 1, 2.5, 'hello', X'4849', NULL, 9223372036854775807;";
    int rc;
    int iCol;
    int nCols;

    if (Size < 1) return 0;

    /* Use in-memory database for fuzzing */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;

    /* Create a simple table to allow more complex queries if needed */
    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS test (id INTEGER, val TEXT, data BLOB);";
    sqlite3_exec(db, createTableSQL, NULL, NULL, NULL);

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step to get a row */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Get column count */
    nCols = sqlite3_column_count(stmt);
    if (nCols <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Fuzz column index based on input data */
    iCol = Data[0] % nCols;

    /* 1. sqlite3_column_count */
    (void)sqlite3_column_count(stmt);

    /* 2. sqlite3_column_type */
    (void)sqlite3_column_type(stmt, iCol);

    /* 3. sqlite3_column_int64 */
    (void)sqlite3_column_int64(stmt, iCol);

    /* 4. sqlite3_column_double */
    (void)sqlite3_column_double(stmt, iCol);

    /* 5. sqlite3_value_encoding */
    sqlite3_value *val = sqlite3_column_value(stmt, iCol);
    if (val) {
        (void)sqlite3_value_encoding(val);
    }

    /* 6. sqlite3_column_value */
    (void)sqlite3_column_value(stmt, iCol);

    /* 7. sqlite3_column_bytes */
    (void)sqlite3_column_bytes(stmt, iCol);

    /* 8. sqlite3_column_blob */
    (void)sqlite3_column_blob(stmt, iCol);

    /* 9. sqlite3_column_bytes16 */
    (void)sqlite3_column_bytes16(stmt, iCol);

    /* 10. sqlite3_column_blob */
    (void)sqlite3_column_blob(stmt, iCol);

    /* 11. sqlite3_column_bytes16 */
    (void)sqlite3_column_bytes16(stmt, iCol);

    /* 12. sqlite3_column_blob */
    (void)sqlite3_column_blob(stmt, iCol);

    /* 13. sqlite3_value_encoding */
    val = sqlite3_column_value(stmt, iCol);
    if (val) {
        (void)sqlite3_value_encoding(val);
    }

    /* 14. sqlite3_column_value */
    (void)sqlite3_column_value(stmt, iCol);

    /* 15. sqlite3_column_bytes */
    (void)sqlite3_column_bytes(stmt, iCol);

    /* 16. sqlite3_column_blob */
    (void)sqlite3_column_blob(stmt, iCol);

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
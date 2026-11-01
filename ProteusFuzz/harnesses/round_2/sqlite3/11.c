// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
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
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 1, 2.0, 'test', X'0102', 12345678901234567890;";
    int rc;

    /* Initialize in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step to the first row */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Fuzzing logic: Invoke target API functions in specified order */
    
    /* 1. sqlite3_column_count */
    int count = sqlite3_column_count(stmt);
    (void)count;

    /* 2. sqlite3_column_type */
    int type = sqlite3_column_type(stmt, 0);
    (void)type;

    /* 3. sqlite3_column_int64 */
    sqlite3_int64 intVal = sqlite3_column_int64(stmt, 0);
    (void)intVal;

    /* 4. sqlite3_column_double */
    double doubleVal = sqlite3_column_double(stmt, 1);
    (void)doubleVal;

    /* 5. sqlite3_value_encoding (requires sqlite3_column_value first) */
    sqlite3_value *val = sqlite3_column_value(stmt, 2);
    if (val) {
        int enc = sqlite3_value_encoding(val);
        (void)enc;
    }

    /* 6. sqlite3_column_value */
    val = sqlite3_column_value(stmt, 0);
    (void)val;

    /* 7. sqlite3_column_bytes */
    int bytes = sqlite3_column_bytes(stmt, 3);
    (void)bytes;

    /* 8. sqlite3_column_blob */
    const void *blob = sqlite3_column_blob(stmt, 3);
    (void)blob;

    /* 9. sqlite3_column_bytes16 */
    int bytes16 = sqlite3_column_bytes16(stmt, 2);
    (void)bytes16;

    /* 10. sqlite3_column_blob (again) */
    blob = sqlite3_column_blob(stmt, 3);
    (void)blob;

    /* 11. sqlite3_column_bytes16 (again) */
    bytes16 = sqlite3_column_bytes16(stmt, 2);
    (void)bytes16;

    /* 12. sqlite3_column_blob (again) */
    blob = sqlite3_column_blob(stmt, 3);
    (void)blob;

    /* 13. sqlite3_value_encoding (again) */
    val = sqlite3_column_value(stmt, 2);
    if (val) {
        int enc2 = sqlite3_value_encoding(val);
        (void)enc2;
    }

    /* 14. sqlite3_column_value (again) */
    val = sqlite3_column_value(stmt, 0);
    (void)val;

    /* 15. sqlite3_column_bytes (again) */
    bytes = sqlite3_column_bytes(stmt, 3);
    (void)bytes;

    /* 16. sqlite3_column_blob (again) */
    blob = sqlite3_column_blob(stmt, 3);
    (void)blob;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB); INSERT INTO test VALUES (?, ?); SELECT id, data FROM test;";
    int rc;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Prepare the initial setup statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind fuzzing data to the statement parameters */
    if (Size > 0) {
        sqlite3_bind_int(stmt, 1, (int)(Data[0] & 0xFF));
        sqlite3_bind_blob(stmt, 2, Data, Size, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_int(stmt, 1, 0);
        sqlite3_bind_blob(stmt, 2, "", 0, SQLITE_STATIC);
    }

    /* Execute the setup (Create and Insert) */
    rc = sqlite3_step(stmt);
    
    /* Finalize the setup statement */
    sqlite3_finalize(stmt);
    stmt = NULL;

    /* Re-prepare a simple SELECT to ensure we have data to read */
    rc = sqlite3_prepare_v2(db, "SELECT id, data FROM test;", -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Step to get the first row */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* 
       Target API Sequence:
       1. sqlite3_column_text
       2. sqlite3_bind_value
       3. sqlite3_column_value
       4. sqlite3_bind_value
       5. sqlite3_column_value
       6. sqlite3_step
       7. sqlite3_column_int
       8. sqlite3_reset
       9. sqlite3_column_bytes16
       10. sqlite3_column_blob
       11. sqlite3_column_bytes16
       12. sqlite3_column_blob
       13. sqlite3_column_bytes
       14. sqlite3_column_blob
       15. sqlite3_column_bytes
       16. sqlite3_column_blob
    */

    /* 1. sqlite3_column_text */
    const unsigned char *text_val = sqlite3_column_text(stmt, 0);
    (void)text_val;

    /* 2. sqlite3_bind_value */
    sqlite3_value *val1 = sqlite3_column_value(stmt, 0);
    if (val1) {
        sqlite3_bind_value(stmt, 1, val1); 
    }

    /* 3. sqlite3_column_value */
    sqlite3_value *val2 = sqlite3_column_value(stmt, 1);
    (void)val2;

    /* 4. sqlite3_bind_value */
    if (val2) {
        sqlite3_bind_value(stmt, 2, val2);
    }

    /* 5. sqlite3_column_value */
    sqlite3_value *val3 = sqlite3_column_value(stmt, 0);
    (void)val3;

    /* 6. sqlite3_step */
    rc = sqlite3_step(stmt);

    /* 7. sqlite3_column_int */
    int int_val = sqlite3_column_int(stmt, 0);
    (void)int_val;

    /* 8. sqlite3_reset */
    rc = sqlite3_reset(stmt);
    (void)rc;

    /* 9. sqlite3_column_bytes16 */
    int bytes16 = sqlite3_column_bytes16(stmt, 0);
    (void)bytes16;

    /* 10. sqlite3_column_blob */
    const void *blob1 = sqlite3_column_blob(stmt, 1);
    (void)blob1;

    /* 11. sqlite3_column_bytes16 */
    int bytes16_2 = sqlite3_column_bytes16(stmt, 1);
    (void)bytes16_2;

    /* 12. sqlite3_column_blob */
    const void *blob2 = sqlite3_column_blob(stmt, 0);
    (void)blob2;

    /* 13. sqlite3_column_bytes */
    int bytes = sqlite3_column_bytes(stmt, 0);
    (void)bytes;

    /* 14. sqlite3_column_blob */
    const void *blob3 = sqlite3_column_blob(stmt, 1);
    (void)blob3;

    /* 15. sqlite3_column_bytes */
    int bytes_2 = sqlite3_column_bytes(stmt, 1);
    (void)bytes_2;

    /* 16. sqlite3_column_blob */
    const void *blob4 = sqlite3_column_blob(stmt, 0);
    (void)blob4;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
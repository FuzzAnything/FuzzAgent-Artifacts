// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 column access APIs.
 * Target sequence:
 * sqlite3_column_count -> sqlite3_column_type -> sqlite3_column_int64 ->
 * sqlite3_column_double -> sqlite3_value_encoding -> sqlite3_column_value ->
 * sqlite3_column_bytes -> sqlite3_column_blob -> sqlite3_column_bytes16 ->
 * sqlite3_column_blob -> sqlite3_column_bytes16 -> sqlite3_column_blob ->
 * sqlite3_value_encoding -> sqlite3_column_value -> sqlite3_column_bytes ->
 * sqlite3_column_blob
 */

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int iCol = 0;
    sqlite3_value *val = NULL;

    /* 1. Prepare environment */
    if (Size < 1) {
        return 0;
    }

    /* Use the input data to construct a simple SQL query.
     * We try to create a SELECT statement that returns various types.
     * We ensure the string is null-terminated and safe.
     */
    size_t sqlLen = Size;
    if (sqlLen > 2000) sqlLen = 2000; /* Limit SQL size for safety */

    char *sql = (char *)malloc(sqlLen + 2);
    if (!sql) return 0;

    memcpy(sql, Data, sqlLen);
    sql[sqlLen] = '\0';

    /* Ensure the SQL starts with SELECT to guarantee columns if possible,
     * otherwise fallback to a safe default if parsing fails or it's not SELECT.
     * We attempt to execute the raw input first to test robustness.
     */

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        free(sql);
        return 0;
    }

    /* Try to prepare the statement from input data */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        /* If preparation fails, try a fallback query to ensure we test the column APIs */
        sqlite3_finalize(stmt);
        stmt = NULL;
        const char *fallback = "SELECT 1, 2.0, 'hello', X'010203', NULL;";
        rc = sqlite3_prepare_v2(db, fallback, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            free(sql);
            return 0;
        }
    }
    free(sql);

    /* Step to get a row */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        /* If no row, try to reset and step again or just finalize */
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* 2. Execute the target API sequence */

    /* 1. sqlite3_column_count */
    int colCount = sqlite3_column_count(stmt);
    if (colCount <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    /* Ensure iCol is within bounds */
    if (iCol >= colCount) iCol = 0;

    /* 2. sqlite3_column_type */
    (void)sqlite3_column_type(stmt, iCol);

    /* 3. sqlite3_column_int64 */
    (void)sqlite3_column_int64(stmt, iCol);

    /* 4. sqlite3_column_double */
    (void)sqlite3_column_double(stmt, iCol);

    /* 5. sqlite3_value_encoding (Requires sqlite3_value) */
    /* We need to get the value first to call encoding on it.
     * The sequence lists sqlite3_value_encoding before sqlite3_column_value.
     * To satisfy the dependency, we fetch the value now, call encoding,
     * then call sqlite3_column_value as per the list order.
     */
    val = sqlite3_column_value(stmt, iCol);
    if (val) {
        (void)sqlite3_value_encoding(val);
    }

    /* 6. sqlite3_column_value */
    val = sqlite3_column_value(stmt, iCol);
    (void)val;

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

    /* 13. sqlite3_value_encoding (Requires value again) */
    /* Fetch value first */
    val = sqlite3_column_value(stmt, iCol);
    if (val) {
        (void)sqlite3_value_encoding(val);
    }

    /* 14. sqlite3_column_value */
    val = sqlite3_column_value(stmt, iCol);
    (void)val;

    /* 15. sqlite3_column_bytes */
    (void)sqlite3_column_bytes(stmt, iCol);

    /* 16. sqlite3_column_blob */
    (void)sqlite3_column_blob(stmt, iCol);

    /* 3. Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
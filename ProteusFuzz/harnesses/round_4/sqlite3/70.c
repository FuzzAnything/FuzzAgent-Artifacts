// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:93342:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:93350:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3_value_* and sqlite3_vtab_in_* functions.
 */

static void fuzz_value_callback(sqlite3_context *pCtx, int argc, sqlite3_value **argv) {
    if (argc == 0) {
        sqlite3_result_null(pCtx);
        return;
    }

    sqlite3_value *val = argv[0];
    sqlite3_value *out_val = NULL;
    int rc;

    /* Test sqlite3_value_int on a protected value */
    int int_val = sqlite3_value_int(val);
    (void)int_val;

    /* Test sqlite3_value_text16 on a protected value */
    const void *text16 = sqlite3_value_text16(val);
    (void)text16;

    /* Test sqlite3_value_text16le on a protected value */
    const void *text16le = sqlite3_value_text16le(val);
    (void)text16le;

    /* Test sqlite3_value_nochange on a protected value */
    int nochange = sqlite3_value_nochange(val);
    (void)nochange;

    /* 
     * Test sqlite3_vtab_in_first and sqlite3_vtab_in_next.
     * These are expected to fail (return SQLITE_ERROR) because 'val'
     * is not an IN-list constraint from an xFilter method.
     */
    rc = sqlite3_vtab_in_first(val, &out_val);
    (void)rc;

    if (out_val != NULL) {
        rc = sqlite3_vtab_in_next(val, &out_val);
        (void)rc;
    }

    /* Return a dummy result to avoid errors */
    sqlite3_result_int(pCtx, 0);
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a custom function to host the protected sqlite3_value context */
    rc = sqlite3_create_function(db, "fuzz_test_func", -1, SQLITE_UTF8, NULL, fuzz_value_callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to hold data */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert fuzzed data */
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql = "INSERT INTO test_table (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);
        (void)rc;
        sqlite3_finalize(stmt);
    }

    /* Call the custom function with the data to trigger the callback */
    const char *select_sql = "SELECT fuzz_test_func(data) FROM test_table LIMIT 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        (void)rc; /* We just want to trigger the callback */
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}
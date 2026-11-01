// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
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

/*
 * Custom SQL function to fuzz sqlite3_value_encoding, sqlite3_value_bytes,
 * sqlite3_value_blob, sqlite3_value_int64, and sqlite3_value_text.
 * It also uses sqlite3_result_int64 to set the result.
 */
static void fuzz_value_functions(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1) return;

    sqlite3_value *val = argv[0];

    /* Fuzz sqlite3_value_encoding */
    int encoding = sqlite3_value_encoding(val);
    (void)encoding; /* Use return value to prevent unused warnings */

    /* Fuzz sqlite3_value_bytes */
    int bytes = sqlite3_value_bytes(val);
    (void)bytes;

    /* Fuzz sqlite3_value_blob */
    const void *blob = sqlite3_value_blob(val);
    if (blob != NULL) {
        /* Just ensure we don't use it after the next value access */
    }

    /* Fuzz sqlite3_value_int64 */
    sqlite3_int64 int64_val = sqlite3_value_int64(val);
    (void)int64_val;

    /* Fuzz sqlite3_value_text */
    const unsigned char *text = sqlite3_value_text(val);
    if (text != NULL) {
        /* Ensure we don't use it after the next value access */
    }

    /* Use sqlite3_result_int64 to set a result based on fuzzing state */
    sqlite3_result_int64(ctx, (sqlite3_int64)bytes + (sqlite3_int64)encoding);
}

/*
 * Helper to create a database and register the custom function.
 */
static int setup_database(sqlite3 **db_out) {
    int rc = sqlite3_open(":memory:", db_out);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db_out);
        *db_out = NULL;
        return -1;
    }

    rc = sqlite3_create_function(
        *db_out,
        "fuzz_value_func",
        -1, /* Variable number of arguments */
        SQLITE_UTF8,
        NULL, /* User data */
        fuzz_value_functions,
        NULL, /* xStep */
        NULL  /* xFinal */
    );

    if (rc != SQLITE_OK) {
        sqlite3_close(*db_out);
        *db_out = NULL;
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Setup database */
    if (setup_database(&db) != 0) {
        return 0;
    }

    /* Create a table to test with */
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare an insert statement */
    const char *insert_sql = "INSERT INTO test_table (data) VALUES (?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzzed data as a BLOB */
    sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);

    /* Execute the insert */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);

    /* Prepare a select statement to retrieve the data and call our custom function */
    const char *select_sql = "SELECT fuzz_value_func(data) FROM test_table";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Execute the select */
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        /* The result is already handled in the custom function */
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
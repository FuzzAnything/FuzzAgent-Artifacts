// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:93342:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:93350:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a temporary database file for fuzzing.
 * Uses "./dummy_file" as requested.
 */
static int create_dummy_db(sqlite3 **ppDb) {
    int rc;
    const char *zFile = "./dummy_file";

    /* Remove existing file to ensure a clean state */
    remove(zFile);

    rc = sqlite3_open(zFile, ppDb);
    if (rc != SQLITE_OK) {
        if (*ppDb) {
            sqlite3_close(*ppDb);
        }
        *ppDb = NULL;
        return -1;
    }
    return 0;
}

/*
 * Static helper to close the database and clean up.
 */
static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
    /* Clean up the dummy file */
    remove("./dummy_file");
}

/*
 * Callback for a custom SQL function to safely test value accessors.
 * sqlite3_value_* functions require "protected" sqlite3_value objects,
 * which are only guaranteed to be protected inside user-defined SQL functions.
 * Calling them on unprotected values (like those from sqlite3_column_value)
 * can trigger assertions in debug/asan builds.
 */
static void fuzz_value_accessors(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1) return;

    sqlite3_value *pVal = argv[0];
    const void *blob_ptr = NULL;
    const void *text16le_ptr = NULL;
    int int_val = 0;
    int nochange_val = 0;
    sqlite3_value *pOut = NULL;
    int rc;

    /* Test sqlite3_value_blob */
    blob_ptr = sqlite3_value_blob(pVal);
    (void)blob_ptr;

    /* Test sqlite3_value_text16le */
    text16le_ptr = sqlite3_value_text16le(pVal);
    (void)text16le_ptr;

    /* Test sqlite3_value_int */
    int_val = sqlite3_value_int(pVal);
    (void)int_val;

    /* Test sqlite3_value_nochange */
    nochange_val = sqlite3_value_nochange(pVal);
    (void)nochange_val;

    /* Test sqlite3_vtab_in_first and sqlite3_vtab_in_next */
    /* Note: These are designed for xFilter of virtual tables.
     * Calling them here is technically undefined behavior, but useful for fuzzing
     * to trigger error paths without a full vtab setup.
     * We expect them to return SQLITE_ERROR or similar.
     */
    rc = sqlite3_vtab_in_first(pVal, &pOut);
    (void)rc;
    (void)pOut;

    rc = sqlite3_vtab_in_next(pVal, &pOut);
    (void)rc;
    (void)pOut;

    /* Return a dummy result to satisfy the function contract */
    sqlite3_result_int(ctx, 0);
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    if (Size < 2) {
        return 0;
    }

    /* 1. Prepare the environment */
    if (create_dummy_db(&db) != 0) {
        return 0;
    }

    /* Create a simple table to work with */
    const char *zCreate = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB, name TEXT);";
    rc = sqlite3_exec(db, zCreate, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    /* Register the custom function that contains the actual fuzzing logic */
    rc = sqlite3_create_function(db, "fuzz_test_func", -1, SQLITE_UTF8, NULL, fuzz_value_accessors, NULL, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    /* Prepare an INSERT statement to bind fuzz data */
    const char *zInsert = "INSERT INTO fuzz_test (data, name) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, zInsert, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    /* Use fuzz data to bind values */
    size_t blob_len = (Size > 100) ? 100 : Size;
    sqlite3_bind_blob(stmt, 1, Data, (int)blob_len, SQLITE_STATIC);

    size_t text_len = Size - blob_len;
    if (text_len > 0) {
        char *zText = (char *)malloc(text_len + 1);
        if (zText) {
            memcpy(zText, Data + blob_len, text_len);
            zText[text_len] = '\0';
            sqlite3_bind_text(stmt, 2, zText, -1, SQLITE_TRANSIENT);
            free(zText);
        }
    } else {
        sqlite3_bind_text(stmt, 2, "", 0, SQLITE_STATIC);
    }

    /* Execute the insert */
    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);

    /* Prepare a SELECT statement that calls our custom function.
     * This ensures the sqlite3_value objects passed to fuzz_value_accessors
     * are "protected" and safe to use with the target APIs.
     */
    sqlite3_finalize(stmt);
    const char *zSelect = "SELECT fuzz_test_func(data) FROM fuzz_test LIMIT 1;";
    rc = sqlite3_prepare_v2(db, zSelect, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    /* Step to get a row, which triggers the callback */
    rc = sqlite3_step(stmt);
    /* We don't strictly need to check the result here, the side effect is the fuzzing */

    sqlite3_finalize(stmt);

    /* 3. Cleanup */
    cleanup_db(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 value access functions.
 * Target APIs:
 * - sqlite3_value_text16be
 * - sqlite3_value_text16
 * - sqlite3_value_free
 * - sqlite3_value_dup
 * - sqlite3_value_blob
 * - sqlite3_value_text16le
 */

static void custom_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int i;
    sqlite3_value *val;
    sqlite3_value *dup_val;
    const void *blob_data;
    const void *text16_data;
    const void *text16be_data;
    const void *text16le_data;

    for (i = 0; i < argc; i++) {
        val = argv[i];
        if (!val) continue;

        /* Test sqlite3_value_blob */
        blob_data = sqlite3_value_blob(val);
        (void)blob_data; /* Suppress unused variable warning */

        /* Test sqlite3_value_text16 */
        text16_data = sqlite3_value_text16(val);
        (void)text16_data;

        /* Test sqlite3_value_text16be */
        text16be_data = sqlite3_value_text16be(val);
        (void)text16be_data;

        /* Test sqlite3_value_text16le */
        text16le_data = sqlite3_value_text16le(val);
        (void)text16le_data;

        /* Test sqlite3_value_dup and sqlite3_value_free */
        dup_val = sqlite3_value_dup(val);
        if (dup_val) {
            /* Access duplicated value to ensure it's protected and valid */
            blob_data = sqlite3_value_blob(dup_val);
            (void)blob_data;
            text16_data = sqlite3_value_text16(dup_val);
            (void)text16_data;
            text16be_data = sqlite3_value_text16be(dup_val);
            (void)text16be_data;
            text16le_data = sqlite3_value_text16le(dup_val);
            (void)text16le_data;
            sqlite3_value_free(dup_val);
        }
    }
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *dummy_file = "./dummy_file";

    /* Create a temporary file for the database */
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fclose(f);
    }

    /* Open database */
    rc = sqlite3_open_v2(dummy_file, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }

    /* Register custom function */
    rc = sqlite3_create_function(db, "fuzz_func", -1, SQLITE_UTF8, NULL, custom_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a simple table */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare an INSERT statement with parameters */
    rc = sqlite3_prepare_v2(db, "INSERT INTO test (data) VALUES (?);", -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a SELECT statement to retrieve data and trigger the custom function */
    sqlite3_stmt *select_stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_func(data) FROM test;", -1, &select_stmt, NULL);
    if (rc != SQLITE_OK || !select_stmt) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Use input data to bind parameters */
    if (Size > 0) {
        /* Bind the input data as a blob */
        rc = sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_STATIC);
        if (rc == SQLITE_OK) {
            /* Execute insert */
            rc = sqlite3_step(stmt);
            (void)rc;
        }
        sqlite3_reset(stmt);
    }

    /* Execute select to trigger custom function */
    rc = sqlite3_step(select_stmt);
    (void)rc;

    /* Finalize statements */
    sqlite3_finalize(select_stmt);
    sqlite3_finalize(stmt);

    /* Cleanup database */
    sqlite3_close(db);

    /* Remove dummy file */
    remove(dummy_file);

    return 0;
}
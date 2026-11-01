// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:104964:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:104957:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
 * Dummy callback functions for sqlite3_create_function
 */
static void dummy_xFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;
    sqlite3_result_null(context);
}

static void dummy_xStep(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;
}

static void dummy_xFinal(sqlite3_context *context) {
    (void)context;
    sqlite3_result_null(context);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *err_msg;
    int rc;
    int nArg, eTextRep;
    void *pApp;
    int iOffset, nBytes;
    char *buffer;
    const char *sql;
    const char *pzTail;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a table to work with for BLOB operations */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert a row with some initial data to open a BLOB handle */
    rc = sqlite3_exec(db, "INSERT INTO test_table (data) VALUES (X'00000000000000000000000000000000');", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Safely extract parameters from fuzz input */
    if (Size > 0) {
        nArg = (int)(Data[0] & 0x7F) - 128; /* Range: -128 to 127 */
        if (nArg < -1) nArg = -1;
        if (nArg > 127) nArg = 127;
    } else {
        nArg = 0;
    }

    if (Size > 1) {
        eTextRep = (int)(Data[1] & 0x03); /* SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16 */
    } else {
        eTextRep = SQLITE_UTF8;
    }

    pApp = (void *)(uintptr_t)((Size > 2) ? (Data[2] & 0xFF) : 0);

    if (Size > 3) {
        iOffset = (int)(Data[3] & 0xFF);
    } else {
        iOffset = 0;
    }

    if (Size > 4) {
        nBytes = (int)(Data[4] & 0xFF);
    } else {
        nBytes = 0;
    }

    /* Allocate buffer for BLOB read/write */
    buffer = (char *)malloc(nBytes > 0 ? nBytes : 1);
    if (!buffer) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare SQL for later use */
    if (Size > 5) {
        size_t sql_len = (Size > 5) ? (Size - 5) : 0;
        if (sql_len > 0) {
            sql = (const char *)(Data + 5);
        } else {
            sql = "SELECT 1;";
        }
    } else {
        sql = "SELECT 1;";
    }

    /* Open a BLOB handle for writing */
    rc = sqlite3_blob_open(db, "main", "test_table", "data", 1, SQLITE_OPEN_READWRITE, &blob);
    if (rc != SQLITE_OK) {
        /* Try to open in read-only mode if write fails, to test error paths */
        rc = sqlite3_blob_open(db, "main", "test_table", "data", 1, SQLITE_OPEN_READONLY, &blob);
    }

    /* 1. sqlite3_blob_write */
    if (blob) {
        rc = sqlite3_blob_write(blob, buffer, nBytes, iOffset);
        (void)rc; /* Ignore return for fuzzing, we just want to exercise the code path */
    }

    /* 2. sqlite3_errmsg */
    err_msg = sqlite3_errmsg(db);
    (void)err_msg; /* Use the result to ensure it's computed */

    /* 3. sqlite3_blob_read */
    if (blob) {
        rc = sqlite3_blob_read(blob, buffer, nBytes, iOffset);
        (void)rc;
    }

    /* 4. sqlite3_errmsg */
    err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    /* Close the BLOB handle */
    if (blob) {
        sqlite3_blob_close(blob);
        blob = NULL;
    }

    /* 5. sqlite3_create_function */
    rc = sqlite3_create_function(db, "fuzz_func", nArg, eTextRep, pApp, dummy_xFunc, dummy_xStep, dummy_xFinal);
    (void)rc;

    /* 6. sqlite3_prepare_v2 */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &pzTail);
    (void)rc;
    (void)pzTail;

    /* Cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    free(buffer);
    sqlite3_close(db);

    return 0;
}
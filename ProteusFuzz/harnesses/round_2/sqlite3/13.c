// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_bytes at sqlite3.c:104974:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:104964:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:104964:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:104964:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:104974:16 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:104957:16 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:104957:16 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:104957:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/*
 * Static helper function to create a dummy database file.
 * This ensures the file exists for sqlite3_blob_open operations.
 */
static int setup_dummy_db(const char *zFilename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(zFilename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS fuzz_table (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, sql, NULL, NULL, NULL);

    /* Insert a row with a blob to ensure we have something to read/write */
    const char *insert_sql = "INSERT OR IGNORE INTO fuzz_table (id, data) VALUES (1, X'0102030405060708090A0B0C0D0E0F10');";
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);

    sqlite3_close(db);
    return 1;
}

/*
 * Dummy function implementation for sqlite3_create_function
 */
static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;
    sqlite3_result_null(context);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    const char *zFilename = "./dummy_file";
    int rc;
    int i;

    /* 1. Prepare the environment */
    if (Size == 0) return 0;

    /* Ensure the dummy file exists and has the required table */
    setup_dummy_db(zFilename);

    rc = sqlite3_open(zFilename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Initialize a buffer for blob operations */
    unsigned char *blob_buf = (unsigned char *)malloc(Size > 0 ? Size : 1);
    if (!blob_buf) {
        sqlite3_close(db);
        return 0;
    }
    memset(blob_buf, 0, Size > 0 ? Size : 1);

    /* Try to open a blob handle. We need a valid row to operate on. */
    /* Using a hardcoded SQL to open the blob for the first row */
    const char *blob_sql = "SELECT data FROM fuzz_table WHERE id = 1";
    rc = sqlite3_prepare_v2(db, blob_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        rc = sqlite3_blob_open(
            db,
            "main",
            "fuzz_table",
            "data",
            sqlite3_column_int64(stmt, 0),
            1, /* writable */
            &blob
        );
    }
    if (stmt) sqlite3_finalize(stmt);
    stmt = NULL;

    /*
     * 2. Invoke target functions in the specified order:
     *    sqlite3_blob_write -> sqlite3_errmsg -> sqlite3_blob_read -> sqlite3_errmsg
     *    -> sqlite3_create_function -> sqlite3_prepare_v2
     */

    /* A. sqlite3_blob_write */
    /* We attempt to write data. If blob is NULL or invalid, this will fail gracefully. */
    if (blob && Size > 0) {
        /* Use fuzz data to determine offset and length, but clamp to safe ranges */
        int offset = (Size > 1) ? (Data[0] % 16) : 0;
        int len = (Size > 2) ? (Data[1] % 16) : 1;
        
        /* Ensure we don't write beyond reasonable bounds if blob is valid */
        int blob_size = sqlite3_blob_bytes(blob);
        if (blob_size > 0) {
            if (offset + len > blob_size) len = blob_size - offset;
            if (len > 0) {
                sqlite3_blob_write(blob, &Data[2], len, offset);
            }
        } else {
            /* Try to write anyway to trigger error states if blob is invalid/expired */
            sqlite3_blob_write(blob, &Data[0], 1, 0);
        }
    } else {
        /* Call with NULL or invalid blob to test error handling */
        sqlite3_blob_write(blob, NULL, 0, 0);
    }

    /* B. sqlite3_errmsg */
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg; /* Consume result */

    /* C. sqlite3_blob_read */
    if (blob && Size > 0) {
        int offset = (Size > 1) ? (Data[0] % 16) : 0;
        int len = (Size > 2) ? (Data[1] % 16) : 1;
        
        int blob_size = sqlite3_blob_bytes(blob);
        if (blob_size > 0) {
            if (offset + len > blob_size) len = blob_size - offset;
            if (len > 0) {
                sqlite3_blob_read(blob, blob_buf, len, offset);
            }
        } else {
            sqlite3_blob_read(blob, blob_buf, 1, 0);
        }
    } else {
        sqlite3_blob_read(blob, NULL, 0, 0);
    }

    /* D. sqlite3_errmsg (again) */
    err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    /* Cleanup blob handle if open */
    if (blob) {
        sqlite3_blob_close(blob);
        blob = NULL;
    }

    /* E. sqlite3_create_function */
    /* Register a dummy function using fuzz data for name if possible, else fixed */
    const char *func_name = "fuzz_func";
    if (Size > 10 && Data[0] != 0) {
        /* Create a temporary name from fuzz data, ensuring null termination */
        static char name_buf[256];
        int name_len = Size > 255 ? 255 : Size;
        memcpy(name_buf, Data, name_len);
        name_buf[name_len] = '\0';
        func_name = name_buf;
    }
    
    int nArg = (Size > 2) ? (int)Data[2] : 0;
    int eTextRep = (Size > 3) ? (int)Data[3] : SQLITE_UTF8;
    /* Clamp eTextRep to valid values roughly */
    if (eTextRep != SQLITE_UTF8 && eTextRep != SQLITE_UTF16LE && eTextRep != SQLITE_UTF16BE && eTextRep != SQLITE_UTF16) {
        eTextRep = SQLITE_UTF8;
    }

    rc = sqlite3_create_function(
        db,
        func_name,
        nArg,
        eTextRep,
        NULL,
        dummy_func,
        NULL,
        NULL
    );
    (void)rc;

    /* F. sqlite3_prepare_v2 */
    /* Prepare a statement using the fuzz data as SQL */
    const char *zSql = (const char *)Data;
    int nByte = (int)Size;
    
    /* Ensure null termination for safety if the fuzzer doesn't provide it */
    /* Note: sqlite3_prepare_v2 handles nByte, but let's be safe */
    sqlite3_stmt *pStmt = NULL;
    const char *pzTail = NULL;

    rc = sqlite3_prepare_v2(db, zSql, nByte, &pStmt, &pzTail);
    (void)rc;

    /* Cleanup */
    if (pStmt) {
        sqlite3_finalize(pStmt);
    }
    free(blob_buf);
    sqlite3_close(db);

    return 0;
}
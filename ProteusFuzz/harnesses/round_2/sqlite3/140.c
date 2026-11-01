// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
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
 * Fuzz driver for sqlite3_value_* and sqlite3_result_int64 APIs.
 *
 * Strategy:
 * 1. Create an in-memory database.
 * 2. Create a custom SQL scalar function that receives arguments.
 * 3. Inside the function callback, invoke the target APIs on the arguments.
 * 4. Use the fuzzing input to drive the SQL statement and bound parameters.
 * 5. Ensure proper cleanup and error handling.
 */

static void my_test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int i;
    sqlite3_int64 int_val;
    const void *blob_ptr;
    const unsigned char *text_ptr;
    int bytes_len;
    int encoding;

    if (!ctx) return;

    for (i = 0; i < argc; i++) {
        if (!argv[i]) continue;

        /* Test sqlite3_value_bytes */
        bytes_len = sqlite3_value_bytes(argv[i]);
        (void)bytes_len; /* Use the return value to avoid unused warnings */

        /* Test sqlite3_value_blob */
        blob_ptr = sqlite3_value_blob(argv[i]);
        (void)blob_ptr;

        /* Test sqlite3_value_text */
        text_ptr = sqlite3_value_text(argv[i]);
        (void)text_ptr;

        /* Test sqlite3_value_int64 */
        int_val = sqlite3_value_int64(argv[i]);
        (void)int_val;

        /* Test sqlite3_value_encoding (only meaningful for TEXT, but safe to call) */
        encoding = sqlite3_value_encoding(argv[i]);
        (void)encoding;
    }

    /* Test sqlite3_result_int64 with a value derived from input or fixed */
    sqlite3_result_int64(ctx, 0);
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    char *sql;
    int sql_len;
    int i;

    /* Guard against empty or extremely small inputs to avoid trivial crashes */
    if (Size < 1) {
        return 0;
    }

    /* Initialize in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to hold test data */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "my_test_func", -1, SQLITE_UTF8, NULL, my_test_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a SQL statement that uses the custom function and binds parameters */
    /* We construct a simple SELECT that calls our function with bound values */
    sql = sqlite3_mprintf("SELECT my_test_func(?, ?, ?, ?, ?);");
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    sql_len = (int)strlen(sql);

    rc = sqlite3_prepare_v2(db, sql, sql_len, &stmt, NULL);
    sqlite3_free(sql);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Use the fuzzing input to bind parameters */
    /* We bind up to 5 parameters, cycling through the input data */
    for (i = 0; i < 5; i++) {
        int param_idx = i + 1;
        int data_idx = (i * (Size / 5));

        if (data_idx >= (int)Size) {
            data_idx = Size - 1;
        }

        /* Bind different types based on index to explore more states */
        if (i == 0) {
            /* Bind as integer */
            sqlite3_bind_int64(stmt, param_idx, (sqlite3_int64)Data[data_idx]);
        } else if (i == 1) {
            /* Bind as text (ensure null termination) */
            int text_len = Size - data_idx;
            if (text_len > 100) text_len = 100; /* Cap text length */
            /* Create a temporary buffer for text */
            char *text_buf = (char *)malloc(text_len + 1);
            if (text_buf) {
                memcpy(text_buf, Data + data_idx, text_len);
                text_buf[text_len] = '\0';
                sqlite3_bind_text(stmt, param_idx, text_buf, text_len, SQLITE_TRANSIENT);
                free(text_buf);
            } else {
                sqlite3_bind_text(stmt, param_idx, "", 0, SQLITE_STATIC);
            }
        } else if (i == 2) {
            /* Bind as blob */
            int blob_len = Size - data_idx;
            if (blob_len > 100) blob_len = 100;
            sqlite3_bind_blob(stmt, param_idx, Data + data_idx, blob_len, SQLITE_TRANSIENT);
        } else if (i == 3) {
            /* Bind as NULL */
            sqlite3_bind_null(stmt, param_idx);
        } else {
            /* Bind as double */
            double d_val = (double)Data[data_idx];
            sqlite3_bind_double(stmt, param_idx, d_val);
        }
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    /* We don't care about the result of the step, just that it doesn't crash */
    (void)rc;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
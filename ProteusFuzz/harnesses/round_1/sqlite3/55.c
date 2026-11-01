// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for sqlite3_result_error16, sqlite3_result_blob,
 * sqlite3_result_text, sqlite3_result_zeroblob, sqlite3_aggregate_context,
 * and sqlite3_result_error_code.
 */

/* Custom function that exercises the target APIs based on input data */
static void fuzz_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* We do not have direct access to raw fuzz input here, so we rely on
     * the caller to have set up the context or use dummy data.
     * This function is invoked by SQLite during query execution.
     * We will use the arguments passed to the function to derive test data.
     */
    
    if (ctx == NULL) {
        /* API armor: safely return if context is NULL */
        return;
    }

    /* Extract some data from arguments to use as fuzz input */
    const void *blob_data = NULL;
    int blob_len = 0;
    const char *text_data = NULL;
    int text_len = 0;
    int err_code = SQLITE_ERROR;
    int zero_len = 0;
    int agg_size = 0;

    if (argc > 0) {
        blob_data = sqlite3_value_blob(argv[0]);
        blob_len = sqlite3_value_bytes(argv[0]);
    }
    if (argc > 1) {
        text_data = (const char *)sqlite3_value_text(argv[1]);
        text_len = sqlite3_value_bytes(argv[1]);
    }
    if (argc > 2) {
        err_code = sqlite3_value_int(argv[2]);
    }
    if (argc > 3) {
        zero_len = sqlite3_value_int(argv[3]);
    }
    if (argc > 4) {
        agg_size = sqlite3_value_int(argv[4]);
    }

    /* Test sqlite3_result_error16 with UTF-16 data */
    if (argc > 0 && blob_len > 0) {
        /* Treat blob as UTF-16 for error message */
        sqlite3_result_error16(ctx, blob_data, blob_len);
    }

    /* Test sqlite3_result_blob */
    if (argc > 0 && blob_len > 0) {
        sqlite3_result_blob(ctx, blob_data, blob_len, SQLITE_TRANSIENT);
    }

    /* Test sqlite3_result_text */
    if (argc > 1 && text_data != NULL && text_len > 0) {
        sqlite3_result_text(ctx, text_data, text_len, SQLITE_TRANSIENT);
    }

    /* Test sqlite3_result_zeroblob */
    if (argc > 3) {
        sqlite3_result_zeroblob(ctx, zero_len);
    }

    /* Test sqlite3_aggregate_context */
    if (argc > 4) {
        void *pCtx = sqlite3_aggregate_context(ctx, agg_size);
        if (pCtx != NULL) {
            /* Use the context to store some state */
            memset(pCtx, 0xAB, agg_size);
        }
    }

    /* Test sqlite3_result_error_code */
    if (argc > 2) {
        sqlite3_result_error_code(ctx, err_code);
        /* Also set an error message to trigger the error state */
        sqlite3_result_error(ctx, "Custom error from fuzz", -1);
    }
}

/*
 * LLVMFuzzerTestOneInput is the entry point for the fuzzer.
 * It creates a database, registers the custom function, and executes
 * queries that invoke the custom function with varying inputs.
 */
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Open an in-memory database */
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
    rc = sqlite3_create_function(db, "fuzz_func", 5, SQLITE_UTF8, NULL, fuzz_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert fuzz data into the table */
    char *insert_sql = NULL;
    if (Size > 0) {
        /* Escape single quotes in the data */
        char *escaped = malloc(Size * 2 + 1);
        if (escaped) {
            size_t j = 0;
            for (size_t i = 0; i < Size; i++) {
                if (Data[i] == '\'') {
                    escaped[j++] = '\'';
                }
                escaped[j++] = Data[i];
            }
            escaped[j] = '\0';

            insert_sql = malloc(strlen("INSERT INTO test_table (data) VALUES (X'") + Size * 2 + 4);
            if (insert_sql) {
                /* Convert binary data to hex string for SQL insertion */
                char hex[Size * 2 + 1];
                for (size_t i = 0; i < Size; i++) {
                    sprintf(hex + i * 2, "%02x", Data[i]);
                }
                hex[Size * 2] = '\0';
                sprintf(insert_sql, "INSERT INTO test_table (data) VALUES (X'%s')", hex);

                sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
                free(insert_sql);
            }
            free(escaped);
        }
    }

    /* Execute a query that calls the custom function with varying arguments */
    const char *query = "SELECT fuzz_func(data, data, 1, 100, 256) FROM test_table;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Process result if needed */
        }
        sqlite3_finalize(stmt);
    }

    /* Execute another query to test aggregate context */
    const char *agg_query = "SELECT fuzz_func(data, data, 2, 50, 128) FROM test_table GROUP BY data;";
    rc = sqlite3_prepare_v2(db, agg_query, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Process result if needed */
        }
        sqlite3_finalize(stmt);
    }

    /* Clean up */
    sqlite3_close(db);
    return 0;
}
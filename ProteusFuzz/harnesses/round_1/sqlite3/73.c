// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:95598:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:92610:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_value_* and sqlite3_result_int64 APIs.
 *
 * Fix for AddressSanitizer error:
 * The error "assert( pMem->db==0 || sqlite3_mutex_held(pMem->db->mutex) )" indicates
 * that we are accessing an unprotected sqlite3_value object (returned by sqlite3_column_value)
 * directly with APIs that require a protected object (mutex held).
 *
 * Solution:
 * Instead of calling sqlite3_column_value and then accessing it, we must use the column
 * access functions (sqlite3_column_int64, sqlite3_column_blob, etc.) which handle the
 * locking internally, OR we must duplicate the value using sqlite3_value_dup to make it
 * protected, then free it later.
 *
 * However, the target APIs are specifically sqlite3_value_*. To test them correctly,
 * we must obtain a PROTECTED sqlite3_value.
 * Protected values are typically passed as arguments to user-defined functions.
 * Therefore, the correct approach to fuzz sqlite3_value_* APIs is to:
 * 1. Create a user-defined function.
 * 2. Bind data to a statement.
 * 3. Call the user-defined function within the SQL execution, passing the bound columns.
 * 4. Inside the user-defined function, the arguments (sqlite3_value*) are PROTECTED.
 */

static void fuzz_value_extractor(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (argc < 1) return;

    sqlite3_value *val = argv[0];
    if (!val) return;

    /* Test sqlite3_value_int64 on protected value */
    sqlite3_int64 int64_val = sqlite3_value_int64(val);
    (void)int64_val;

    /* Test sqlite3_value_blob on protected value */
    const void *blob = sqlite3_value_blob(val);
    int blob_bytes = sqlite3_value_bytes(val);
    if (blob && blob_bytes > 0) {
        /* Safe access */
        const uint8_t *p = (const uint8_t *)blob;
        (void)p[0];
    }

    /* Test sqlite3_value_text on protected value */
    const unsigned char *text = sqlite3_value_text(val);
    int text_bytes = sqlite3_value_bytes(val);
    if (text && text_bytes > 0) {
        (void)text[0];
    }

    /* Test sqlite3_value_encoding on protected value */
    if (sqlite3_value_type(val) == SQLITE_TEXT) {
        int enc = sqlite3_value_encoding(val);
        (void)enc;
    }

    /* Force conversions to explore states */
    (void)sqlite3_value_type(val);
    (void)sqlite3_value_numeric_type(val);
    (void)sqlite3_value_nochange(val);
    (void)sqlite3_value_frombind(val);
    
    /* Test pointer passing if applicable (requires specific setup, skipping for general fuzz) */
    // const void *ptr = sqlite3_value_pointer(val, "test");
    // (void)ptr;
}

static void my_result_int64_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_int64 val = 0;
    if (sqlite3_context_db_handle(ctx)) {
        val = 123456789012345LL;
        sqlite3_result_int64(ctx, val);
    }
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a test table */
    const char *sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB, name TEXT, val INTEGER);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement to insert fuzzed data */
    const char *insert_sql = "INSERT INTO test_table (id, data, name, val) VALUES (?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Use fuzzed data to bind parameters */
    if (Size > 0) {
        /* Bind id (integer) */
        int id_val = (int)(Data[0] % 1000);
        sqlite3_bind_int64(stmt, 1, id_val);

        /* Bind data (BLOB) */
        size_t blob_len = 0;
        if (Size > 1) {
            blob_len = Size - 1;
            if (blob_len > 100) blob_len = 100;
            sqlite3_bind_blob(stmt, 2, (const void *)(Data + 1), (int)blob_len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 2);
        }

        /* Bind name (TEXT) */
        if (Size > 1) {
            size_t text_len = Size - 1;
            if (text_len > 100) text_len = 100;
            char *text_buf = (char *)malloc(text_len + 1);
            if (text_buf) {
                memcpy(text_buf, Data + 1, text_len);
                text_buf[text_len] = '\0';
                sqlite3_bind_text(stmt, 3, text_buf, -1, SQLITE_TRANSIENT);
                free(text_buf);
            } else {
                sqlite3_bind_null(stmt, 3);
            }
        } else {
            sqlite3_bind_null(stmt, 3);
        }

        /* Bind val (integer) */
        int val_int = (int)(Data[0] % 10000);
        sqlite3_bind_int64(stmt, 4, val_int);
    } else {
        sqlite3_bind_null(stmt, 1);
        sqlite3_bind_null(stmt, 2);
        sqlite3_bind_null(stmt, 3);
        sqlite3_bind_null(stmt, 4);
    }

    /* Execute insert */
    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);

    /* Register the custom function that will receive PROTECTED sqlite3_value objects */
    sqlite3_create_function(db, "fuzz_value_extractor", 1, SQLITE_UTF8, NULL, fuzz_value_extractor, NULL, NULL);
    
    /* Register the result function */
    sqlite3_create_function(db, "my_result_int64_func", 0, SQLITE_UTF8, NULL, my_result_int64_func, NULL, NULL);

    /* Prepare a select statement that calls our custom function with column values */
    /* This ensures the arguments passed to fuzz_value_extractor are protected */
    const char *select_sql = "SELECT fuzz_value_extractor(data), fuzz_value_extractor(name), fuzz_value_extractor(id), fuzz_value_extractor(val) FROM test_table LIMIT 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Execute the query */
    rc = sqlite3_step(stmt);
    /* We don't need to check the result values here, the side effects in the function are what we want */

    sqlite3_finalize(stmt);

    /* Test sqlite3_result_int64 via the registered function */
    const char *func_sql = "SELECT my_result_int64_func();";
    rc = sqlite3_prepare_v2(db, func_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            sqlite3_int64 res = sqlite3_column_int64(stmt, 0);
            (void)res;
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    return 0;
}
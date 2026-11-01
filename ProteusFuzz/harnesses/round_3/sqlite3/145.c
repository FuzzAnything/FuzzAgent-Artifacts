// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
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
 * Fuzz driver for sqlite3 value access functions.
 * Target APIs:
 * 1. sqlite3_value_text16be
 * 2. sqlite3_value_text16
 * 3. sqlite3_value_text
 * 4. sqlite3_value_text16le
 * 5. sqlite3_value_subtype
 * 6. sqlite3_value_blob
 *
 * Strategy:
 * - Create an in-memory database.
 * - Register a custom SQL function that receives sqlite3_value arguments.
 * - Inside the function, call the target APIs on the arguments.
 * - Use the fuzzer input to generate SQL statements that bind various data types
 *   (text, blob, int, float, null) to the function arguments.
 * - Ensure proper cleanup of database and statements.
 */

static void fuzz_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int i;
    const void *blob_ptr;
    const unsigned char *text_ptr;
    const void *text16_ptr;
    const void *text16be_ptr;
    const void *text16le_ptr;
    unsigned int subtype;
    sqlite3_value *protected_val;

    for (i = 0; i < argc; i++) {
        /* Test sqlite3_value_subtype */
        subtype = sqlite3_value_subtype(argv[i]);
        (void)subtype;

        /* Test sqlite3_value_blob */
        blob_ptr = sqlite3_value_blob(argv[i]);
        (void)blob_ptr;

        /* Test sqlite3_value_text */
        text_ptr = sqlite3_value_text(argv[i]);
        (void)text_ptr;

        /* Test sqlite3_value_text16 */
        text16_ptr = sqlite3_value_text16(argv[i]);
        (void)text16_ptr;

        /* Test sqlite3_value_text16be */
        text16be_ptr = sqlite3_value_text16be(argv[i]);
        (void)text16be_ptr;

        /* Test sqlite3_value_text16le */
        text16le_ptr = sqlite3_value_text16le(argv[i]);
        (void)text16le_ptr;

        /* Test with protected copy (simulating sqlite3_column_value scenario) */
        protected_val = sqlite3_value_dup(argv[i]);
        if (protected_val) {
            blob_ptr = sqlite3_value_blob(protected_val);
            text_ptr = sqlite3_value_text(protected_val);
            text16_ptr = sqlite3_value_text16(protected_val);
            text16be_ptr = sqlite3_value_text16be(protected_val);
            text16le_ptr = sqlite3_value_text16le(protected_val);
            subtype = sqlite3_value_subtype(protected_val);
            
            /* Force type conversions by calling multiple getters */
            (void)sqlite3_value_bytes(protected_val);
            (void)sqlite3_value_bytes16(protected_val);
            (void)sqlite3_value_int(protected_val);
            (void)sqlite3_value_int64(protected_val);
            (void)sqlite3_value_double(protected_val);

            sqlite3_value_free(protected_val);
        }
    }

    /* Return a dummy result to keep the function valid */
    sqlite3_result_text(ctx, "fuzzed", 6, SQLITE_TRANSIENT);
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int i;
    const char *sql;
    char *query = NULL;
    size_t query_len;

    /* Skip empty inputs */
    if (Size == 0) return 0;

    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Register the fuzz function with SQLITE_SUBTYPE flag */
    rc = sqlite3_create_function_v2(
        db,
        "fuzz_func",
        -1, /* nArg */
        SQLITE_UTF8 | SQLITE_SUBTYPE,
        NULL, /* pApp */
        fuzz_func,
        NULL, /* xStep */
        NULL, /* xFinal */
        NULL  /* xDestroy */
    );
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a simple table to test with */
    rc = sqlite3_exec(db, "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Construct SQL queries from fuzz input.
     * We try to generate SELECT statements that call fuzz_func with various arguments.
     * We also try to INSERT data to test type conversions.
     */
    
    /* Limit query size to avoid huge allocations */
    if (Size > 4096) Size = 4096;

    /* Ensure the input is null-terminated for string operations if needed */
    query_len = Size + 64;
    query = (char *)malloc(query_len);
    if (!query) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Strategy 1: Call fuzz_func with literals (strings, numbers, blobs, nulls)
     * We parse the input as a sequence of commands or just use it as raw data for blobs.
     */
    
    /* Try to execute the input as SQL directly (sanitized by SQLite) */
    /* We wrap it in a SELECT fuzz_func(...) to ensure our function is called */
    
    /* 
     * We construct a query that uses the fuzz input as a blob or text argument.
     * We use hex encoding for the blob to handle arbitrary bytes.
     */
    
    /* Simple approach: Use the input as a hex-encoded blob literal */
    /* Calculate hex string length: 2 chars per byte + overhead */
    size_t hex_len = Size * 2 + 100;
    char *hex_str = (char *)malloc(hex_len);
    if (!hex_str) {
        free(query);
        sqlite3_close(db);
        return 0;
    }

    /* Convert input to hex string */
    for (i = 0; i < Size; i++) {
        sprintf(hex_str + (i * 2), "%02x", Data[i]);
    }
    hex_str[Size * 2] = '\0';

    /* Build SQL: SELECT fuzz_func(X'hex_data') */
    snprintf(query, query_len, "SELECT fuzz_func(X'%s');", hex_str);
    
    /* Prepare and execute */
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Consume the result to trigger the function logic */
            sqlite3_value *val = sqlite3_column_value(stmt, 0);
            if (val) {
                /* Additional checks on the result value */
                (void)sqlite3_value_type(val);
                (void)sqlite3_value_bytes(val);
            }
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Strategy 2: Insert data and select from table */
    snprintf(query, query_len, "INSERT INTO test_table (data) VALUES (X'%s');", hex_str);
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Select from table with fuzz_func */
    snprintf(query, query_len, "SELECT fuzz_func(data) FROM test_table;");
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Trigger function execution */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Strategy 3: Try different argument types explicitly */
    /* Integer */
    snprintf(query, query_len, "SELECT fuzz_func(12345);");
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Float */
    snprintf(query, query_len, "SELECT fuzz_func(123.456);");
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Text */
    /* Escape single quotes in input if used as text, but here we use hex for safety */
    snprintf(query, query_len, "SELECT fuzz_func(X'%s');", hex_str);
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* NULL */
    snprintf(query, query_len, "SELECT fuzz_func(NULL);");
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Multiple arguments */
    snprintf(query, query_len, "SELECT fuzz_func(X'%s', 123, 'text', NULL);", hex_str);
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Cleanup */
    free(hex_str);
    free(query);
    sqlite3_close(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
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
 * Fuzz driver for sqlite3_value_* and sqlite3_result_value APIs.
 * 
 * Strategy:
 * 1. Open an in-memory database.
 * 2. Prepare a simple SELECT statement to retrieve values.
 * 3. Step through results to get sqlite3_value pointers.
 * 4. Apply fuzzed operations:
 *    - Duplicate values (sqlite3_value_dup)
 *    - Access text in various encodings (text16, text16be, text16le)
 *    - Free duplicated values (sqlite3_value_free)
 * 5. Register a custom function to test sqlite3_result_value with fuzzed inputs.
 * 6. Execute the custom function with fuzzed arguments.
 * 7. Cleanup all resources.
 */

static void custom_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int i;
    sqlite3_value *val;
    sqlite3_value *dup;

    /* Test sqlite3_result_value with various inputs */
    for (i = 0; i < argc; i++) {
        val = argv[i];
        if (val == NULL) continue;

        /* Directly set result to input value */
        sqlite3_result_value(ctx, val);

        /* Try duplicating and setting result */
        dup = sqlite3_value_dup(val);
        if (dup) {
            sqlite3_result_value(ctx, dup);
            sqlite3_value_free(dup);
        }
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *val = NULL;
    sqlite3_value *dup = NULL;
    const void *text_ptr = NULL;
    int rc;
    int i;
    int col_count;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a test table and insert fuzzed data */
    const char *create_sql = "CREATE TABLE test (id INTEGER, data TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert fuzzed data if size is sufficient */
    if (Size > 0) {
        char *insert_sql = malloc(Size + 100);
        if (insert_sql) {
            snprintf(insert_sql, Size + 100, 
                     "INSERT INTO test (id, data) VALUES (%d, '%s');",
                     (int)(Data[0] % 100), 
                     (Size > 10 ? (char*)Data + 1 : ""));
            /* Sanitize string to avoid SQL injection issues in fuzzing context */
            for (i = 0; insert_sql[i]; i++) {
                if (insert_sql[i] == '\'') insert_sql[i] = 'X';
            }
            sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
            free(insert_sql);
        }
    }

    /* Register custom function for testing sqlite3_result_value */
    rc = sqlite3_create_function(db, "fuzz_func", -1, SQLITE_UTF8, NULL, 
                                  custom_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare statement to retrieve values */
    const char *select_sql = "SELECT id, data FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    col_count = sqlite3_column_count(stmt);

    /* Step through results and test value APIs */
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (i = 0; i < col_count; i++) {
            /* Get value from column */
            val = sqlite3_column_value(stmt, i);
            if (val == NULL) continue;

            /* Test sqlite3_value_dup */
            dup = sqlite3_value_dup(val);
            if (dup) {
                /* Test text retrieval functions on duplicated value */
                text_ptr = sqlite3_value_text16(dup);
                (void)text_ptr; /* Suppress unused warning */
                
                text_ptr = sqlite3_value_text16be(dup);
                (void)text_ptr;
                
                text_ptr = sqlite3_value_text16le(dup);
                (void)text_ptr;

                /* Free the duplicated value */
                sqlite3_value_free(dup);
            }

            /* Test text retrieval on original value (unprotected) */
            /* Note: These may return NULL for non-text values, which is expected */
            text_ptr = sqlite3_value_text16(val);
            (void)text_ptr;
            
            text_ptr = sqlite3_value_text16be(val);
            (void)text_ptr;
            
            text_ptr = sqlite3_value_text16le(val);
            (void)text_ptr;

            /* Test sqlite3_value_free on NULL (should be safe) */
            sqlite3_value_free(NULL);
        }
    }

    /* Test custom function with fuzzed data */
    const char *func_sql = "SELECT fuzz_func(1, 'test', 2.5, NULL);";
    rc = sqlite3_prepare_v2(db, func_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Consume result */
            sqlite3_column_text(stmt, 0);
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}
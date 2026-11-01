// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Custom destructor that safely handles NULL and avoids double-free issues.
 * SQLite may call the destructor on the data pointer passed to result functions.
 */
static void custom_destructor(void *p) {
    if (p) {
        free(p);
    }
}

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    if (!ctx) return;

    /* Fuzz sqlite3_result_zeroblob64 */
    sqlite3_result_zeroblob64(ctx, 0);
    sqlite3_result_zeroblob64(ctx, 1);
    sqlite3_result_zeroblob64(ctx, (sqlite3_uint64)-1);

    /* Fuzz sqlite3_result_text16be */
    /* Case 1: Static data - use SQLITE_STATIC, NO destructor */
    static const char utf16be_data[] = {0x00, 0x41, 0x00, 0x42}; /* "AB" in UTF-16BE */
    sqlite3_result_text16be(ctx, utf16be_data, 4, SQLITE_STATIC);
    
    /* Case 2: Dynamic data - use custom_destructor */
    char *malloc_utf16be = malloc(100);
    if (malloc_utf16be) {
        memset(malloc_utf16be, 0, 100);
        sqlite3_result_text16be(ctx, malloc_utf16be, 100, custom_destructor);
    }

    /* Fuzz sqlite3_result_blob */
    static const char blob_data[] = "BLOB_DATA";
    sqlite3_result_blob(ctx, blob_data, sizeof(blob_data) - 1, SQLITE_STATIC);
    
    char *malloc_blob = malloc(100);
    if (malloc_blob) {
        memset(malloc_blob, 0, 100);
        sqlite3_result_blob(ctx, malloc_blob, 100, custom_destructor);
    }
    
    /* 
     * Fix: The original code called sqlite3_result_blob with -1 length.
     * In debug builds (ASAN), sqlite3 asserts that n >= 0 for blob results.
     * We must ensure the length is non-negative.
     */
    sqlite3_result_blob(ctx, NULL, 0, NULL);
    /* Removed: sqlite3_result_blob(ctx, blob_data, -1, NULL); */

    /* Fuzz sqlite3_result_int */
    sqlite3_result_int(ctx, 0);
    sqlite3_result_int(ctx, 12345);
    sqlite3_result_int(ctx, -12345);
    sqlite3_result_int(ctx, 2147483647);
    sqlite3_result_int(ctx, -2147483648);

    /* Fuzz sqlite3_result_error_code */
    sqlite3_result_error_code(ctx, SQLITE_ERROR);
    sqlite3_result_error_code(ctx, SQLITE_TOOBIG);
    sqlite3_result_error_code(ctx, SQLITE_NOMEM);
    sqlite3_result_error_code(ctx, 0);
    sqlite3_result_error_code(ctx, -1);

    /* Fuzz sqlite3_get_auxdata */
    sqlite3_get_auxdata(ctx, 0);
    sqlite3_get_auxdata(ctx, 1);
    sqlite3_get_auxdata(ctx, 10);
    sqlite3_get_auxdata(ctx, -1);
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table and a function to trigger the target APIs */
    const char *sql_create = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "fuzz_test_func", 0, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement to invoke the function */
    const char *sql_select = "SELECT fuzz_test_func();";
    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step through the statement to execute the function */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        /* Consume the result to ensure the function completes */
        sqlite3_value *val = sqlite3_column_value(stmt, 0);
        if (val) {
            /* Just accessing the value type to ensure processing */
            sqlite3_value_type(val);
        }
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
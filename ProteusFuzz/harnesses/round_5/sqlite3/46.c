// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:92889:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:92871:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
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
#include <stdio.h>

/*
 * Custom destructor for fuzzing tests
 */
static void custom_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/*
 * Custom SQL function implementation that uses the target APIs
 */
static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    const uint8_t *data = sqlite3_value_blob(argv[0]);
    int size = sqlite3_value_bytes(argv[0]);
    
    if (!ctx || size < 0) {
        return;
    }

    /* Ensure size is even for UTF-16 functions */
    int even_size = (size & ~1);
    if (even_size < 0) even_size = 0;

    /* Fuzz sqlite3_result_text16be */
    if (even_size > 0) {
        sqlite3_result_text16be(ctx, data, even_size, custom_destructor);
    }

    /* Fuzz sqlite3_result_text16le */
    if (even_size > 0) {
        sqlite3_result_text16le(ctx, data, even_size, custom_destructor);
    }

    /* Fuzz sqlite3_set_auxdata */
    if (argc > 1) {
        void *aux_data = malloc(size > 0 ? size : 1);
        if (aux_data) {
            if (size > 0 && data) {
                memcpy(aux_data, data, size);
            }
            sqlite3_set_auxdata(ctx, 0, aux_data, custom_destructor);
        }
    }

    /* Fuzz sqlite3_result_blob */
    if (size > 0) {
        sqlite3_result_blob(ctx, data, size, custom_destructor);
    }

    /* Fuzz sqlite3_result_text */
    if (size > 0) {
        /* Create a temporary null-terminated string for text result */
        char *text_buf = malloc(size + 1);
        if (text_buf) {
            memcpy(text_buf, data, size);
            text_buf[size] = '\0';
            sqlite3_result_text(ctx, text_buf, size, custom_destructor);
        }
    }

    /* Fuzz sqlite3_result_text16 */
    if (even_size > 0) {
        sqlite3_result_text16(ctx, data, even_size, custom_destructor);
    }
}

/*
 * Fuzzing entry point
 */
int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a test table */
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, test_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement to call the custom function with fuzzed input */
    const char *sql = "SELECT fuzz_func(?) FROM test;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzzed data as a BLOB */
    sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    (void)rc; /* Ignore return value for fuzzing purposes */

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
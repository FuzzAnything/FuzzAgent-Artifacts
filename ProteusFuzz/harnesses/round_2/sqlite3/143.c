// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:92889:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:92871:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static helper to create a dummy database and prepare a statement for fuzzing */
static int setup_fuzzing_environment(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return -1;
    }

    /* Create a table to ensure the database is valid */
    rc = sqlite3_exec(*db, "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return -1;
    }

    /* Prepare a simple statement to execute */
    const char *sql = "SELECT my_func(?)";
    rc = sqlite3_prepare_v2(*db, sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return -1;
    }

    return 0;
}

/* Static destructor for auxdata and blob/text data */
static void dummy_destructor(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

/* Custom SQL function implementation to test target APIs */
static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (!ctx) return;

    /* Test sqlite3_set_auxdata with various inputs */
    void *aux_data = NULL;
    if (argc > 0) {
        int len = sqlite3_value_bytes(argv[0]);
        if (len > 0 && len < 10000) {
            aux_data = malloc(len);
            if (aux_data) {
                memcpy(aux_data, sqlite3_value_blob(argv[0]), len);
                sqlite3_set_auxdata(ctx, 0, aux_data, dummy_destructor);
            }
        }
    }

    /* Test sqlite3_result_blob */
    if (argc > 1) {
        const void *blob_data = sqlite3_value_blob(argv[1]);
        int blob_len = sqlite3_value_bytes(argv[1]);
        /* Fuzz with negative length, zero length, and valid length */
        if (blob_len < 0) blob_len = -1; 
        sqlite3_result_blob(ctx, blob_data, blob_len, blob_len > 0 ? dummy_destructor : NULL);
    }

    /* Test sqlite3_result_text16be */
    if (argc > 2) {
        const void *utf16be_data = sqlite3_value_blob(argv[2]);
        int utf16be_len = sqlite3_value_bytes(argv[2]);
        /* Ensure even length for UTF-16 */
        if (utf16be_len % 2 != 0) utf16be_len--;
        sqlite3_result_text16be(ctx, utf16be_data, utf16be_len, NULL);
    }

    /* Test sqlite3_result_text16le */
    if (argc > 3) {
        const void *utf16le_data = sqlite3_value_blob(argv[3]);
        int utf16le_len = sqlite3_value_bytes(argv[3]);
        if (utf16le_len % 2 != 0) utf16le_len--;
        sqlite3_result_text16le(ctx, utf16le_data, utf16le_len, NULL);
    }

    /* Test sqlite3_result_text16 */
    if (argc > 4) {
        const void *utf16_data = sqlite3_value_blob(argv[4]);
        int utf16_len = sqlite3_value_bytes(argv[4]);
        if (utf16_len % 2 != 0) utf16_len--;
        sqlite3_result_text16(ctx, utf16_data, utf16_len, NULL);
    }

    /* Test sqlite3_result_text */
    if (argc > 5) {
        const char *text_data = (const char *)sqlite3_value_text(argv[5]);
        int text_len = sqlite3_value_bytes(argv[5]);
        /* Fuzz with -1 for null-terminated or specific length */
        if (text_len < 0) text_len = -1;
        sqlite3_result_text(ctx, text_data, text_len, NULL);
    }
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Setup environment */
    if (setup_fuzzing_environment(&db, &stmt) != 0) {
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "my_func", -1, SQLITE_UTF8, NULL, test_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Use input data to bind parameters */
    int argc = (Size > 0) ? (Size % 6) + 1 : 1;
    
    for (int i = 0; i < argc; i++) {
        int offset = (i * (Size / argc)) % Size;
        int len = (Size / argc) > 0 ? (Size / argc) : 1;
        if (offset + len > Size) len = Size - offset;
        
        /* Bind blob data */
        rc = sqlite3_bind_blob(stmt, i + 1, Data + offset, len, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) break;
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    /* We don't care about the result, just that it doesn't crash */

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
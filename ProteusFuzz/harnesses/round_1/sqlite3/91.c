// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:92871:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:92889:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
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

static void dummy_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/*
 * Custom SQL function implementation that uses the fuzzed input
 * to call the target APIs.
 */
static void fuzz_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    /* We cannot access the raw fuzz data directly here because it's not passed
       to the SQL function. Instead, we rely on the fuzzer setting up global
       or static state, or we pass data via the function arguments if possible.
       However, since we need to fuzz the APIs directly, we will use a different approach:
       We will create a prepared statement that calls our function, and the function
       will use static buffers or memory allocated based on the fuzzer's input.
       But the fuzzer input is in LLVMFuzzerTestOneInput.
       
       Better approach: The fuzzer will call these APIs directly inside LLVMFuzzerTestOneInput
       by creating a context. But we can't easily create a valid sqlite3_context without
       a running VM.
       
       Alternative: We create a custom function that, when called, uses the fuzz input.
       But the fuzz input is not available inside the function callback unless we store it globally.
       
       Let's use a global buffer to store the fuzz input for the function to use.
    */
    static const uint8_t *g_fuzz_data = NULL;
    static size_t g_fuzz_size = 0;
    
    if (!g_fuzz_data) {
        /* No data set yet, return NULL */
        sqlite3_result_null(ctx);
        return;
    }

    /* Randomly choose which API to test based on the first byte of input */
    uint8_t choice = g_fuzz_data[0] % 6;
    
    size_t len = g_fuzz_size > 1 ? (g_fuzz_size - 1) : 0;
    const void *data = g_fuzz_data + 1;
    
    /* Ensure length is safe */
    if (len > 1024 * 1024) len = 1024 * 1024; /* Cap at 1MB */

    switch (choice) {
        case 0:
            /* sqlite3_result_text16 */
            /* Ensure even length for UTF-16 */
            if (len % 2 != 0) len--;
            sqlite3_result_text16(ctx, data, len, g_fuzz_size > 2 ? dummy_destructor : SQLITE_TRANSIENT);
            break;
        case 1:
            /* sqlite3_result_text16be */
            if (len % 2 != 0) len--;
            sqlite3_result_text16be(ctx, data, len, g_fuzz_size > 2 ? dummy_destructor : SQLITE_TRANSIENT);
            break;
        case 2:
            /* sqlite3_result_text16le */
            if (len % 2 != 0) len--;
            sqlite3_result_text16le(ctx, data, len, g_fuzz_size > 2 ? dummy_destructor : SQLITE_TRANSIENT);
            break;
        case 3:
            /* sqlite3_result_text */
            sqlite3_result_text(ctx, (const char *)data, len, g_fuzz_size > 2 ? dummy_destructor : SQLITE_TRANSIENT);
            break;
        case 4:
            /* sqlite3_result_blob */
            sqlite3_result_blob(ctx, data, len, g_fuzz_size > 2 ? dummy_destructor : SQLITE_TRANSIENT);
            break;
        case 5:
            /* sqlite3_set_auxdata */
            /* We need an argument index. Use first byte of data (minus 1) modulo argc if available, else 0 */
            int idx = 0;
            if (argc > 0) {
                idx = (g_fuzz_size > 1 ? g_fuzz_data[1] % argc : 0);
            }
            /* Allocate some dummy data for auxdata */
            void *aux = malloc(len > 0 ? len : 1);
            if (aux && len > 0) {
                memcpy(aux, data, len);
            }
            sqlite3_set_auxdata(ctx, idx, aux, g_fuzz_size > 2 ? dummy_destructor : SQLITE_TRANSIENT);
            break;
    }
}

/* Global variables to pass fuzz data to the SQL function */
static const uint8_t *g_fuzz_input_data = NULL;
static size_t g_fuzz_input_size = 0;

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Set global fuzz data for the function to use */
    g_fuzz_input_data = Data;
    g_fuzz_input_size = Size;

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, NULL, fuzz_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement that calls the function */
    const char *sql = "SELECT fuzz_func();";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step through the statement to trigger the function */
    rc = sqlite3_step(stmt);
    /* We don't care about the result, just that it didn't crash */
    (void)rc;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    /* Reset global state */
    g_fuzz_input_data = NULL;
    g_fuzz_input_size = 0;

    return 0;
}
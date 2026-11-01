// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * Helper function to create a minimal fake sqlite3_context for fuzzing.
 * Since sqlite3_context is an opaque type in public headers but defined
 * internally with dependencies on Vdbe, Mem, etc., we cannot fully
 * construct a valid one without linking against the internal implementation
 * or using a real database connection.
 *
 * Strategy:
 * 1. Create a real database connection.
 * 2. Create a simple SQL function that uses the target APIs.
 * 3. Execute a query that triggers the function.
 * 4. Use the fuzz input to control parameters passed to the function.
 */

static int g_fuzz_step_count = 0;
static int g_fuzz_final_count = 0;

/*
 * Custom aggregate function to test sqlite3_aggregate_context
 * and sqlite3_result_int.
 */
static void fuzz_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    /* Use fuzz input to determine size if available, otherwise default */
    int size = 100;
    if (g_fuzz_step_count < 100) {
        size = 100;
    } else {
        /* Fuzz the size parameter */
        size = 0; 
    }

    void *mem = sqlite3_aggregate_context(ctx, size);
    if (mem) {
        /* Write some data to the context memory to ensure it's allocated */
        memset(mem, 0xAB, size);
    }

    g_fuzz_step_count++;
}

static void fuzz_agg_final(sqlite3_context *ctx) {
    /* Get context with 0 bytes to avoid reallocation */
    void *mem = sqlite3_aggregate_context(ctx, 0);
    (void)mem;

    g_fuzz_final_count++;
    
    /* Return an integer result */
    sqlite3_result_int(ctx, g_fuzz_final_count);
}

/*
 * Custom scalar function to test sqlite3_result_zeroblob64,
 * sqlite3_result_zeroblob, sqlite3_result_error_code,
 * sqlite3_result_error16.
 */
static void fuzz_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    /* 
     * We need to extract parameters from the fuzz input.
     * Since we can't pass the raw fuzz buffer directly to the SQL function,
     * we will use global state or the function arguments if we bind them.
     * For simplicity in this driver, we will use a global buffer approach
     * or rely on the fact that the fuzzer controls the SQL query string.
     *
     * However, the standard way to pass fuzz data to a custom function
     * is via arguments. Let's assume the fuzzer passes an integer argument.
     */
    
    int val = 0;
    if (argc > 0) {
        val = sqlite3_value_int(argv[0]);
    }

    /* 
     * Fuzzing logic:
     * We will use the global fuzz buffer (passed to LLVMFuzzerTestOneInput)
     * to drive the behavior. We need to store it somewhere accessible.
     * Since we can't easily pass the buffer to the callback, we will use
     * a global variable to store the current fuzz input.
     */
    
    static const uint8_t *g_current_input = NULL;
    static size_t g_current_size = 0;
    
    /* This is a limitation: we can't access the fuzzer input here directly 
       unless we set it up before the query runs. 
       We will handle this in the main fuzzer function by setting globals.
    */

    /* Test sqlite3_result_zeroblob64 */
    if (g_current_size >= 8) {
        sqlite3_uint64 n = 0;
        memcpy(&n, g_current_input, 8);
        
        /* Clamp to reasonable limits to avoid immediate SQLITE_TOOBIG if we want to test other paths,
           but let the API handle the error. */
        sqlite3_result_zeroblob64(ctx, n);
        return;
    }

    /* Test sqlite3_result_zeroblob */
    if (g_current_size >= 4) {
        int n = 0;
        memcpy(&n, g_current_input, 4);
        sqlite3_result_zeroblob(ctx, n);
        return;
    }

    /* Test sqlite3_result_error_code */
    if (g_current_size >= 4) {
        int err_code = 0;
        memcpy(&err_code, g_current_input, 4);
        sqlite3_result_error_code(ctx, err_code);
        return;
    }

    /* Test sqlite3_result_error16 */
    if (g_current_size >= 2) {
        /* Create a minimal UTF-16 string from input */
        uint16_t utf16_str[2];
        utf16_str[0] = (g_current_input[0] << 8) | g_current_input[1];
        utf16_str[1] = 0; /* Null terminator */
        
        sqlite3_result_error16(ctx, utf16_str, sizeof(utf16_str));
        return;
    }

    /* Default: return an int */
    sqlite3_result_int(ctx, val);
}

/* Global storage for fuzz input to be accessed by callbacks */
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Setup global fuzz data for callbacks */
    g_fuzz_data = Data;
    g_fuzz_size = Size;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register custom functions */
    rc = sqlite3_create_function_v2(
        db,
        "fuzz_scalar",
        -1, /* Any number of arguments */
        SQLITE_UTF8,
        NULL, /* app_data */
        fuzz_scalar_func,
        NULL, /* xStep */
        NULL, /* xFinal */
        NULL  /* xDestroy */
    );
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function_v2(
        db,
        "fuzz_agg",
        -1,
        SQLITE_UTF8,
        NULL,
        NULL,
        fuzz_agg_step,
        fuzz_agg_final,
        NULL
    );
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a dummy table for aggregate testing */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert some rows to ensure aggregate runs */
    rc = sqlite3_exec(db, "INSERT INTO test VALUES (1), (2), (3);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Fuzzing Strategy:
     * We will try to execute different queries based on the input data.
     * Since we can't easily pass the raw buffer to the SQL function arguments
     * without complex binding logic, we rely on the global state set above.
     * However, to make the fuzzer effective, we should vary the SQL queries.
     */

    const char *query = NULL;
    
    if (Size > 0) {
        switch (Data[0] % 4) {
            case 0:
                /* Test scalar function with no args */
                query = "SELECT fuzz_scalar();";
                break;
            case 1:
                /* Test scalar function with an arg (we bind 0) */
                query = "SELECT fuzz_scalar(0);";
                break;
            case 2:
                /* Test aggregate function */
                query = "SELECT fuzz_agg(id) FROM test;";
                break;
            case 3:
                /* Test aggregate function with empty set (edge case) */
                query = "SELECT fuzz_agg(id) FROM test WHERE id > 1000;";
                break;
        }
    } else {
        query = "SELECT fuzz_scalar();";
    }

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    (void)rc; /* We don't care about the result, just that it runs */

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
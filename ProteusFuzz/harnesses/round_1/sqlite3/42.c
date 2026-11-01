// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* Global variables to pass data to the callback */
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

/*
 * Custom destructor for text/blob results to avoid memory leaks in fuzzing.
 */
static void fuzzer_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/*
 * Scalar function implementation to test result APIs.
 * Note: We do NOT call sqlite3_aggregate_context here because this is a scalar function.
 * Calling it in a scalar context can trigger assertions in debug builds if the function
 * is not registered as an aggregate.
 */
static void fuzzer_scalar_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!context) return;

    if (!g_fuzz_data || g_fuzz_size == 0) {
        sqlite3_result_int(context, 0);
        return;
    }

    /*
     * Target 1: sqlite3_result_int
     */
    int val_int = 0;
    if (g_fuzz_size >= 4) {
        memcpy(&val_int, g_fuzz_data, sizeof(int));
    }
    sqlite3_result_int(context, val_int);

    /*
     * Target 2: sqlite3_result_text
     */
    if (g_fuzz_size > 0) {
        /* Ensure we don't allocate huge amounts of memory */
        size_t safe_size = g_fuzz_size;
        if (safe_size > 1024) safe_size = 1024;
        
        char *safe_str = malloc(safe_size + 1);
        if (safe_str) {
            memcpy(safe_str, g_fuzz_data, safe_size);
            safe_str[safe_size] = '\0';
            sqlite3_result_text(context, safe_str, (int)safe_size, fuzzer_destructor);
        }
    }

    /*
     * Target 4: sqlite3_result_error_code
     */
    if (g_fuzz_size >= 2) {
        short err_code = 0;
        memcpy(&err_code, g_fuzz_data, sizeof(short));
        if (err_code != 0) {
            sqlite3_result_error_code(context, err_code);
        }
    }

    /*
     * Target 5: sqlite3_result_int64
     */
    sqlite3_int64 val_int64 = 0;
    if (g_fuzz_size >= 8) {
        memcpy(&val_int64, g_fuzz_data, sizeof(sqlite3_int64));
    }
    sqlite3_result_int64(context, val_int64);

    /*
     * Target 6: sqlite3_result_blob
     */
    if (g_fuzz_size > 0) {
        size_t safe_size = g_fuzz_size;
        if (safe_size > 1024) safe_size = 1024;

        void *blob_data = malloc(safe_size);
        if (blob_data) {
            memcpy(blob_data, g_fuzz_data, safe_size);
            sqlite3_result_blob(context, blob_data, (int)safe_size, fuzzer_destructor);
        }
    }
}

/*
 * Aggregate function step implementation.
 * This is where sqlite3_aggregate_context is valid.
 */
static void fuzzer_agg_step(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    if (!context) return;

    /* Target 3: sqlite3_aggregate_context */
    int size = 10;
    if (g_fuzz_size > 0) {
        size = (int)g_fuzz_size;
        if (size > 1000) size = 1000;
        if (size <= 0) size = 10;
    }
    
    void *ctx_mem = sqlite3_aggregate_context(context, size);
    if (ctx_mem) {
        memset(ctx_mem, 0xFF, size);
    }
}

/*
 * Aggregate function final implementation.
 */
static void fuzzer_agg_final(sqlite3_context *context) {
    if (!context) return;

    /* Final step: get context again with size 0 as per documentation */
    void *ctx_mem = sqlite3_aggregate_context(context, 0);
    if (ctx_mem) {
        /* Use the memory to ensure it's valid */
        volatile int dummy = 0;
        if (ctx_mem) dummy = 1;
        (void)dummy;
    }

    /* Set a result */
    sqlite3_result_int(context, 42);
}

/*
 * Fuzzer entry point.
 */
int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Step 1: Prepare the environment */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Update global data for the function to use */
    g_fuzz_data = Data;
    g_fuzz_size = Size;

    /* Register the scalar function */
    rc = sqlite3_create_function(db, "fuzz_func", -1, SQLITE_UTF8, NULL, fuzzer_scalar_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the aggregate function */
    rc = sqlite3_create_function(db, "fuzz_agg", -1, SQLITE_UTF8, NULL, NULL, fuzzer_agg_step, fuzzer_agg_final);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Execute a query to trigger the scalar function */
    const char *sql_scalar = "SELECT fuzz_func();";
    rc = sqlite3_prepare_v2(db, sql_scalar, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Execute a query to trigger the aggregate function */
    const char *sql_agg = "SELECT fuzz_agg(1) FROM (SELECT 1 UNION SELECT 2 UNION SELECT 3);";
    rc = sqlite3_prepare_v2(db, sql_agg, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Step 2: Cleanup */
    sqlite3_close(db);

    return 0;
}
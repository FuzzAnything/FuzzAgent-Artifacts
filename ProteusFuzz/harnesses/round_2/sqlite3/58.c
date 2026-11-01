// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:95598:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:92610:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Helper function to create a valid sqlite3_value from a string input.
 * We use a custom SQL function to generate a protected sqlite3_value 
 * that is safe to use with the target APIs.
 */
static int custom_func_called = 0;

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* 
     * We don't actually return a value here, but the existence of the function
     * allows us to potentially trigger code paths if we were calling it.
     * For this specific fuzzing task, we rely on binding to create values.
     * However, the previous approach failed because sqlite3_column_value returns
     * an unprotected value, and the target APIs (like sqlite3_value_int) require
     * a protected value (which holds a mutex).
     * 
     * To fix the ASan error (assertion failure on mutex), we must ensure the value
     * is "protected". The only way to get a protected value from the outside
     * without running a full statement step in a callback is to use sqlite3_value_dup
     * on a value obtained from a callback context, OR simply ensure we are in a 
     * context where the value is valid.
     * 
     * Actually, the documentation says: "The sqlite3_value objects that are passed 
     * as parameters into the implementation of application-defined SQL functions 
     * are protected."
     * 
     * So, the correct approach is to define a custom function, register it, 
     * execute a statement that calls it, and inside the callback, fuzz the arguments.
     * But the fuzzer input is external. We can't easily pass the fuzzer input 
     * directly into the callback arguments unless we bind them.
     * 
     * Let's try a different approach: 
     * 1. Create a custom function.
     * 2. Prepare a statement that calls this function with a bound parameter.
     * 3. Step the statement.
     * 4. The custom function will be called with the bound value as a protected sqlite3_value*.
     * 5. Inside the custom function, we call the target APIs.
     * 
     * This ensures the value is protected and valid.
     */
}

/* Global pointer to store the value to be fuzzed inside the callback */
static sqlite3_value *g_fuzz_value = NULL;
static int g_fuzz_done = 0;

static void fuzz_callback(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    
    /* 
     * We need to get the value that was bound to the function argument.
     * The 'argv' array contains the protected values.
     * We will use the first argument (argv[0]) as our fuzz target.
     */
    if (argc > 0) {
        g_fuzz_value = argv[0];
        
        /* Call the target APIs on the protected value */
        (void)sqlite3_value_numeric_type(g_fuzz_value);
        (void)sqlite3_value_type(g_fuzz_value);
        (void)sqlite3_value_nochange(g_fuzz_value);
        (void)sqlite3_value_bytes16(g_fuzz_value);
        (void)sqlite3_value_frombind(g_fuzz_value);
        (void)sqlite3_value_int(g_fuzz_value);
        
        g_fuzz_done = 1;
    }
    
    /* Return NULL to avoid further processing issues */
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql = "SELECT fuzz_test(?1);";
    
    /* Reset global state */
    g_fuzz_value = NULL;
    g_fuzz_done = 0;

    /* Initialize in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "fuzz_test", -1, SQLITE_UTF8, NULL, fuzz_callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzz input as a blob to the first parameter */
    if (Size > 0) {
        rc = sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    } else {
        /* If size is 0, bind a NULL or empty blob to ensure valid state */
        rc = sqlite3_bind_null(stmt, 1);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    }

    /* Execute the statement to trigger the callback */
    rc = sqlite3_step(stmt);
    /* We don't care about the result of step, just that the callback ran */
    
    /* 
     * Additional test: Try with an integer value to exercise different type paths
     * We need to re-bind and re-step for a new value type.
     */
    if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
        int int_val = 0;
        if (Size >= 4) {
            int_val = *(const int32_t *)Data;
        } else if (Size > 0) {
            int_val = Data[0];
        }
        
        /* Reset for next test */
        g_fuzz_value = NULL;
        g_fuzz_done = 0;
        
        /* Reset statement */
        sqlite3_reset(stmt);
        
        /* Bind as integer */
        rc = sqlite3_bind_int(stmt, 1, int_val);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt);
        }
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
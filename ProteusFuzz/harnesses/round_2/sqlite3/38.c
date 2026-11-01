// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
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
 * Fuzz driver for sqlite3_value_bytes -> sqlite3_value_blob -> sqlite3_result_int
 *
 * This driver simulates a custom SQL function context to exercise the target APIs.
 * Since these APIs are designed to be called from within a user-defined SQL function,
 * we must construct a valid sqlite3_context and sqlite3_value environment.
 *
 * Note: Directly manipulating internal structures (like sqlite3_context) is generally
 * unsafe and relies on internal headers. However, for fuzzing purposes where we
 * want to test the API logic, we will use sqlite3_create_function to register a
 * test function, then invoke it via SQL execution. This ensures all internal state
 * is properly initialized by SQLite itself.
 */

/* Global database handle for the fuzzer */
static sqlite3 *g_db = NULL;

/*
 * The custom SQL function that will be invoked during fuzzing.
 * This function receives the fuzzed data as arguments and executes the target API chain.
 */
static void fuzz_target_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (!ctx || argc < 1) {
        /* If context or args are invalid, return safely */
        sqlite3_result_int(ctx, 0);
        return;
    }

    /*
     * We will use the first argument (argv[0]) as the source for our testing.
     * The fuzzer will provide various types of data (BLOB, TEXT, NULL, etc.)
     * to trigger different code paths in the target APIs.
     */
    sqlite3_value *val = argv[0];

    /*
     * Step 1: Invoke sqlite3_value_bytes
     * This returns the byte length of the value.
     */
    int bytes_len = sqlite3_value_bytes(val);

    /*
     * Step 2: Invoke sqlite3_value_blob
     * This retrieves the BLOB data. It might convert non-BLOB types to BLOB.
     * The pointer is valid only within this call scope.
     */
    const void *blob_data = sqlite3_value_blob(val);

    /*
     * Optional: Use the data to ensure memory is accessed (if not NULL)
     * This helps the fuzzer detect memory corruption or invalid pointers.
     */
    if (blob_data != NULL && bytes_len > 0) {
        /* Read a byte to ensure the pointer is valid and accessible */
        volatile const unsigned char *uc = (const unsigned char *)blob_data;
        volatile unsigned char dummy = uc[0];
        (void)dummy;
    }

    /*
     * Step 3: Invoke sqlite3_result_int
     * We set the result based on the byte length or a fixed value to explore states.
     * Using bytes_len directly might be 0 for NULL, so we add a variation.
     */
    int result_val = bytes_len;
    
    /* Add some variation based on blob existence to explore more states */
    if (blob_data != NULL) {
        result_val += 1;
    }

    sqlite3_result_int(ctx, result_val);
}

/*
 * Entry point for the fuzzer
 */
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    /*
     * 1. Prepare the environment
     * Initialize the database connection if not already done.
     * We use an in-memory database for speed and isolation.
     */
    if (g_db == NULL) {
        int rc = sqlite3_open(":memory:", &g_db);
        if (rc != SQLITE_OK) {
            sqlite3_close(g_db);
            g_db = NULL;
            return 0;
        }
    }

    /*
     * Register the custom function if not already registered.
     * We use a unique name to avoid conflicts, though in a single process it's fine.
     */
    static int registered = 0;
    if (!registered) {
        sqlite3_create_function(g_db, "fuzz_target_func", 1, SQLITE_UTF8, NULL, fuzz_target_function, NULL, NULL);
        registered = 1;
    }

    /*
     * Prepare a dummy file if needed (not strictly needed for :memory: but good practice per instructions)
     * We write the fuzzed data to a dummy file to ensure file I/O paths are touched if the DB logic uses them.
     */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /*
     * 2. Construct the SQL statement
     * We will bind the fuzzed input data as a BLOB or TEXT to the function argument.
     * To explore more states, we try different SQL patterns.
     */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT fuzz_target_func(?)";
    int rc;

    rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return 0;
    }

    /*
     * Bind the fuzzed data.
     * We try binding as BLOB first.
     */
    sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);

    /*
     * Execute the statement to trigger the custom function.
     */
    rc = sqlite3_step(stmt);
    
    /*
     * If execution fails, we might want to try a different binding type to explore states.
     * However, for a simple driver, one execution per input is standard.
     * We can also try binding as TEXT to see if the conversion logic in sqlite3_value_blob is hit.
     */
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        /* Reset and try binding as TEXT */
        sqlite3_reset(stmt);
        sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);
    }

    /*
     * 3. Cleanup
     * Finalize the statement.
     */
    sqlite3_finalize(stmt);

    /*
     * Handle exceptions: If the database is corrupted or in a bad state, close and reopen.
     */
    if (sqlite3_errcode(g_db) != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
    }

    return 0;
}
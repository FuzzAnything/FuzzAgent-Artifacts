// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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
 * Static helper function to act as a busy handler.
 * Returns 0 to stop retrying immediately, or 1 to retry.
 * We use the user_data to decide behavior based on fuzz input.
 */
static int busy_handler_callback(void *arg, int count) {
    int *ptr = (int *)arg;
    if (ptr) {
        /* If the pointer is valid and the value is non-zero, retry. */
        if (*ptr != 0) {
            return 1;
        }
    }
    return 0;
}

/*
 * Static helper function to act as a custom SQL scalar function.
 * It does nothing but return NULL to avoid crashes.
 */
static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;
    sqlite3_result_null(context);
}

/*
 * Static helper function to act as a callback for sqlite3_exec.
 * It simply returns 0 to continue execution.
 */
static int exec_callback(void *arg, int argc, char **argv, char **colNames) {
    (void)arg;
    (void)argc;
    (void)argv;
    (void)colNames;
    return 0;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *errmsg = NULL;
    int busy_arg_val = 0;
    
    /* 
     * Prepare environment: 
     * We need a filename. The task specifies using "./dummy_file".
     * We also need to derive flags and other parameters from the fuzz data if possible.
     */
    const char *filename = "./dummy_file";
    
    /* 
     * Step 1: sqlite3_open_v2
     * We use the first byte of input to determine flags if Size > 0.
     * Default to SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE if data is empty.
     */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        /* Use low bits of first byte to vary flags slightly, but keep it safe */
        flags |= (Data[0] & 0x07); 
    }
    
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        /* If we cannot open the database, we cannot proceed with other calls.
         * We clean up and return early.
         */
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Step 2: sqlite3_close
     * The task requires invoking sqlite3_close immediately after open.
     * This will close the connection. Subsequent calls on 'db' will likely fail or be no-ops.
     * However, the task explicitly asks for this order.
     */
    rc = sqlite3_close(db);
    /* db is now invalid/closed. We must re-open to test the remaining functions effectively,
     * or the remaining calls will just test error handling on a closed handle.
     * To explore more program states as per instruction #3, we will re-open.
     */
    if (rc != SQLITE_OK) {
        /* If close failed (e.g. SQLITE_BUSY), we still try to proceed or exit. */
        sqlite3_close(db);
        return 0;
    }
    
    /* Re-open to allow testing of create_function, busy_handler, and exec */
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Step 3: sqlite3_create_function
     * We register a dummy function.
     * We use fuzz data to vary the function name length and content safely.
     */
    const char *func_name = "fuzz_func";
    int n_arg = 0;
    int text_rep = SQLITE_UTF8;
    
    if (Size > 1) {
        n_arg = Data[1] % 10; /* Limit arguments to 0-9 */
    }
    
    /* 
     * We pass NULL for pApp and use our static dummy_func.
     * xStep and xFinal are NULL for a scalar function.
     */
    rc = sqlite3_create_function(db, func_name, n_arg, text_rep, NULL, dummy_func, NULL, NULL);
    /* Ignore rc here, we just want to exercise the code path. */

    /* 
     * Step 4: sqlite3_busy_handler
     * Register the busy handler.
     * We use a static integer to act as user_data, derived from input if available.
     */
    if (Size > 2) {
        busy_arg_val = Data[2];
    }
    
    rc = sqlite3_busy_handler(db, busy_handler_callback, &busy_arg_val);
    /* Ignore rc, just exercising the API. */

    /* 
     * Step 5: sqlite3_exec
     * Execute a simple SQL statement.
     * We construct a safe SQL string.
     */
    const char *sql = "SELECT 1;";
    
    /* 
     * We pass NULL for callback to ignore results, or use our static callback.
     * We use our static callback to exercise that path.
     */
    rc = sqlite3_exec(db, sql, exec_callback, NULL, &errmsg);
    
    /* 
     * Cleanup
     * If sqlite3_exec returned an error and errmsg is not NULL, free it.
     */
    if (errmsg != NULL) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* Close the database connection */
    sqlite3_close(db);
    
    return 0;
}
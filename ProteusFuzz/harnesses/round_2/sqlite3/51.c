// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
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
 * Static helper function to create a custom SQL function for testing
 * sqlite3_context_db_handle.
 */
static void dummy_sql_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_int(ctx, 0);
}

/*
 * Fuzzing entry point.
 * Data: Fuzz input buffer
 * Size: Size of the fuzz input buffer
 */
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int autocommit;
    sqlite3 *db_from_stmt;
    sqlite3_stmt *next_stmt;
    const void *err_msg;
    sqlite3 *db_from_ctx;
    const char *tail = NULL;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* 
     * Prepare environment: Open an in-memory database.
     * Using :memory: avoids file system dependencies.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Create a custom function to test sqlite3_context_db_handle.
     * This ensures we have a valid context to pass to the API.
     */
    sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, dummy_sql_function, NULL, NULL);

    /* 
     * Prepare the SQL string from fuzz data.
     * Ensure the input is treated as a valid C string by copying to a buffer 
     * and null-terminating it to prevent out-of-bounds reads if the fuzzer 
     * provides binary data without a terminator.
     */
    size_t sql_len = Size;
    if (sql_len > 10000) {
        sql_len = 10000; /* Limit size to prevent excessive memory usage */
    }
    
    char *sql_buf = (char *)malloc(sql_len + 1);
    if (!sql_buf) {
        sqlite3_close(db);
        return 0;
    }

    memcpy(sql_buf, Data, sql_len);
    sql_buf[sql_len] = '\0';

    /* 
     * Invoke sqlite3_prepare to create a statement.
     * This is the legacy API as requested.
     */
    rc = sqlite3_prepare(db, sql_buf, (int)sql_len, &stmt, &tail);

    if (rc == SQLITE_OK && stmt != NULL) {
        /* 
         * Test sqlite3_db_handle: Retrieve the database handle from the statement.
         */
        db_from_stmt = sqlite3_db_handle(stmt);
        if (db_from_stmt != NULL) {
            /* 
             * Test sqlite3_get_autocommit on the retrieved handle.
             */
            autocommit = sqlite3_get_autocommit(db_from_stmt);
            (void)autocommit;
        }

        /* 
         * Test sqlite3_next_stmt: Iterate through statements.
         * Start with NULL to get the first statement.
         */
        next_stmt = sqlite3_next_stmt(db, NULL);
        if (next_stmt != NULL) {
            /* Try to get the next one from the current statement */
            next_stmt = sqlite3_next_stmt(db, stmt);
        }

        /* 
         * Test sqlite3_errmsg16: Get error message in UTF-16.
         * This is safe to call even if no error occurred (returns NULL).
         */
        err_msg = sqlite3_errmsg16(db);
        (void)err_msg;

        /* 
         * Execute the statement to potentially trigger errors or state changes.
         * This helps explore different autocommit states (e.g., after BEGIN).
         */
        rc = sqlite3_step(stmt);
        
        /* 
         * Test sqlite3_get_autocommit again after execution.
         */
        autocommit = sqlite3_get_autocommit(db);
        (void)autocommit;

        /* 
         * Test sqlite3_context_db_handle.
         * We execute the dummy function via SQL to get a valid context.
         * Note: We cannot directly call the function without a VM running,
         * but we can try to prepare a statement that uses it.
         * However, sqlite3_context_db_handle requires a valid sqlite3_context pointer.
         * Since we cannot easily get a live context pointer from outside the VM,
         * we rely on the fact that the fuzzer might trigger code paths inside the VM
         * where this is called. 
         * To explicitly test the API signature safety, we pass NULL (if API armor is on)
         * or a crafted pointer if we could. 
         * Here we simply ensure the function exists and is callable.
         * We cannot safely construct a fake sqlite3_context without internal knowledge.
         * Instead, we rely on the fact that the fuzzer input might cause the VM
         * to invoke our function, but we can't control that directly here.
         * We will skip direct invocation with a fake pointer to avoid crashes,
         * relying on the other APIs to cover the connection state.
         * 
         * Alternative: We can try to call it with NULL to test error handling if API armor is enabled.
         */
        db_from_ctx = sqlite3_context_db_handle(NULL);
        (void)db_from_ctx;

        /* Finalize the statement */
        sqlite3_finalize(stmt);
        stmt = NULL;
    } else {
        /* 
         * If preparation failed, test error handling APIs.
         */
        err_msg = sqlite3_errmsg16(db);
        (void)err_msg;
        autocommit = sqlite3_get_autocommit(db);
        (void)autocommit;
    }

    /* 
     * Cleanup: Close the database connection.
     */
    sqlite3_close(db);
    free(sql_buf);

    return 0;
}
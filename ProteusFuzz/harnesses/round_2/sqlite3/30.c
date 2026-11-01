// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
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
 * Callback function for sqlite3_exec.
 * This is a simple no-op callback to satisfy the function signature.
 */
static int exec_callback(void *data, int argc, char **argv, char **col_name) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

/*
 * Busy handler callback for sqlite3_busy_handler.
 * Returns 0 to stop retrying immediately.
 */
static int busy_handler_callback(void *data, int count) {
    (void)data;
    (void)count;
    return 0;
}

/*
 * Custom SQL function implementation for sqlite3_create_function.
 * This is a scalar function that returns the input integer unchanged.
 */
static void custom_func_impl(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    if (argc > 0) {
        sqlite3_result_int(ctx, sqlite3_value_int(argv[0]));
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int rc;
    const char *filename = "./dummy_file";

    /* Ensure we have enough data to construct a minimal SQL string if needed */
    if (Size < 1) {
        return 0;
    }

    /* 
     * Step 1: Open the database using sqlite3_open_v2.
     * We use SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE to allow creating the file.
     * We pass NULL for VFS to use the default.
     */
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    
    /* 
     * If opening fails, we still need to pass the handle (which might be NULL or partially allocated)
     * to close functions as per the documentation, though sqlite3_close handles NULL safely.
     */
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Step 2: Close the database immediately using sqlite3_close.
     * This fulfills the first part of the required sequence: sqlite3_open_v2 -> sqlite3_close.
     * After this, 'db' is invalid.
     */
    sqlite3_close(db);
    db = NULL;

    /* 
     * To proceed with the rest of the sequence (create_function, busy_handler, exec),
     * we must reopen the database. The task requires invoking these functions,
     * which require a valid database connection.
     */
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Step 3: Register a custom function using sqlite3_create_function.
     * Arguments:
     * - db: The connection
     * - "fuzz_func": Function name
     * - 1: Number of arguments
     * - SQLITE_UTF8: Text encoding
     * - NULL: User data
     * - custom_func_impl: The function pointer
     * - NULL, NULL: No step/final for scalar function
     */
    rc = sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, custom_func_impl, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Step 4: Register a busy handler using sqlite3_busy_handler.
     * Arguments:
     * - db: The connection
     * - busy_handler_callback: The callback function
     * - NULL: User data
     */
    rc = sqlite3_busy_handler(db, busy_handler_callback, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Step 5: Execute SQL using sqlite3_exec.
     * We construct a safe SQL string from the fuzz input to explore program states.
     * We limit the length to prevent stack overflow and ensure null termination.
     */
    size_t sql_len = Size;
    if (sql_len > 1000) {
        sql_len = 1000;
    }

    /* Allocate buffer for SQL string */
    char *sql_buf = (char *)malloc(sql_len + 2); /* +1 for safe terminator, +1 for newline if needed */
    if (!sql_buf) {
        sqlite3_close(db);
        return 0;
    }

    /* Copy input data, ensuring null termination */
    memcpy(sql_buf, Data, sql_len);
    sql_buf[sql_len] = '\0';

    /* 
     * Execute the SQL. 
     * We use the exec_callback for rows, NULL for user data, and &err_msg for errors.
     */
    rc = sqlite3_exec(db, sql_buf, exec_callback, NULL, &err_msg);

    /* Clean up error message if one was generated */
    if (err_msg) {
        sqlite3_free(err_msg);
    }

    /* Free the SQL buffer */
    free(sql_buf);

    /*
     * Final cleanup: Close the database connection.
     */
    sqlite3_close(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to safely extract a null-terminated string from fuzz input.
 * Returns a newly allocated string using malloc, or NULL if Size is 0.
 * The caller is responsible for freeing the returned pointer using free().
 */
static char *safe_extract_string(const uint8_t *Data, size_t Size) {
    if (Size == 0) return NULL;
    size_t len = strnlen((const char *)Data, Size);
    if (len == Size) len = Size - 1; // Ensure null termination if no null found
    char *str = (char *)malloc(len + 1);
    if (str) {
        memcpy(str, Data, len);
        str[len] = '\0';
    }
    return str;
}

/*
 * Static callback for sqlite3_exec.
 * This is a dummy callback that does nothing but return 0 to continue execution.
 */
static int dummy_exec_callback(void *data, int argc, char **argv, char **col_name) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

/*
 * Static callback for sqlite3_busy_handler.
 * Returns 1 to retry once, then 0.
 */
static int dummy_busy_handler(void *data, int count) {
    (void)data;
    (void)count;
    return 1;
}

/*
 * Static scalar function implementation for sqlite3_create_function.
 */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    sqlite3_result_int(ctx, 0);
}

/*
 * Static aggregate step function for sqlite3_create_function.
 */
static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

/*
 * Static aggregate final function for sqlite3_create_function.
 */
static void dummy_final(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *filename = NULL;
    char *func_name = NULL;
    char *sql = NULL;
    char *errmsg = NULL;
    int rc;
    int allocated_filename = 0;
    int allocated_func_name = 0;
    int allocated_sql = 0;

    // 1. Prepare the environment
    // Extract filename from input if available, otherwise use default
    if (Size > 0) {
        filename = safe_extract_string(Data, Size);
        if (filename) {
            allocated_filename = 1;
        }
    }
    if (!filename) {
        filename = "./dummy_file";
        allocated_filename = 0;
    }

    // 2. Invoke sqlite3_open_v2
    // Flags: SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        if (allocated_filename) free(filename);
        return 0;
    }

    // 3. Invoke sqlite3_close (as per requested order)
    rc = sqlite3_close(db);
    
    // If close failed (returned SQLITE_BUSY), the connection is still open but marked as zombie.
    // To ensure we test the subsequent APIs on a clean connection, we will reopen.
    if (rc == SQLITE_BUSY) {
        sqlite3_close(db); // Try to close again
        db = NULL;
    } else {
        db = NULL;
    }

    // Reopen to allow subsequent API calls to execute on a valid handle
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        if (allocated_filename) free(filename);
        return 0;
    }

    // 4. Invoke sqlite3_create_function
    // Extract function name from input if possible
    if (Size > 0) {
        func_name = safe_extract_string(Data, Size);
        if (func_name) {
            allocated_func_name = 1;
        }
    }
    if (!func_name) {
        func_name = "fuzz_func";
        allocated_func_name = 0;
    }
    
    // Register a scalar function
    rc = sqlite3_create_function(db, func_name, 0, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);
    // Register an aggregate function
    rc = sqlite3_create_function(db, "fuzz_agg", 0, SQLITE_UTF8, NULL, NULL, dummy_step, dummy_final);
    
    // Free func_name if we allocated it. Use free() because safe_extract_string uses malloc.
    if (allocated_func_name) {
        free(func_name);
    }

    // 5. Invoke sqlite3_busy_handler
    rc = sqlite3_busy_handler(db, dummy_busy_handler, NULL);

    // 6. Invoke sqlite3_exec
    // Extract SQL from input if possible
    if (Size > 0) {
        sql = safe_extract_string(Data, Size);
        if (sql) {
            allocated_sql = 1;
        }
    }
    if (!sql) {
        sql = "SELECT 1;";
        allocated_sql = 0;
    }

    rc = sqlite3_exec(db, sql, dummy_exec_callback, NULL, &errmsg);
    
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Free sql if we allocated it
    if (allocated_sql) {
        free(sql);
    }

    // 7. Cleanup
    // Finalize the database connection
    sqlite3_close(db);
    
    // Free filename if we allocated it
    if (allocated_filename) {
        free(filename);
    }

    return 0;
}
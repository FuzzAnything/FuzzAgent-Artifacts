// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:92742:17 in sqlite3.h
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
 * Fuzz driver for sqlite3 result functions.
 * Target APIs:
 * 1. sqlite3_result_value
 * 2. sqlite3_result_error_nomem
 * 3. sqlite3_result_double
 * 4. sqlite3_user_data
 * 5. sqlite3_result_null
 * 6. sqlite3_result_error_toobig
 *
 * Note: These APIs are designed to be called from within a custom SQL function callback.
 * To fuzz them effectively, we must:
 * 1. Open a database connection.
 * 2. Register a custom function that invokes these APIs.
 * 3. Execute a SQL statement that calls the custom function.
 * 4. Use the fuzzing input to drive the logic inside the custom function.
 */

/* Global pointer to hold user data for the custom function */
static void *g_user_data_ptr = NULL;

/*
 * Custom SQL function implementation.
 * This function will be called by SQLite during SQL execution.
 * It uses the fuzzing input to decide which API to call and with what arguments.
 */
static void fuzz_func_impl(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    /* Retrieve the fuzzing data passed as user_data */
    const uint8_t *data = (const uint8_t *)sqlite3_user_data(ctx);
    size_t size = 0;

    /* We need to know the size of the data. Since we can't pass size directly via user_data
       in a standard way without a wrapper, we rely on a static global or a specific convention.
       However, the standard pattern for fuzzing is to pass the data pointer and assume
       the function knows how to interpret it or uses a global.
       For this driver, we will use a static global to store the current fuzzing buffer size.
    */
    static size_t g_fuzz_size = 0;
    size = g_fuzz_size;

    if (data == NULL || size == 0) {
        /* Test sqlite3_result_null with valid context */
        sqlite3_result_null(ctx);
        return;
    }

    /* Use the first byte to select a test case */
    int choice = data[0] % 6;

    switch (choice) {
        case 0: {
            /* Test sqlite3_result_null */
            sqlite3_result_null(ctx);
            break;
        }
        case 1: {
            /* Test sqlite3_result_error_nomem */
            sqlite3_result_error_nomem(ctx);
            break;
        }
        case 2: {
            /* Test sqlite3_result_error_toobig */
            sqlite3_result_error_toobig(ctx);
            break;
        }
        case 3: {
            /* Test sqlite3_result_double */
            /* Use bytes 1-8 to form a double if available, else use a constant */
            double val = 0.0;
            if (size >= 8) {
                memcpy(&val, data + 1, sizeof(double));
            } else {
                val = (double)data[1];
            }
            sqlite3_result_double(ctx, val);
            break;
        }
        case 4: {
            /* Test sqlite3_result_value */
            /* We need a valid sqlite3_value. We can construct one or use an argument.
               Since we are inside a function, we can try to use an argument if available.
               If not, we can't easily create a valid sqlite3_value from scratch without
               internal APIs. However, we can try to pass NULL to test the API armor.
               The API documentation says it handles NULL inputs via API armor.
               Let's try passing NULL to see if it triggers the armor or crashes.
               Note: Passing NULL to sqlite3_result_value is undefined behavior in some contexts
               but the prompt says it handles NULL inputs via API armor.
            */
            sqlite3_value *val = NULL;
            if (size > 1 && data[1] == 1) {
                /* If we have arguments, try to use the first one */
                if (argc > 0) {
                    val = argv[0];
                }
            }
            sqlite3_result_value(ctx, val);
            break;
        }
        case 5: {
            /* Test sqlite3_user_data again (already called at start, but good to exercise) */
            void *ud = sqlite3_user_data(ctx);
            (void)ud; /* Use the result to prevent warnings */
            sqlite3_result_null(ctx);
            break;
        }
        default:
            sqlite3_result_null(ctx);
            break;
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Update the global size for the custom function to use */
    static size_t g_fuzz_size = 0;
    g_fuzz_size = Size;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Set the user data pointer for the custom function */
    /* We pass the fuzzing data as the user_data */
    g_user_data_ptr = (void *)Data;

    /* Register the custom function */
    /* Name: fuzz_func, args: -1 (variable), user_data: Data, func: fuzz_func_impl */
    rc = sqlite3_create_function(db, "fuzz_func", -1, SQLITE_UTF8, (void *)Data, fuzz_func_impl, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare and execute a SQL statement that calls our custom function */
    /* We try different SQL patterns based on the input to explore more states */
    const char *sql;
    int pattern = Data[0] % 3;

    switch (pattern) {
        case 0:
            sql = "SELECT fuzz_func();";
            break;
        case 1:
            sql = "SELECT fuzz_func(1, 2, 3);";
            break;
        case 2:
            /* Try to use a string argument if available */
            sql = "SELECT fuzz_func('test');";
            break;
    }

    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        /* Execute the statement */
        rc = sqlite3_step(stmt);
        /* We don't care about the result, just that the function was called */
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}
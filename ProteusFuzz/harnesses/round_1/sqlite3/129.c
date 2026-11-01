// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:92807:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:92807:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Global variable to store the current fuzz size for the callback */
static size_t g_fuzz_size = 0;

/* 
 * Custom SQL function implementation that uses the fuzz data to trigger various result APIs.
 * This function is registered with SQLITE_RESULT_SUBTYPE.
 */
static void fuzz_function_impl(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Retrieve user data (fuzz data pointer) */
    const uint8_t *data = (const uint8_t *)sqlite3_user_data(ctx);
    
    /* Safety check */
    if (!ctx) return;
    if (!data) return;

    /* Use the first byte of data to determine which API to test */
    if (g_fuzz_size > 0) {
        unsigned int op = data[0] & 0xFF;

        switch (op) {
            case 0:
                /* Test sqlite3_result_subtype */
                /* Use the second byte as the subtype value */
                if (g_fuzz_size > 1) {
                    sqlite3_result_subtype(ctx, data[1]);
                } else {
                    sqlite3_result_subtype(ctx, 0);
                }
                break;
            case 1:
                /* Test sqlite3_result_value */
                /* We need a valid sqlite3_value. We can use one of the arguments if available. */
                if (argc > 0 && argv[0]) {
                    sqlite3_result_value(ctx, argv[0]);
                } else {
                    /* If no args, create a dummy value? No, we can't easily create one.
                     * Just return NULL or error if no args.
                     */
                    sqlite3_result_null(ctx);
                }
                break;
            case 2:
                /* Test sqlite3_result_error_nomem */
                sqlite3_result_error_nomem(ctx);
                break;
            case 3:
                /* Test sqlite3_user_data (already called above, but let's call it again to be safe) */
                (void)sqlite3_user_data(ctx);
                /* Also set a result to indicate success */
                sqlite3_result_int(ctx, 1);
                break;
            case 4:
                /* Test sqlite3_result_null */
                sqlite3_result_null(ctx);
                break;
            case 5:
                /* Test sqlite3_result_error_toobig */
                sqlite3_result_error_toobig(ctx);
                break;
            default:
                /* Default: Return a simple integer to avoid hanging or undefined behavior */
                sqlite3_result_int(ctx, 0);
                break;
        }
    } else {
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *sql;

    if (Size == 0) {
        return 0;
    }

    /* Set the global size for the callback to use */
    g_fuzz_size = Size;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to allow queries */
    sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function with SQLITE_RESULT_SUBTYPE */
    /* The 5th argument is pUserData. We pass the fuzz data pointer. */
    /* Note: We are passing Data as pUserData. The function will cast it back. */
    /* We also need to ensure the function is registered with the SQLITE_RESULT_SUBTYPE flag. */
    rc = sqlite3_create_function(db, "test_func", -1, SQLITE_UTF8 | SQLITE_RESULT_SUBTYPE, 
                                  (void*)Data, fuzz_function_impl, NULL, NULL);
    
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement to call the function */
    /* We pass the fuzz data as an argument to the function to trigger it */
    sqlite3_stmt *stmt = NULL;
    sql = "SELECT test_func(?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzz data as a blob to the statement */
    sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);

    /* Execute the statement. This will call fuzz_function_impl */
    rc = sqlite3_step(stmt);
    /* We don't care about the result of the step, just that it executed without crashing */
    /* If rc is SQLITE_ROW, the function returned a value. If SQLITE_DONE, it returned nothing. */
    /* Errors inside the function (like result_error_nomem) will be reflected in rc or the function's internal state. */

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
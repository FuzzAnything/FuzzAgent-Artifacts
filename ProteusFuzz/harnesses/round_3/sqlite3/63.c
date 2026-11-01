// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_result_double at sqlite3.c:92742:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
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
 * Custom SQL function callback to test result setting functions.
 * This function is registered with sqlite3_create_function.
 */
static void test_result_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Test sqlite3_result_double */
    double test_double = 0.0;
    if (argc > 0) {
        /* Attempt to get a double from argument if possible, otherwise use default */
        test_double = sqlite3_value_double(argv[0]);
    }
    sqlite3_result_double(ctx, test_double);

    /* Test sqlite3_result_null */
    sqlite3_result_null(ctx);

    /* Test sqlite3_result_error_toobig */
    sqlite3_result_error_toobig(ctx);

    /* Test sqlite3_user_data */
    void *userData = sqlite3_user_data(ctx);
    (void)userData; /* Suppress unused warning */

    /* Test sqlite3_result_value */
    if (argc > 1 && argv[1] != NULL) {
        sqlite3_result_value(ctx, argv[1]);
    }

    /* Test sqlite3_result_error_nomem */
    sqlite3_result_error_nomem(ctx);
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql = "SELECT test_result_function(?)";
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "test_result_function", -1, SQLITE_UTF8, (void*)0x12345678, test_result_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare the SQL statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind input data to the statement */
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, (const void*)Data, Size, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_null(stmt, 1);
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    (void)rc; /* We don't care about the result of execution for fuzzing */

    /* Finalize and cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
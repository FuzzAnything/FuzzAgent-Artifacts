// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_result_double at sqlite3.c:92742:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:92742:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <math.h>

/*
 * Static helper function to simulate a custom SQL function callback.
 * This function uses the fuzzed input to decide which result functions to call.
 */
static void fuzzed_custom_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (ctx == NULL) {
        return;
    }

    if (argc > 0) {
        int action = sqlite3_value_int(argv[0]);
        
        switch (action) {
            case 0:
                /* Test sqlite3_result_null */
                sqlite3_result_null(ctx);
                break;
            case 1:
                /* Test sqlite3_result_double */
                if (argc > 1) {
                    double val = sqlite3_value_double(argv[1]);
                    sqlite3_result_double(ctx, val);
                } else {
                    sqlite3_result_double(ctx, 0.0);
                }
                break;
            case 2:
                /* Test sqlite3_result_error_toobig */
                sqlite3_result_error_toobig(ctx);
                break;
            case 3:
                /* Test sqlite3_result_zeroblob */
                if (argc > 1) {
                    int n = sqlite3_value_int(argv[1]);
                    sqlite3_result_zeroblob(ctx, n);
                } else {
                    sqlite3_result_zeroblob(ctx, 0);
                }
                break;
            case 4:
                /* Test sqlite3_user_data (read-only, no result setting) */
                (void)sqlite3_user_data(ctx);
                sqlite3_result_int(ctx, 0);
                break;
            case 5:
                /* Test sqlite3_result_error_nomem */
                sqlite3_result_error_nomem(ctx);
                break;
            default:
                /* Default: set a simple integer result */
                sqlite3_result_int(ctx, action);
                break;
        }
    } else {
        /* No arguments, just return a null to be safe */
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    if (Size < 1) {
        return 0;
    }

    /* Initialize SQLite database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* 
     * Register the custom function.
     * We pass a pointer to a small integer from the input data as user_data
     * to test sqlite3_user_data().
     */
    void *user_data = NULL;
    if (Size > 0) {
        user_data = (void*)(uintptr_t)(Data[0]); 
    }

    rc = sqlite3_create_function(
        db, 
        "fuzz_func", 
        -1, 
        SQLITE_UTF8, 
        user_data, 
        fuzzed_custom_function, 
        NULL, 
        NULL  
    );

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Construct a SQL query that calls our function.
     * We will parse the input data to generate arguments for the function.
     */
    
    /* Count arguments we can extract from the input */
    int num_args = 0;
    if (Size > 1) {
        num_args = (Size - 1) / 2; 
        if (num_args > 10) num_args = 10;
    }

    /* 
     * Calculate exact buffer size needed to avoid overflow.
     * "SELECT fuzz_func(" is 18 chars.
     * First arg: up to 3 digits + sign = 4 chars.
     * Subsequent args: ", " (2 chars) + up to 3 digits + sign = 5 chars each.
     * Closing ")" = 1 char.
     * Null terminator = 1 char.
     */
    size_t sql_len = 18 + 4 + (num_args * 5) + 1 + 1;
    
    char *sql = (char *)malloc(sql_len);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }

    /* Start building the string */
    size_t current_len = 0;
    current_len += snprintf(sql, sql_len, "SELECT fuzz_func(%d", Data[0]);

    /* Add arguments from the input data */
    for (int i = 0; i < num_args; i++) {
        if (1 + i < Size) {
            int val = Data[1 + i];
            current_len += snprintf(sql + current_len, sql_len - current_len, ", %d", val);
        }
    }

    /* Close the argument list safely */
    current_len += snprintf(sql + current_len, sql_len - current_len, ")");

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    free(sql);

    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Step through the statement to trigger the function execution */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        (void)sqlite3_column_int(stmt, 0);
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a valid sqlite3_context for fuzzing.
 * We will use a global variable to pass fuzzing data to the callback.
 */

/* Global buffer to hold fuzzing data for the callback */
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

/* 
 * Callback function for scalar and aggregate functions.
 * Signature must be void (context, argc, argv) as per sqlite3_create_function.
 */
static void fuzzing_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (g_fuzz_data == NULL || g_fuzz_size == 0) {
        /* Fallback if no data provided */
        sqlite3_result_int(ctx, 0);
        return;
    }

    /* 
     * We will interpret the fuzzing data to decide which API to call.
     * This allows us to fuzz all 6 target APIs.
     */
    
    /* Read a command byte to select the function */
    int cmd = g_fuzz_data[0] % 6;
    size_t offset = 1;

    switch (cmd) {
        case 0: {
            /* sqlite3_result_zeroblob64(sqlite3_context*, sqlite3_uint64 n) */
            if (offset + sizeof(sqlite3_uint64) <= g_fuzz_size) {
                sqlite3_uint64 n = 0;
                memcpy(&n, g_fuzz_data + offset, sizeof(n));
                offset += sizeof(n);
                sqlite3_result_zeroblob64(ctx, n);
            } else {
                sqlite3_result_zeroblob64(ctx, 0);
            }
            break;
        }
        case 1: {
            /* sqlite3_result_error_code(sqlite3_context*, int) */
            if (offset + sizeof(int) <= g_fuzz_size) {
                int errCode = 0;
                memcpy(&errCode, g_fuzz_data + offset, sizeof(errCode));
                offset += sizeof(int);
                sqlite3_result_error_code(ctx, errCode);
            } else {
                sqlite3_result_error_code(ctx, 1);
            }
            break;
        }
        case 2: {
            /* sqlite3_result_error16(sqlite3_context*, const void*, int) */
            /* We need a length and a pointer. We'll use the rest of the buffer as the string */
            if (offset + sizeof(int) <= g_fuzz_size) {
                int len = 0;
                memcpy(&len, g_fuzz_data + offset, sizeof(len));
                offset += sizeof(int);
                
                /* Ensure len is within bounds */
                if (len > (int)(g_fuzz_size - offset)) {
                    len = (int)(g_fuzz_size - offset);
                }
                
                /* The data might not be valid UTF-16, but the API accepts void* and length */
                sqlite3_result_error16(ctx, g_fuzz_data + offset, len);
            } else {
                sqlite3_result_error16(ctx, "", 0);
            }
            break;
        }
        case 3: {
            /* sqlite3_result_zeroblob(sqlite3_context*, int n) */
            if (offset + sizeof(int) <= g_fuzz_size) {
                int n = 0;
                memcpy(&n, g_fuzz_data + offset, sizeof(n));
                offset += sizeof(int);
                sqlite3_result_zeroblob(ctx, n);
            } else {
                sqlite3_result_zeroblob(ctx, 0);
            }
            break;
        }
        case 4: {
            /* sqlite3_aggregate_context(sqlite3_context*, int nBytes) */
            /* This function returns a pointer, we just call it. */
            if (offset + sizeof(int) <= g_fuzz_size) {
                int nBytes = 0;
                memcpy(&nBytes, g_fuzz_data + offset, sizeof(nBytes));
                offset += sizeof(int);
                void *ptr = sqlite3_aggregate_context(ctx, nBytes);
                /* Use the pointer to avoid unused variable warning, though not strictly needed */
                if (ptr) {
                    /* Do something with ptr if needed, but just calling is enough for state exploration */
                }
            } else {
                sqlite3_aggregate_context(ctx, 0);
            }
            break;
        }
        case 5: {
            /* sqlite3_result_int(sqlite3_context*, int) */
            if (offset + sizeof(int) <= g_fuzz_size) {
                int val = 0;
                memcpy(&val, g_fuzz_data + offset, sizeof(val));
                offset += sizeof(int);
                sqlite3_result_int(ctx, val);
            } else {
                sqlite3_result_int(ctx, 0);
            }
            break;
        }
        default:
            sqlite3_result_int(ctx, 0);
            break;
    }
}

/*
 * Fuzzing entry point
 */
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom scalar function */
    rc = sqlite3_create_function_v2(db, "fuzz_func", -1, SQLITE_UTF8, NULL, fuzzing_func, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Set global fuzz data for the callback */
    g_fuzz_data = Data;
    g_fuzz_size = Size;

    /* 
     * We need to execute a query that calls our function.
     * Since the function is scalar, we can use "SELECT fuzz_func()".
     * To test aggregate context, we might need an aggregate function, 
     * but the prompt lists sqlite3_aggregate_context as a target.
     * Let's create an aggregate function as well to ensure we can test case 4.
     */
    
    /* Register an aggregate function that also uses the fuzzing logic */
    rc = sqlite3_create_function(db, "fuzz_agg", -1, SQLITE_UTF8, NULL, fuzzing_func, fuzzing_func, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement to call the scalar function */
    const char *sql = "SELECT fuzz_func();";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Step through the statement to trigger the callback */
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Consume result if any */
        }
        sqlite3_finalize(stmt);
    }

    /* Prepare a statement to call the aggregate function to test aggregate_context */
    /* We create a dummy table to have rows for the aggregate to iterate over */
    rc = sqlite3_exec(db, "CREATE TABLE dummy (id INT);", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        /* Insert a few rows to ensure aggregate step is called */
        rc = sqlite3_exec(db, "INSERT INTO dummy VALUES (1), (2), (3);", NULL, NULL, NULL);
        if (rc == SQLITE_OK) {
            const char *sql_agg = "SELECT fuzz_agg(id) FROM dummy;";
            rc = sqlite3_prepare_v2(db, sql_agg, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    /* Consume result */
                }
                sqlite3_finalize(stmt);
            }
        }
    }

    /* 2. Cleanup */
    sqlite3_close(db);
    
    /* Reset globals */
    g_fuzz_data = NULL;
    g_fuzz_size = 0;

    return 0;
}
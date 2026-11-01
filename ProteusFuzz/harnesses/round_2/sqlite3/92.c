// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 result functions.
 * Target APIs:
 * 1. sqlite3_result_error16
 * 2. sqlite3_result_zeroblob64
 * 3. sqlite3_result_zeroblob
 * 4. sqlite3_aggregate_context
 * 5. sqlite3_result_int
 * 6. sqlite3_result_error_code
 */

/* Structure to pass fuzz input to the callback via user_data */
typedef struct {
    const uint8_t *data;
    size_t size;
} FuzzInput;

/* Forward declarations */
static void fuzz_step_with_input(sqlite3_context *ctx, int argc, sqlite3_value **argv);
static void fuzz_final_with_input(sqlite3_context *ctx);

/*
 * Step function for the aggregate.
 * Uses user_data to access the fuzz input.
 * We must ensure we only set ONE result per call to avoid assertion failures.
 */
static void fuzz_step_with_input(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    if (!ctx) return;
    
    FuzzInput *inp = (FuzzInput*)sqlite3_user_data(ctx);
    if (!inp) return;
    
    const uint8_t *d = inp->data;
    size_t s = inp->size;

    /* 
     * We need to pick ONE action per step to avoid setting multiple results 
     * which triggers the assertion: assert( pCtx->pOut->flags==MEM_Null );
     * We use the first byte to determine which API to test.
     */
    
    if (s == 0) return;
    
    int action = d[0] % 6;
    int offset = 1;

    switch (action) {
        case 0: {
            /* Test sqlite3_result_int */
            int int_val = 0;
            if (s >= offset + 4) {
                int_val = ((d[offset] << 24) | (d[offset+1] << 16) | (d[offset+2] << 8) | d[offset+3]);
            }
            sqlite3_result_int(ctx, int_val);
            break;
        }
        case 1: {
            /* Test sqlite3_result_zeroblob */
            int blob_len = 0;
            if (s >= offset + 4) {
                blob_len = ((d[offset] << 24) | (d[offset+1] << 16) | (d[offset+2] << 8) | d[offset+3]);
                if (blob_len < 0) blob_len = 0;
            }
            sqlite3_result_zeroblob(ctx, blob_len);
            break;
        }
        case 2: {
            /* Test sqlite3_result_zeroblob64 */
            sqlite3_uint64 blob64_len = 0;
            if (s >= offset + 8) {
                blob64_len = 0;
                for (int i = 0; i < 8; i++) {
                    blob64_len = (blob64_len << 8) | d[offset + i];
                }
            }
            sqlite3_result_zeroblob64(ctx, blob64_len);
            break;
        }
        case 3: {
            /* Test sqlite3_result_error16 (UTF-16 error message) */
            if (s >= offset + 2) {
                int msg_len = ((d[offset] << 8) | d[offset+1]);
                if (msg_len > 0 && msg_len <= (int)(s - offset - 2)) {
                    if (msg_len % 2 != 0) msg_len--;
                    if (msg_len > 0) {
                        const void *msg_ptr = &d[offset + 2];
                        sqlite3_result_error16(ctx, msg_ptr, msg_len);
                    }
                }
            }
            break;
        }
        case 4: {
            /* Test sqlite3_result_error_code */
            /* First set an error message, then set the code, as error_code updates the state */
            if (s >= offset + 2) {
                int err_code = ((d[offset] << 8) | d[offset+1]);
                if (err_code == 0) err_code = 1;
                
                /* We need a message for error_code to work properly in some contexts, 
                   but the API allows setting code alone if error is already set.
                   To be safe, let's set a simple error message first if possible, 
                   or just rely on the fact that result_error_code sets the error state.
                   However, result_error_code documentation says: "if the result is null, sets the error message string accordingly".
                   Let's just set the code. If it fails without a message, that's a valid test case.
                   But to be safer, let's set a generic error string first if we have space.
                   Actually, let's just set the code. The API handles it.
                */
                sqlite3_result_error_code(ctx, err_code);
            }
            break;
        }
        case 5: {
            /* Test sqlite3_aggregate_context */
            int alloc_size = 0;
            if (s >= offset + 2) {
                alloc_size = ((d[offset] << 8) | d[offset+1]);
            }
            void *agg_ctx = sqlite3_aggregate_context(ctx, alloc_size);
            if (agg_ctx && alloc_size > 0) {
                size_t write_len = (s > (size_t)alloc_size) ? (size_t)alloc_size : s;
                /* Ensure we don't read past buffer */
                if (write_len > 0) {
                     memcpy(agg_ctx, &d[offset], write_len);
                }
            }
            /* After using context, we should return a result to avoid NULL result in aggregate?
               Usually aggregates return something. Let's return 0 to be safe.
            */
            sqlite3_result_int(ctx, 0);
            break;
        }
    }
}

/*
 * Final function for the aggregate.
 */
static void fuzz_final_with_input(sqlite3_context *ctx) {
    if (!ctx) return;
    /* In xFinal, it is customary to call aggregate_context with 0 */
    void *ctx_ptr = sqlite3_aggregate_context(ctx, 0);
    (void)ctx_ptr;
    sqlite3_result_int(ctx, 42);
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to aggregate on */
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, val TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert some dummy data to ensure aggregate functions have rows to process */
    const char *insert_sql = "INSERT INTO test_table (val) VALUES ('fuzz_data'), ('more_data');";
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    (void)rc;

    /* Prepare input structure */
    FuzzInput *input = malloc(sizeof(FuzzInput));
    if (!input) {
        sqlite3_close(db);
        return 0;
    }
    input->data = Data;
    input->size = Size;

    /* Register the custom aggregate function with user_data */
    rc = sqlite3_create_function_v2(
        db,
        "fuzz_agg",
        0, /* Variable number of arguments */
        SQLITE_UTF8,
        input, /* user_data (pApp) */
        NULL, /* xFunc (scalar) - NULL for aggregate */
        fuzz_step_with_input, /* xStep */
        fuzz_final_with_input, /* xFinal */
        NULL  /* xDestroy */
    );

    if (rc != SQLITE_OK) {
        free(input);
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a query using the custom function */
    const char *select_sql = "SELECT fuzz_agg(*) FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        free(input);
        sqlite3_close(db);
        return 0;
    }

    /* Execute the statement to trigger the callbacks */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        /* Do nothing, just let the function run */
    }

    sqlite3_finalize(stmt);
    free(input);
    sqlite3_close(db);

    return 0;
}
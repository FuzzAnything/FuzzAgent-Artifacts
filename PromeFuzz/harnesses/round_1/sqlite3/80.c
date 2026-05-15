// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "sqlite3.h"

static void dummy_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This function is never actually called, but provides a context
}

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // We'll use static variables to pass the fuzzer data into this function
    static const uint8_t *fuzz_data = NULL;
    static size_t fuzz_size = 0;
    
    if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_BLOB) {
        // Get the fuzzer data from the blob argument
        fuzz_data = (const uint8_t *)sqlite3_value_blob(argv[0]);
        fuzz_size = sqlite3_value_bytes(argv[0]);
    }
    
    if (fuzz_size < 1) {
        sqlite3_result_null(ctx);
        return;
    }
    
    uint8_t selector = fuzz_data[0];
    const uint8_t *params = fuzz_data + 1;
    size_t params_size = fuzz_size - 1;
    
    // Create a dummy value for testing
    sqlite3_value *dummy_val = sqlite3_value_dup(argv[0]);
    
    switch (selector % 6) {
        case 0: {
            // Test sqlite3_result_zeroblob
            int n = 0;
            if (params_size >= sizeof(int)) {
                memcpy(&n, params, sizeof(int));
            } else if (params_size > 0) {
                n = params[0];
            }
            sqlite3_result_zeroblob(ctx, n);
            break;
        }
        
        case 1: {
            // Test sqlite3_result_value
            if (dummy_val) {
                sqlite3_result_value(ctx, dummy_val);
            } else {
                sqlite3_result_null(ctx);
            }
            break;
        }
        
        case 2: {
            // Test sqlite3_result_blob
            if (params_size > 0) {
                void (*destructor)(void*) = NULL;
                // Choose destructor based on data
                if (params[0] % 3 == 0) {
                    destructor = SQLITE_STATIC;
                } else if (params[0] % 3 == 1) {
                    destructor = SQLITE_TRANSIENT;
                }
                // Use remaining data as blob content
                const void *blob_data = params;
                int blob_size = (params_size < 1000) ? (int)params_size : 1000;
                sqlite3_result_blob(ctx, blob_data, blob_size, destructor);
            } else {
                sqlite3_result_null(ctx);
            }
            break;
        }
        
        case 3: {
            // Test sqlite3_get_auxdata
            int N = 0;
            if (params_size >= sizeof(int)) {
                memcpy(&N, params, sizeof(int));
            } else if (params_size > 0) {
                N = params[0];
            }
            void *aux = sqlite3_get_auxdata(ctx, N);
            // aux may be NULL, that's expected
            (void)aux;
            sqlite3_result_null(ctx);
            break;
        }
        
        case 4: {
            // Test sqlite3_result_zeroblob64
            sqlite3_uint64 n = 0;
            if (params_size >= sizeof(sqlite3_uint64)) {
                memcpy(&n, params, sizeof(sqlite3_uint64));
            } else if (params_size > 0) {
                n = params[0];
            }
            int result = sqlite3_result_zeroblob64(ctx, n);
            (void)result; // Handle return value
            break;
        }
        
        case 5: {
            // Test sqlite3_result_error
            const char *err_msg = "fuzzer error";
            int err_len = -1; // Use null-terminated string
            
            if (params_size > 0) {
                // Use params as error message if we have data
                err_msg = (const char*)params;
                if (params_size < 1000) {
                    err_len = (int)params_size;
                }
            }
            sqlite3_result_error(ctx, err_msg, err_len);
            break;
        }
    }
    
    if (dummy_val) {
        sqlite3_value_free(dummy_val);
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple user-defined function to get a context
    sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, 
                           dummy_function, NULL, NULL);
    
    // Prepare a statement to call the function
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT dummy_func()";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create the test function
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                           test_function, NULL, NULL);
    
    // Prepare a statement to call test_func with our fuzzer data
    sqlite3_stmt *test_stmt = NULL;
    const char *test_sql = "SELECT test_func(?)";
    
    if (sqlite3_prepare_v2(db, test_sql, -1, &test_stmt, NULL) == SQLITE_OK) {
        // Bind the fuzzer data as a blob parameter
        sqlite3_bind_blob(test_stmt, 1, Data, (int)Size, SQLITE_STATIC);
        
        // Execute the statement - this will call test_function with our context
        sqlite3_step(test_stmt);
        
        // Finalize the test statement
        sqlite3_finalize(test_stmt);
    }
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
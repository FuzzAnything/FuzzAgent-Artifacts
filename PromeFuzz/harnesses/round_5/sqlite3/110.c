// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - used for sqlite3_result_blob
}

static void test_udf(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    uint8_t selector = 0;
    const uint8_t *data = NULL;
    int data_size = 0;
    
    if (argc > 0) {
        data = sqlite3_value_blob(argv[0]);
        data_size = sqlite3_value_bytes(argv[0]);
        
        if (data != NULL && data_size > 0) {
            selector = data[0] % 6;
        }
    }
    
    switch (selector) {
        case 0:
            sqlite3_result_null(ctx);
            break;
            
        case 1:
            if (argc > 0) {
                sqlite3_result_value(ctx, argv[0]);
            } else {
                sqlite3_result_null(ctx);
            }
            break;
            
        case 2:
            if (data != NULL && data_size > 1) {
                // Create error message from data (skip first byte for selector)
                int msg_len = data_size - 1;
                if (msg_len > 0) {
                    char *error_msg = malloc(msg_len + 1);
                    if (error_msg) {
                        memcpy(error_msg, data + 1, msg_len);
                        error_msg[msg_len] = '\0';
                        sqlite3_result_error(ctx, error_msg, -1);
                        free(error_msg);
                    } else {
                        sqlite3_result_error_nomem(ctx);
                    }
                } else {
                    sqlite3_result_error(ctx, "fuzzer error", -1);
                }
            } else {
                sqlite3_result_error(ctx, "test error", -1);
            }
            break;
            
        case 3:
            sqlite3_result_error_toobig(ctx);
            break;
            
        case 4:
            if (data != NULL && data_size > 1) {
                int blob_len = data_size - 1;
                if (blob_len > 0) {
                    sqlite3_result_blob(ctx, data + 1, blob_len, SQLITE_TRANSIENT);
                } else {
                    sqlite3_result_null(ctx);
                }
            } else {
                sqlite3_result_null(ctx);
            }
            break;
            
        case 5:
            sqlite3_result_error_nomem(ctx);
            break;
            
        default:
            sqlite3_result_null(ctx);
            break;
    }
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Register the test function
    sqlite3_create_function(
        db,
        "fuzz_test",
        1,
        SQLITE_UTF8,
        NULL,
        test_udf,
        NULL,
        NULL
    );
    
    // Prepare a statement to call our fuzz_test function
    sqlite3_stmt *stmt = NULL;
    const char *test_sql = "SELECT fuzz_test(?)";
    
    if (sqlite3_prepare_v2(db, test_sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (Size > 0) {
            int bind_size = (int)(Size > INT_MAX ? INT_MAX : Size);
            sqlite3_bind_blob(stmt, 1, Data, bind_size, SQLITE_STATIC);
        }
        
        // Execute - this will call our test_udf function with the fuzzer data
        sqlite3_step(stmt);
        
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return 0;
}
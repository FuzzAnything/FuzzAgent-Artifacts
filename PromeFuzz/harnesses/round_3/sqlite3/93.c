// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_pointer at sqlite3.c:78781:17 in sqlite3.h
// sqlite3_result_blob64 at sqlite3.c:78715:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:78837:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - just a dummy destructor for testing
}

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Get the blob argument
    const void *blob_data = sqlite3_value_blob(argv[0]);
    int blob_size = sqlite3_value_bytes(argv[0]);
    
    if (blob_size > 0) {
        const uint8_t *data = (const uint8_t *)blob_data;
        size_t size = (size_t)blob_size;
        
        // Use the first byte to select which function to test
        uint8_t selector = data[0] % 6;
        const uint8_t *func_data = data + 1;
        size_t func_data_size = size > 1 ? size - 1 : 1;
        
        switch (selector) {
            case 0: { // sqlite3_result_pointer
                const char *type_str = "TestType";
                void *ptr = (void *)func_data;
                sqlite3_result_pointer(ctx, ptr, type_str, dummy_destructor);
                break;
            }
            
            case 1: { // sqlite3_result_blob64
                sqlite3_uint64 blob_size64 = (sqlite3_uint64)func_data_size;
                if (blob_size64 > 0xFFFFFFFF) blob_size64 = 0xFFFFFFFF;
                sqlite3_result_blob64(ctx, func_data, blob_size64, dummy_destructor);
                break;
            }
            
            case 2: { // sqlite3_result_error_nomem
                sqlite3_result_error_nomem(ctx);
                break;
            }
            
            case 3: { // sqlite3_result_text
                // Create a null-terminated string
                char *text = (char *)malloc(func_data_size + 1);
                if (text) {
                    memcpy(text, func_data, func_data_size);
                    text[func_data_size] = '\0';
                    sqlite3_result_text(ctx, text, -1, dummy_destructor);
                    free(text);
                } else {
                    sqlite3_result_error_nomem(ctx);
                }
                break;
            }
            
            case 4: { // sqlite3_result_text64
                // Create a null-terminated string
                char *text = (char *)malloc(func_data_size + 1);
                if (text) {
                    memcpy(text, func_data, func_data_size);
                    text[func_data_size] = '\0';
                    sqlite3_uint64 text_size = (sqlite3_uint64)func_data_size;
                    unsigned char encoding = SQLITE_UTF8;
                    if (func_data_size > 1) {
                        encoding = func_data[0] % 4; // 4 possible encodings
                    }
                    sqlite3_result_text64(ctx, text, text_size, dummy_destructor, encoding);
                    free(text);
                } else {
                    sqlite3_result_error_nomem(ctx);
                }
                break;
            }
            
            case 5: { // sqlite3_result_blob
            default:
                int blob_size_int = (int)func_data_size;
                if (blob_size_int < 0) blob_size_int = 0;
                if (blob_size_int > 1000000) blob_size_int = 1000000; // Limit size
                sqlite3_result_blob(ctx, func_data, blob_size_int, dummy_destructor);
                break;
            }
        }
    } else {
        // No data - return null
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Register the test function
    sqlite3_create_function(
        db,
        "test_fuzz_func",
        1,
        SQLITE_UTF8,
        NULL,
        test_function,
        NULL,
        NULL
    );
    
    // Prepare and execute a statement that calls our test function
    const char *test_sql = "SELECT test_fuzz_func(?)";
    if (sqlite3_prepare_v2(db, test_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the fuzzer data
        if (Size > 0) {
            size_t bind_size = Size;
            if (bind_size > 1024 * 1024) {
                bind_size = 1024 * 1024;
            }
            sqlite3_bind_blob(stmt, 1, Data, (int)bind_size, SQLITE_STATIC);
        } else {
            sqlite3_bind_blob(stmt, 1, "", 0, SQLITE_STATIC);
        }
        
        // Execute - this will call our test_function with a valid sqlite3_context
        sqlite3_step(stmt);
        
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return 0;
}
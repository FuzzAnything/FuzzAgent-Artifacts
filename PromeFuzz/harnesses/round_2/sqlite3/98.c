// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_vtab_nochange at sqlite3.c:79256:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Structure to hold fuzzer data for the user-defined function
typedef struct {
    const uint8_t* data;
    size_t size;
    int call_count;
} fuzz_data_t;

// User-defined function that uses the fuzzer input
static void test_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    fuzz_data_t* fuzz_data = (fuzz_data_t*)sqlite3_user_data(ctx);
    if (!fuzz_data || fuzz_data->size < 1) {
        sqlite3_result_int(ctx, 0);
        return;
    }
    
    const uint8_t* Data = fuzz_data->data;
    size_t Size = fuzz_data->size;
    
    // Use first byte to determine which function to test
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            // Test sqlite3_result_error16
            if (Size > 1) {
                const void* err_msg = (const void*)(Data + 1);
                int err_len = (Size - 1 > 1000) ? 1000 : (int)(Size - 1);
                sqlite3_result_error16(ctx, err_msg, err_len);
            } else {
                sqlite3_result_error16(ctx, L"test error", 10);
            }
            break;
        }
        
        case 1: {
            // Test sqlite3_result_error
            if (Size > 1) {
                const char* err_msg = (const char*)(Data + 1);
                int err_len = (Size - 1 > 1000) ? 1000 : (int)(Size - 1);
                sqlite3_result_error(ctx, err_msg, err_len);
            } else {
                sqlite3_result_error(ctx, "test error", 10);
            }
            break;
        }
        
        case 2: {
            // Test sqlite3_result_int
            int int_val = 0;
            if (Size >= 5) {
                memcpy(&int_val, Data + 1, sizeof(int));
            } else if (Size > 1) {
                int_val = Data[1];
            }
            sqlite3_result_int(ctx, int_val);
            break;
        }
        
        case 3: {
            // Test sqlite3_vtab_nochange
            int result = sqlite3_vtab_nochange(ctx);
            // Return the result as an integer
            sqlite3_result_int(ctx, result);
            break;
        }
        
        case 4: {
            // Test sqlite3_result_error_code
            int err_code = 0;
            if (Size >= 5) {
                memcpy(&err_code, Data + 1, sizeof(int));
            } else if (Size > 1) {
                err_code = Data[1];
            }
            sqlite3_result_error_code(ctx, err_code);
            break;
        }
        
        case 5: {
            // Test sqlite3_get_auxdata
            int N = 0;
            if (Size > 1) {
                N = Data[1] % 16;
            }
            
            // Try to get auxdata
            void* aux_data = sqlite3_get_auxdata(ctx, N);
            
            // Set some auxdata first if we have enough data
            if (Size > 10) {
                char* test_data = malloc(10);
                if (test_data) {
                    size_t copy_size = (Size - 2) < 10 ? (Size - 2) : 10;
                    if (copy_size > 0) {
                        memcpy(test_data, Data + 2, copy_size);
                    }
                    sqlite3_set_auxdata(ctx, N, test_data, free);
                    
                    // Get it back
                    aux_data = sqlite3_get_auxdata(ctx, N);
                }
            }
            
            // Return whether we got auxdata or not
            sqlite3_result_int(ctx, aux_data != NULL ? 1 : 0);
            break;
        }
    }
    
    fuzz_data->call_count++;
}

int LLVMFuzzerTestOneInput_98(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3* db = NULL;
    sqlite3_stmt* stmt = NULL;
    int rc;
    
    // Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create fuzz data structure
    fuzz_data_t fuzz_data = {Data, Size, 0};
    
    // Register our test function with the fuzz data as user data
    rc = sqlite3_create_function(db, "test_func", 0, SQLITE_UTF8, &fuzz_data, 
                                  test_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare and execute a statement that calls our test function
    const char* sql = "SELECT test_func()";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        // Step to execute the function
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    if (db) sqlite3_close(db);
    
    return 0;
}
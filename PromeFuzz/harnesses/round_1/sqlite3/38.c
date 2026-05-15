// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_blob(ctx, "dummy", 5, SQLITE_TRANSIENT);
}

static void test_result_int(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    // Use fuzzer data to determine the integer value
    // We'll use a simple hash of the input data
    int int_value = 0;
    
    // Get user data passed to the function
    struct fuzz_data {
        const uint8_t *data;
        size_t size;
    };
    
    struct fuzz_data *fd = (struct fuzz_data *)sqlite3_user_data(ctx);
    if (fd && fd->data && fd->size > 0) {
        for (size_t i = 0; i < fd->size && i < 4; i++) {
            int_value = (int_value << 8) | fd->data[i];
        }
    }
    
    // Call sqlite3_result_int with the computed value
    sqlite3_result_int(ctx, int_value);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Initialize sqlite3 in memory
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy function to get a sqlite3_value
    sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, dummy_function, NULL, NULL);
    
    // Prepare a statement that calls our dummy function
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT dummy_func()";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step to execute the function
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get the value from the first column
        sqlite3_value *value = sqlite3_column_value(stmt, 0);
        
        if (value) {
            // 1. Call sqlite3_value_bytes
            int bytes = sqlite3_value_bytes(value);
            (void)bytes; // Use the result to avoid compiler warnings
            
            // 2. Call sqlite3_value_blob
            const void *blob = sqlite3_value_blob(value);
            (void)blob; // Use the result to avoid compiler warnings
            
            // 3. Create a context for sqlite3_result_int
            // We need to create another function to test sqlite3_result_int
            struct fuzz_data {
                const uint8_t *data;
                size_t size;
            };
            
            struct fuzz_data fd = {Data, Size};
            
            // Register and call the test function with user data
            sqlite3_create_function(db, "test_int", 0, SQLITE_UTF8, &fd, test_result_int, NULL, NULL);
            
            sqlite3_stmt *stmt2 = NULL;
            const char *sql2 = "SELECT test_int()";
            if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL) == SQLITE_OK) {
                sqlite3_step(stmt2);
                sqlite3_finalize(stmt2);
            }
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
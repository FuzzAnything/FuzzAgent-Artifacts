// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function is called by sqlite3 but we don't need to implement it
    // since we're directly calling the API functions in our fuzzer
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Initialize SQLite
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple function to get a context
    sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, dummy_function, NULL, NULL);
    
    // Prepare a statement to get a sqlite3_context
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT dummy_func()";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Get sqlite3_context from the statement
    sqlite3_context *ctx = sqlite3_context_db_handle(stmt) ? (sqlite3_context *)stmt : NULL;
    
    // Create a sqlite3_value using sqlite3_bind_* functions
    sqlite3_stmt *stmt2 = NULL;
    const char *sql2 = "SELECT ?";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Bind the input data as a blob
    if (Size > 0 && Data != NULL) {
        // Cap the size to a reasonable maximum for binding
        int bind_size = (Size > 1000000) ? 1000000 : (int)Size;
        sqlite3_bind_blob(stmt2, 1, Data, bind_size, SQLITE_TRANSIENT);
    } else {
        static const char default_data[] = "test";
        sqlite3_bind_text(stmt2, 1, default_data, -1, SQLITE_STATIC);
    }
    
    // Step to get the value
    if (sqlite3_step(stmt2) == SQLITE_ROW) {
        // Get the sqlite3_value from the first column
        sqlite3_value *val = sqlite3_column_value(stmt2, 0);
        
        if (val && ctx) {
            // Call the target API functions in the specified order
            
            // 1. sqlite3_value_int
            int int_result = sqlite3_value_int(val);
            (void)int_result; // Suppress unused variable warning
            
            // 2. sqlite3_value_bytes
            int bytes_result = sqlite3_value_bytes(val);
            (void)bytes_result; // Suppress unused variable warning
            
            // 3. sqlite3_result_value
            sqlite3_result_value(ctx, val);
            
            // 4. sqlite3_value_blob
            const void *blob_result = sqlite3_value_blob(val);
            (void)blob_result; // Suppress unused variable warning
            
            // 5. sqlite3_malloc
            // Use the size from our input data or a small default
            int alloc_size = (Size > 0 && Size < 1000000) ? (int)Size : 100;
            void *allocated_mem = sqlite3_malloc(alloc_size);
            
            // 6. sqlite3_result_error
            // Create an error message from the input data if possible
            const char *error_msg = "Fuzzer generated error";
            if (Size > 0 && Data != NULL) {
                // Use up to 100 bytes of input for error message
                size_t error_len = (Size < 100) ? Size : 100;
                char *temp_error = (char *)malloc(error_len + 1);
                if (temp_error) {
                    memcpy(temp_error, Data, error_len);
                    temp_error[error_len] = '\0';
                    sqlite3_result_error(ctx, temp_error, (int)error_len);
                    free(temp_error);
                } else {
                    sqlite3_result_error(ctx, error_msg, -1);
                }
            } else {
                sqlite3_result_error(ctx, error_msg, -1);
            }
            
            // 7. sqlite3_result_blob
            // Use the allocated memory if we have it, otherwise use input data
            if (allocated_mem && Size > 0 && Data != NULL) {
                size_t copy_size = (Size < 1000) ? Size : 1000;
                memcpy(allocated_mem, Data, copy_size);
                sqlite3_result_blob(ctx, allocated_mem, (int)copy_size, SQLITE_TRANSIENT);
            } else if (Data != NULL && Size > 0) {
                size_t use_size = (Size < 1000) ? Size : 1000;
                sqlite3_result_blob(ctx, Data, (int)use_size, SQLITE_TRANSIENT);
            } else {
                static const char default_blob[] = "default";
                sqlite3_result_blob(ctx, default_blob, 7, SQLITE_STATIC);
            }
            
            // 8. sqlite3_free
            sqlite3_free(allocated_mem);
        }
    }
    
    // Clean up
    sqlite3_finalize(stmt2);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
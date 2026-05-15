// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This function will be registered as a user-defined function
    // It will call the target API functions based on static variables
    static const uint8_t *fuzz_data = NULL;
    static size_t fuzz_size = 0;
    
    if (fuzz_size == 0) {
        return;
    }
    
    // Extract a selector from the first byte
    uint8_t selector = fuzz_data[0] % 6;
    
    // Call different API functions based on the selector
    switch (selector) {
        case 0:
            sqlite3_result_error_nomem(ctx);
            break;
            
        case 1: {
            sqlite3 *db_handle = sqlite3_context_db_handle(ctx);
            (void)db_handle;
            break;
        }
            
        case 2:
            if (fuzz_size > 1) {
                const void *error_msg = (const void *)(fuzz_data + 1);
                int msg_len = (fuzz_size - 1) & 0x7FFFFFFF;
                if (msg_len > 0) {
                    sqlite3_result_error16(ctx, error_msg, msg_len);
                }
            }
            break;
            
        case 3: {
            int value = 0;
            if (fuzz_size >= sizeof(int) + 1) {
                memcpy(&value, fuzz_data + 1, sizeof(int));
            } else if (fuzz_size > 1) {
                value = fuzz_data[1];
            }
            sqlite3_result_int(ctx, value);
            break;
        }
            
        case 4:
            sqlite3_result_error_toobig(ctx);
            break;
            
        case 5: {
            int error_code = SQLITE_ERROR;
            if (fuzz_size >= sizeof(int) + 1) {
                memcpy(&error_code, fuzz_data + 1, sizeof(int));
            } else if (fuzz_size > 1) {
                error_code = fuzz_data[1];
            }
            sqlite3_result_error_code(ctx, error_code);
            break;
        }
    }
    
    // Try to explore more states by calling multiple functions
    if (fuzz_size > 10) {
        sqlite3 *db_handle = sqlite3_context_db_handle(ctx);
        (void)db_handle;
        
        int another_value = 0;
        if (fuzz_size > 20) {
            memcpy(&another_value, fuzz_data + 10, sizeof(int));
        }
        sqlite3_result_int(ctx, another_value);
    }
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a real database connection to test with actual context
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Store fuzz data in static variables for the dummy function
    static uint8_t *static_data = NULL;
    static size_t static_size = 0;
    
    // Free previous data if any
    if (static_data != NULL) {
        free(static_data);
    }
    
    // Allocate and copy new data
    static_data = (uint8_t *)malloc(Size);
    if (static_data == NULL) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(static_data, Data, Size);
    static_size = Size;
    
    // Register a dummy function
    rc = sqlite3_create_function(
        db,
        "dummy_func",
        0,  // Number of arguments
        SQLITE_UTF8,
        NULL,  // User data
        dummy_function,
        NULL,
        NULL
    );
    
    if (rc == SQLITE_OK) {
        // Prepare and execute a statement that calls our function
        sqlite3_stmt *stmt = NULL;
        const char *sql = "SELECT dummy_func()";
        
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        
        if (rc == SQLITE_OK && stmt != NULL) {
            // Step once to invoke the function
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    // Also test the functions with a NULL context (should not crash)
    if (Size > 50) {
        // These calls with NULL context should be safe
        sqlite3_result_error_nomem(NULL);
        sqlite3_context_db_handle(NULL);
        sqlite3_result_error16(NULL, Data, Size & 0x7FFFFFFF);
        sqlite3_result_int(NULL, 42);
        sqlite3_result_error_toobig(NULL);
        sqlite3_result_error_code(NULL, SQLITE_ERROR);
    }
    
    sqlite3_close(db);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

static void fuzz_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called to provide a valid context
    // We can now call the target API functions from within this callback
    
    // Use the fuzzer input data passed via user_data
    const uint8_t *data = (const uint8_t *)sqlite3_user_data(context);
    size_t size = 0;
    
    // Get size from the second half of user_data pointer
    if (data) {
        // The user_data actually points to a structure containing both data and size
        // We'll use a simple approach: assume first 8 bytes are size, rest is data
        if (data != (void*)1) {  // Special marker for empty data
            memcpy(&size, data, sizeof(size_t));
            data += sizeof(size_t);
        }
    }
    
    // 1. Test sqlite3_result_error_nomem
    sqlite3_result_error_nomem(context);
    
    // 2. Test sqlite3_result_int
    int int_value = 0;
    if (size >= sizeof(int)) {
        memcpy(&int_value, data, sizeof(int));
    } else if (size > 0) {
        int_value = (int)data[0];
    }
    sqlite3_result_int(context, int_value);
    
    // 3. Test sqlite3_result_error_code
    int error_code = 0;
    if (size >= sizeof(int)) {
        size_t offset = size > sizeof(int) ? size/2 : 0;
        memcpy(&error_code, data + offset, sizeof(int));
    } else if (size > 0) {
        error_code = (int)data[0];
    }
    sqlite3_result_error_code(context, error_code);
    
    // 4. Test sqlite3_result_error16
    if (size > 0) {
        sqlite3_result_error16(context, data, (int)(size > 100 ? 100 : size));
    }
    
    // 5. Test sqlite3_context_db_handle
    sqlite3 *db_handle = sqlite3_context_db_handle(context);
    (void)db_handle; // Mark as used to avoid warning
    
    // 6. Test sqlite3_result_error_toobig
    sqlite3_result_error_toobig(context);
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Prepare a buffer to pass both data and size to the user function
    size_t total_size = sizeof(size_t) + Size;
    uint8_t *user_data = (uint8_t *)malloc(total_size);
    if (!user_data) {
        sqlite3_close(db);
        return 0;
    }
    
    // Store size first, then data
    memcpy(user_data, &Size, sizeof(size_t));
    if (Size > 0) {
        memcpy(user_data + sizeof(size_t), Data, Size);
    }
    
    // Create a user-defined function with our fuzzer data as user_data
    rc = sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, 
                                 user_data, fuzz_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        free(user_data);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare and execute a statement that calls our function
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT fuzz_func()";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        // Execute the statement - this will call our fuzz_func with a valid context
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    // Note: user_data will be freed when the function is destroyed
    // We need to explicitly destroy the function to free it
    sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, 
                           NULL, NULL, NULL, NULL);
    free(user_data);
    sqlite3_close(db);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
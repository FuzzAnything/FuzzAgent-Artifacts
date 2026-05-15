// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_pointer at sqlite3.c:78781:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_blob64 at sqlite3.c:78715:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:78837:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Empty destructor for testing
}

static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // Get user data passed during registration
    void *user_data = sqlite3_user_data(context);
    
    // Use the first byte of user_data (if available) to decide which API to test
    uint8_t api_choice = 0;
    if (user_data) {
        api_choice = ((uintptr_t)user_data) & 0x07;
    }
    
    // Prepare test data from the function arguments if available
    const void *test_data = NULL;
    int test_size = 0;
    sqlite3_uint64 test_size64 = 0;
    
    if (argc > 0) {
        test_data = sqlite3_value_blob(argv[0]);
        test_size = sqlite3_value_bytes(argv[0]);
        test_size64 = test_size;
    }
    
    // If no data from arguments, use some default
    if (!test_data || test_size == 0) {
        static const char default_data[] = "test";
        test_data = default_data;
        test_size = sizeof(default_data) - 1;
        test_size64 = test_size;
    }
    
    switch (api_choice) {
        case 0:
            // Test sqlite3_result_blob
            sqlite3_result_blob(context, test_data, test_size, 
                               SQLITE_TRANSIENT);
            break;
            
        case 1:
            // Test sqlite3_result_pointer
            sqlite3_result_pointer(context, (void*)test_data, 
                                  "test_type", 
                                  dummy_destructor);
            break;
            
        case 2:
            // Test sqlite3_result_text
            sqlite3_result_text(context, (const char*)test_data, test_size,
                               SQLITE_TRANSIENT);
            break;
            
        case 3:
            // Test sqlite3_result_blob64
            sqlite3_result_blob64(context, test_data, test_size64,
                                 SQLITE_TRANSIENT);
            break;
            
        case 4:
            // Test sqlite3_user_data - already called above
            // Return the pointer as text for verification
            {
                char buf[64];
                snprintf(buf, sizeof(buf), "%p", user_data);
                sqlite3_result_text(context, buf, -1, SQLITE_TRANSIENT);
            }
            break;
            
        case 5:
            // Test sqlite3_result_text64
            sqlite3_result_text64(context, (const char*)test_data, test_size64,
                                 SQLITE_TRANSIENT,
                                 SQLITE_UTF8);
            break;
            
        default:
            sqlite3_result_null(context);
            break;
    }
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a dummy file for any file operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size < 1024 ? Size : 1024, fp);
        }
        fclose(fp);
    }
    
    // Initialize SQLite in memory
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Use the first byte of input as user data for the function
    void *user_data = (void*)(uintptr_t)Data[0];
    
    // Create a user-defined function
    sqlite3_create_function(
        db,
        "test_func",
        1, // Number of arguments
        SQLITE_UTF8,
        user_data,
        test_function,
        NULL,
        NULL
    );
    
    // Prepare and execute a statement that calls our function
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT test_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the fuzzer input as a blob parameter
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, 
                             Size < 1000 ? (int)Size : 1000, 
                             SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        
        // Execute the statement - this will call our test_function
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // We don't need to do anything with the result
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Also test with different SQL statements
    const char *sql2 = "SELECT test_func(NULL)";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Ignore results
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}
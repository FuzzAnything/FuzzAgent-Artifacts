// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function is just a placeholder for user-defined SQL functions
}

static void fuzz_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called with a valid context from SQLite
    // We'll use the user data to pass fuzzing input
    
    if (argc > 0) {
        const uint8_t *data = (const uint8_t *)sqlite3_user_data(context);
        size_t size = (size_t)sqlite3_value_int(argv[0]);
        
        if (data && size > 0) {
            // Use the fuzzer input to decide which functions to call
            uint8_t selector = data[0] % 7; // 6 functions + 1 for db_handle
            
            switch (selector) {
                case 0:
                    sqlite3_result_error_nomem(context);
                    break;
                case 1:
                    if (size > 1) {
                        int int_val = (int)data[1];
                        sqlite3_result_int(context, int_val);
                    }
                    break;
                case 2:
                    if (size > 1) {
                        int error_code = (int)data[1];
                        sqlite3_result_error_code(context, error_code);
                    }
                    break;
                case 3:
                    if (size > 2) {
                        // Use fuzzer data as UTF-16 error message
                        int length = (size > 254) ? 254 : (int)size - 1;
                        sqlite3_result_error16(context, data + 1, length);
                    }
                    break;
                case 4:
                    {
                        sqlite3 *db_handle = sqlite3_context_db_handle(context);
                        // Do something with the db handle to explore more states
                        if (db_handle) {
                            int err_code = sqlite3_errcode(db_handle);
                            (void)err_code; // Use the value
                        }
                    }
                    break;
                case 5:
                    sqlite3_result_error_toobig(context);
                    break;
                case 6:
                    // Try multiple functions in sequence
                    if (size > 3) {
                        sqlite3_result_int(context, (int)data[1]);
                        sqlite3_result_error_code(context, (int)data[2]);
                    }
                    break;
            }
        }
    }
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    // Initialize database
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *err_msg = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple user-defined function to get a context
    sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, 
                           dummy_function, NULL, NULL);
    
    // Create a fuzzing function that uses our data
    sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, (void*)Data, 
                           fuzz_function, NULL, NULL);
    
    // Prepare and execute a statement to trigger the fuzzing function
    const char *sql = "SELECT fuzz_func(?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the size as parameter
        sqlite3_bind_int(stmt, 1, (int)Size);
        
        // Execute the statement - this will call our fuzz_function with a valid context
        int rc = sqlite3_step(stmt);
        (void)rc; // Use the return code
        
        // Clean up statement
        sqlite3_finalize(stmt);
    }
    
    // Also test with a simple dummy function to ensure we get a context
    if (sqlite3_prepare_v2(db, "SELECT dummy_func()", -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Create a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
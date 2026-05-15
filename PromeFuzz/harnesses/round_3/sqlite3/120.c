// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Global variables to store fuzzer data for the callback function
static const uint8_t *g_fuzzer_data = NULL;
static size_t g_fuzzer_size = 0;

// Destructor for auxiliary data
static void cleanup_auxdata(void *data) {
    // Do nothing since we're fuzzing
}

// Callback function that will be called by SQLite
static void fuzz_callback(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (g_fuzzer_data == NULL || g_fuzzer_size < 1) {
        sqlite3_result_null(context);
        return;
    }
    
    // Use the fuzzer data to determine which functions to call
    unsigned char selector = g_fuzzer_data[0] % 6;
    size_t data_offset = 1;
    if (data_offset >= g_fuzzer_size) data_offset = 0;
    
    switch (selector) {
        case 0: {
            // Test sqlite3_result_error
            const char *error_msg = (const char *)(g_fuzzer_data + data_offset);
            int error_len = (int)(g_fuzzer_size - data_offset);
            if (error_len > 1000) error_len = 1000;
            sqlite3_result_error(context, error_msg, error_len);
            break;
        }
        
        case 1: {
            // Test sqlite3_result_blob
            const void *blob_data = g_fuzzer_data + data_offset;
            int blob_size = (int)(g_fuzzer_size - data_offset);
            if (blob_size > 1000) blob_size = 1000;
            
            // Use different destructor modes based on data
            void (*destructor)(void*) = NULL;
            if (g_fuzzer_size > 1 && (g_fuzzer_data[1] % 3 == 0)) {
                destructor = SQLITE_STATIC;
            } else if (g_fuzzer_size > 1 && (g_fuzzer_data[1] % 3 == 1)) {
                destructor = SQLITE_TRANSIENT;
            } else {
                destructor = cleanup_auxdata;
            }
            
            sqlite3_result_blob(context, blob_data, blob_size, destructor);
            break;
        }
        
        case 2: {
            // Test sqlite3_result_zeroblob
            int n = 0;
            if (g_fuzzer_size - data_offset >= sizeof(int)) {
                n = *((int*)(g_fuzzer_data + data_offset));
                if (n < 0) n = 0;
                if (n > 10000) n = 10000;
            } else if (g_fuzzer_size > data_offset) {
                n = (int)(g_fuzzer_data[data_offset] * 10);
            }
            sqlite3_result_zeroblob(context, n);
            break;
        }
        
        case 3: {
            // Test sqlite3_get_auxdata and sqlite3_set_auxdata
            int N = 0;
            if (g_fuzzer_size - data_offset >= sizeof(int)) {
                N = *((int*)(g_fuzzer_data + data_offset));
            } else if (g_fuzzer_size > data_offset) {
                N = (int)g_fuzzer_data[data_offset];
            }
            
            // Set some auxiliary data
            static const char *test_data = "test_aux_data";
            sqlite3_set_auxdata(context, N % 10, (void*)test_data, cleanup_auxdata);
            
            // Get it back
            void *auxdata = sqlite3_get_auxdata(context, N % 10);
            (void)auxdata; // Use the result
            sqlite3_result_null(context);
            break;
        }
        
        case 4: {
            // Test sqlite3_result_value
            if (argc > 0) {
                sqlite3_result_value(context, argv[0]);
            } else {
                sqlite3_result_null(context);
            }
            break;
        }
        
        case 5: {
            // Test sqlite3_result_zeroblob64
            sqlite3_uint64 n = 0;
            if (g_fuzzer_size - data_offset >= sizeof(sqlite3_uint64)) {
                n = *((sqlite3_uint64*)(g_fuzzer_data + data_offset));
                if (n > 100000) n = 100000;
            } else if (g_fuzzer_size > data_offset) {
                n = (sqlite3_uint64)g_fuzzer_data[data_offset] * 100;
            }
            sqlite3_result_zeroblob64(context, n);
            break;
        }
        
        default:
            sqlite3_result_null(context);
            break;
    }
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Store the fuzzer data in global variables for the callback
    g_fuzzer_data = Data;
    g_fuzzer_size = Size;
    
    // Open an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a user-defined function that will use our callback
    if (sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, 
                                fuzz_callback, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare and execute a statement that calls our function
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT fuzz_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind some data to the parameter (use a small portion of fuzzer data)
    int bind_size = Size > 100 ? 100 : (int)Size;
    sqlite3_bind_blob(stmt, 1, Data, bind_size, SQLITE_TRANSIENT);
    
    // Execute the statement (this will call our callback function)
    sqlite3_step(stmt);
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    // Reset global variables
    g_fuzzer_data = NULL;
    g_fuzzer_size = 0;
    
    return 0;
}
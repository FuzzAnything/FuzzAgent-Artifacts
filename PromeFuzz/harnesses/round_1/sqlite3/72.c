// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "sqlite3.h"

// Global variables to store test data for callback functions
static int g_test_n = 0;
static const void *g_test_blob_data = NULL;
static int g_test_blob_size = 0;
static void (*g_test_destructor)(void*) = NULL;
static sqlite3_uint64 g_test_n64 = 0;

// Callback functions for each test case
static void test_zeroblob_callback(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_zeroblob(context, g_test_n);
}

static void test_error_nomem_callback(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_error_nomem(context);
}

static void test_blob_callback(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_blob(context, g_test_blob_data, g_test_blob_size, g_test_destructor);
}

static void test_null_callback(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_null(context);
}

static void test_error_toobig_callback(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_error_toobig(context);
}

static void test_zeroblob64_callback(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_zeroblob64(context, g_test_n64);
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    // Initialize SQLite once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        initialized = 1;
    }
    
    if (Size < 1) {
        return 0;
    }
    
    // Use first byte to determine which function to test
    uint8_t func_selector = Data[0] % 6;
    size_t data_offset = 1;
    
    // Create a unique function name
    static int func_counter = 0;
    char func_name[64];
    snprintf(func_name, sizeof(func_name), "fuzz_func_%d", func_counter++);
    
    // Set up test data based on selector
    switch (func_selector) {
        case 0: {
            // Test sqlite3_result_zeroblob
            g_test_n = 0;
            if (Size - data_offset >= sizeof(int)) {
                memcpy(&g_test_n, Data + data_offset, sizeof(int));
            } else if (Size - data_offset > 0) {
                g_test_n = Data[data_offset];
            }
            
            sqlite3_create_function(db, func_name, 0, SQLITE_UTF8, NULL, 
                                   test_zeroblob_callback, NULL, NULL);
            break;
        }
        
        case 1: {
            // Test sqlite3_result_error_nomem
            sqlite3_create_function(db, func_name, 0, SQLITE_UTF8, NULL, 
                                   test_error_nomem_callback, NULL, NULL);
            break;
        }
        
        case 2: {
            // Test sqlite3_result_blob
            g_test_blob_data = NULL;
            g_test_blob_size = 0;
            g_test_destructor = SQLITE_TRANSIENT;
            
            if (Size - data_offset > 0) {
                g_test_blob_data = Data + data_offset;
                if ((Size - data_offset) > (size_t)INT_MAX) {
                    g_test_blob_size = INT_MAX;
                } else {
                    g_test_blob_size = (int)(Size - data_offset);
                }
                
                // Use different destructors based on input
                if (Size > 2 && Data[1] % 3 == 0) {
                    g_test_destructor = SQLITE_STATIC;
                } else if (Size > 2 && Data[1] % 3 == 1) {
                    g_test_destructor = SQLITE_TRANSIENT;
                }
            }
            
            sqlite3_create_function(db, func_name, 0, SQLITE_UTF8, NULL, 
                                   test_blob_callback, NULL, NULL);
            break;
        }
        
        case 3: {
            // Test sqlite3_result_null
            sqlite3_create_function(db, func_name, 0, SQLITE_UTF8, NULL, 
                                   test_null_callback, NULL, NULL);
            break;
        }
        
        case 4: {
            // Test sqlite3_result_error_toobig
            sqlite3_create_function(db, func_name, 0, SQLITE_UTF8, NULL, 
                                   test_error_toobig_callback, NULL, NULL);
            break;
        }
        
        case 5: {
            // Test sqlite3_result_zeroblob64
            g_test_n64 = 0;
            if (Size - data_offset >= sizeof(sqlite3_uint64)) {
                memcpy(&g_test_n64, Data + data_offset, sizeof(sqlite3_uint64));
            } else if (Size - data_offset > 0) {
                g_test_n64 = Data[data_offset];
            }
            
            sqlite3_create_function(db, func_name, 0, SQLITE_UTF8, NULL, 
                                   test_zeroblob64_callback, NULL, NULL);
            break;
        }
    }
    
    // Call the function to trigger the API
    char call_sql[128];
    snprintf(call_sql, sizeof(call_sql), "SELECT %s()", func_name);
    
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, call_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Clean up the function
    char drop_sql[128];
    snprintf(drop_sql, sizeof(drop_sql), "DROP FUNCTION IF EXISTS %s", func_name);
    sqlite3_exec(db, drop_sql, NULL, NULL, NULL);
    
    return 0;
}
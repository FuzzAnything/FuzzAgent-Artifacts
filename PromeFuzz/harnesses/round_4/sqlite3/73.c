// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - used for testing
}

static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

static void test_func_with_context(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (g_fuzz_size == 0) {
        sqlite3_result_null(ctx);
        return;
    }
    
    uint8_t opcode = g_fuzz_data[0] % 6;
    size_t data_offset = 1;
    
    switch (opcode) {
        case 0:  // sqlite3_result_blob
            if (g_fuzz_size > data_offset) {
                size_t blob_size = (g_fuzz_size - data_offset) % 256;
                if (blob_size > 0) {
                    void (*destructor)(void*) = NULL;
                    uint8_t dest_type = g_fuzz_data[data_offset] % 3;
                    switch (dest_type) {
                        case 0: destructor = SQLITE_STATIC; break;
                        case 1: destructor = SQLITE_TRANSIENT; break;
                        case 2: destructor = dummy_destructor; break;
                    }
                    sqlite3_result_blob(ctx, g_fuzz_data + data_offset, 
                                      (int)blob_size, destructor);
                }
            }
            break;
            
        case 1:  // sqlite3_result_null
            sqlite3_result_null(ctx);
            break;
            
        case 2:  // sqlite3_result_int64
            if (g_fuzz_size >= data_offset + 8) {
                sqlite3_int64 value = 0;
                memcpy(&value, g_fuzz_data + data_offset, 8);
                sqlite3_result_int64(ctx, value);
            } else if (g_fuzz_size > data_offset) {
                sqlite3_int64 value = g_fuzz_data[data_offset];
                sqlite3_result_int64(ctx, value);
            }
            break;
            
        case 3:  // sqlite3_result_subtype
            if (g_fuzz_size > data_offset) {
                unsigned int subtype = g_fuzz_data[data_offset];
                // First set a result, then set subtype
                sqlite3_result_int64(ctx, 42);
                sqlite3_result_subtype(ctx, subtype);
            }
            break;
            
        case 4:  // sqlite3_result_value
            // Use the first argument as the value if available
            if (argc > 0) {
                sqlite3_result_value(ctx, argv[0]);
            }
            break;
            
        case 5:  // sqlite3_result_error_nomem
            sqlite3_result_error_nomem(ctx);
            break;
    }
}

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This function will be called by SQLite with a valid context
    // We'll use the fuzzer data to decide which API to test
    // The context is provided by SQLite, so it's valid
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy context by registering and calling a function
    sqlite3_create_function(db, "test_func", 0, SQLITE_UTF8, NULL, test_function, NULL, NULL);
    
    // Prepare a statement that calls our function
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT test_func()";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Step once to create the context
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Set global variables for the test function
            g_fuzz_data = Data;
            g_fuzz_size = Size;
            
            // Create a function that uses our test function
            sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, NULL, 
                                   test_func_with_context, NULL, NULL);
            
            // Prepare and execute a statement that calls fuzz_func
            sqlite3_stmt *fuzz_stmt = NULL;
            const char *fuzz_sql = "SELECT fuzz_func()";
            if (sqlite3_prepare_v2(db, fuzz_sql, -1, &fuzz_stmt, NULL) == SQLITE_OK) {
                sqlite3_step(fuzz_stmt);
                sqlite3_finalize(fuzz_stmt);
            }
            
            // Also test with arguments for sqlite3_result_value
            sqlite3_create_function(db, "fuzz_func_arg", 1, SQLITE_UTF8, NULL,
                                   test_func_with_context, NULL, NULL);
            
            sqlite3_stmt *arg_stmt = NULL;
            const char *arg_sql = "SELECT fuzz_func_arg(?)";
            if (sqlite3_prepare_v2(db, arg_sql, -1, &arg_stmt, NULL) == SQLITE_OK) {
                // Bind different types of values to test sqlite3_result_value
                if (Size > 1) {
                    uint8_t bind_type = Data[1] % 4;
                    switch (bind_type) {
                        case 0:
                            sqlite3_bind_int(arg_stmt, 1, 42);
                            break;
                        case 1:
                            sqlite3_bind_text(arg_stmt, 1, "test", 4, SQLITE_TRANSIENT);
                            break;
                        case 2:
                            sqlite3_bind_blob(arg_stmt, 1, Data, (int)(Size % 100), SQLITE_TRANSIENT);
                            break;
                        case 3:
                            sqlite3_bind_null(arg_stmt, 1);
                            break;
                    }
                }
                sqlite3_step(arg_stmt);
                sqlite3_finalize(arg_stmt);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
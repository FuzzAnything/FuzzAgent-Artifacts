// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Helper function to create a protected sqlite3_value
static sqlite3_value* create_protected_value(const uint8_t* data, size_t size) {
    sqlite3* db = NULL;
    sqlite3_value* result = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    
    // Create a table and insert the fuzzer data
    char* err_msg = NULL;
    const char* create_table = "CREATE TABLE test(data TEXT);";
    if (sqlite3_exec(db, create_table, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NULL;
    }
    
    // Prepare statement to insert data
    sqlite3_stmt* insert_stmt = NULL;
    const char* insert_sql = "INSERT INTO test(data) VALUES(?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Bind the fuzzer data
    sqlite3_bind_blob(insert_stmt, 1, data, (int)size, SQLITE_STATIC);
    if (sqlite3_step(insert_stmt) != SQLITE_DONE) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        return NULL;
    }
    sqlite3_finalize(insert_stmt);
    
    // Prepare statement to select the data
    sqlite3_stmt* select_stmt = NULL;
    const char* select_sql = "SELECT data FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Get the value from the first column
    if (sqlite3_step(select_stmt) == SQLITE_ROW) {
        // Get an unprotected value from the column
        sqlite3_value* unprotected_value = sqlite3_column_value(select_stmt, 0);
        
        // Create a protected duplicate
        result = sqlite3_value_dup(unprotected_value);
    }
    
    sqlite3_finalize(select_stmt);
    sqlite3_close(db);
    
    return result;
}

// User-defined function that will be called to use sqlite3_result_text
static void test_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    // Get text from the first argument
    const unsigned char* text = sqlite3_value_text(argv[0]);
    if (text) {
        // Use SQLITE_TRANSIENT so SQLite makes its own copy
        sqlite3_result_text(ctx, (const char*)text, -1, SQLITE_TRANSIENT);
    } else {
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Ensure we have some data
    if (Size == 0) return 0;
    
    // 1. Create a protected sqlite3_value for sqlite3_value_text
    sqlite3_value* value = create_protected_value(Data, Size);
    if (!value) return 0;
    
    // 2. Call sqlite3_value_text
    const unsigned char* text_result = sqlite3_value_text(value);
    
    // 3. Call sqlite3_malloc64 - use Size as allocation size
    sqlite3_uint64 alloc_size = (sqlite3_uint64)Size;
    void* allocated_mem = sqlite3_malloc64(alloc_size);
    
    // 4. Call sqlite3_result_text via a user-defined function
    sqlite3* db = NULL;
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Register our test function
        sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                               test_func, NULL, NULL);
        
        // Prepare a statement that calls our function with the fuzzer data
        sqlite3_stmt* stmt = NULL;
        const char* sql = "SELECT test_func(?);";
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            // Bind the fuzzer data as a blob
            sqlite3_bind_blob(stmt, 1, Data, (int)Size, SQLITE_STATIC);
            
            // Execute the statement (this will call test_func which uses sqlite3_result_text)
            sqlite3_step(stmt);
            
            sqlite3_finalize(stmt);
        }
        
        sqlite3_close(db);
    }
    
    // 5. Call sqlite3_free on the allocated memory
    if (allocated_mem) {
        sqlite3_free(allocated_mem);
    }
    
    // Cleanup the value
    if (value) {
        sqlite3_value_free(value);
    }
    
    return 0;
}
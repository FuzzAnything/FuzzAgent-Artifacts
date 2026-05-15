// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Destructor function for sqlite3_result_text
static void dummy_destructor(void *ptr) {
    // Empty destructor
}

// User-defined function implementation
static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // Call sqlite3_value_text on the first argument (if available)
    const unsigned char *text_result = NULL;
    if (argc > 0) {
        text_result = sqlite3_value_text(argv[0]);
    }
    
    // Call sqlite3_malloc64 with size based on input
    sqlite3_uint64 alloc_size = 1024; // Reasonable default size
    if (text_result) {
        // Use the length of the text as allocation size (capped)
        size_t text_len = strlen((const char*)text_result);
        alloc_size = text_len > 65536 ? 65536 : text_len;
    }
    void *allocated_mem = sqlite3_malloc64(alloc_size);
    
    // Call sqlite3_result_text
    if (text_result) {
        sqlite3_result_text(context, (const char*)text_result, -1, dummy_destructor);
    } else {
        sqlite3_result_text(context, "default", 7, dummy_destructor);
    }
    
    // Call sqlite3_free to free the allocated memory
    sqlite3_free(allocated_mem);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a user-defined function to get a valid sqlite3_context
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                           test_function, NULL, NULL);
    
    // Create a simple table and insert a row with fuzzer data
    char *err_msg = NULL;
    const char *create_table_sql = "CREATE TABLE test (data TEXT);";
    if (sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare insert statement
    sqlite3_stmt *insert_stmt = NULL;
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind fuzzer data as text (limit size to avoid excessive memory)
    size_t data_size = Size > 1024 ? 1024 : Size;
    sqlite3_bind_text(insert_stmt, 1, (const char*)Data, (int)data_size, SQLITE_TRANSIENT);
    sqlite3_step(insert_stmt);
    sqlite3_finalize(insert_stmt);
    
    // Prepare select statement to call our test function
    sqlite3_stmt *select_stmt = NULL;
    const char *select_sql = "SELECT test_func(data) FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Execute the statement - this will invoke our test_function
    // which calls all the target API functions in the required order
    sqlite3_step(select_stmt);
    sqlite3_finalize(select_stmt);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
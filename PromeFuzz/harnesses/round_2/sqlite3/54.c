// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_destructor(void *ptr) {
    // Do nothing - used for sqlite3_bind_text16
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Create in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy table and insert some data
    char *err_msg = NULL;
    const char *create_table_sql = 
        "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);"
        "INSERT INTO test_table (name, value) VALUES ('test1', 1.5);"
        "INSERT INTO test_table (name, value) VALUES ('test2', 2.7);"
        "INSERT INTO test_table (name, value) VALUES ('test3', 3.9);";
    
    if (sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement
    sqlite3_stmt *stmt = NULL;
    const char *select_sql = "SELECT * FROM test_table;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer data to determine column index (modulo column count)
    int col_count = sqlite3_column_count(stmt);
    int col_index = 0;
    if (Size > 0 && col_count > 0) {
        col_index = Data[0] % col_count;
    }
    
    // Test sqlite3_column_count
    int test_col_count = sqlite3_column_count(stmt);
    
    // Step through the statement to access data
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Test sqlite3_column_name16
        const void *col_name = sqlite3_column_name16(stmt, col_index);
        
        // Test sqlite3_column_bytes16
        int bytes_count = sqlite3_column_bytes16(stmt, col_index);
        
        // Test sqlite3_column_text16
        const void *col_text = sqlite3_column_text16(stmt, col_index);
        
        // Test sqlite3_column_table_name16
        const void *table_name = sqlite3_column_table_name16(stmt, col_index);
        
        // Reset statement for next iteration
        sqlite3_reset(stmt);
    }
    
    // Prepare another statement with parameters for binding test
    sqlite3_stmt *bind_stmt = NULL;
    const char *insert_sql = "INSERT INTO test_table (name, value) VALUES (?, 4.2);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &bind_stmt, NULL) == SQLITE_OK) {
        // Test sqlite3_bind_text16 with fuzzer data
        if (Size > 0) {
            // Use first part of data as UTF-16 text (converted from UTF-8 for simplicity)
            // Note: This is a simplified conversion - real UTF-16 would need proper handling
            int bind_len = (Size > 100) ? 100 : Size;
            char *bind_data = (char*)malloc(bind_len * 2);
            if (bind_data) {
                // Simple conversion: each byte becomes 2-byte UTF-16LE (not correct but for fuzzing)
                for (int i = 0; i < bind_len; i++) {
                    bind_data[i*2] = Data[i % Size];
                    bind_data[i*2 + 1] = 0;
                }
                
                sqlite3_bind_text16(bind_stmt, 1, bind_data, bind_len * 2, dummy_destructor);
                free(bind_data);
                
                // Execute the bind statement
                sqlite3_step(bind_stmt);
                sqlite3_reset(bind_stmt);
            }
        }
        sqlite3_finalize(bind_stmt);
    }
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
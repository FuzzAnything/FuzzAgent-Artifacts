// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:173158:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int trace_callback(unsigned mask, void* context, void* p, void* x) {
    // Simple trace callback that does nothing
    return 0;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Open an in-memory database
    sqlite3* db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy file if needed (not used in this case but following instructions)
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        fclose(dummy);
    }
    
    // Use first byte to control extended result codes
    if (Size > 0) {
        int enable_extended = Data[0] & 1;
        sqlite3_extended_result_codes(db, enable_extended);
    }
    
    // Use second byte for trace mask
    unsigned trace_mask = 0;
    if (Size > 1) {
        trace_mask = Data[1];
        // Only set trace if mask is non-zero
        if (trace_mask != 0) {
            sqlite3_trace_v2(db, trace_mask, trace_callback, NULL);
        }
    }
    
    // Create a simple table
    char* create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    // Insert some data
    char* insert_sql = "INSERT INTO test VALUES (1, 'test1'), (2, 'test2');";
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    
    // Prepare a statement using the fuzzer input as SQL (with bounds checking)
    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
    
    if (Size > 2) {
        // Use remaining bytes as SQL query, but ensure null termination
        size_t sql_len = Size - 2;
        if (sql_len > 1024) sql_len = 1024; // Limit to prevent excessive allocation
        
        char* sql = (char*)malloc(sql_len + 1);
        if (sql) {
            memcpy(sql, Data + 2, sql_len);
            sql[sql_len] = '\0';
            
            sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
            free(sql);
        }
    } else {
        // Use a default query if input is too short
        const char* default_sql = "SELECT * FROM test;";
        sqlite3_prepare_v2(db, default_sql, -1, &stmt, &tail);
    }
    
    // If we have a prepared statement, test the other functions
    if (stmt) {
        // Test sqlite3_step
        int step_result = sqlite3_step(stmt);
        
        // If we got a row, test sqlite3_column_type
        if (step_result == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                sqlite3_column_type(stmt, i);
            }
            
            // Step through remaining rows
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Just continue stepping
            }
        }
        
        // Test sqlite3_reset
        sqlite3_reset(stmt);
        
        // Try stepping again after reset
        if (Size > 10) {
            // Use a bit of fuzzer data to decide whether to step again
            if (Data[10] & 1) {
                sqlite3_step(stmt);
            }
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Close the database
    sqlite3_close(db);
    
    return 0;
}
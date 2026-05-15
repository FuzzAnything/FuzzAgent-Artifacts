// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:173158:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int trace_callback(unsigned mask, void* context, void* p1, void* p2) {
    // Simple trace callback that does nothing but return 0
    (void)mask;
    (void)context;
    (void)p1;
    (void)p2;
    return 0;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // Limit input size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }

    // Create a simple table for testing
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Insert some data for testing
    sqlite3_exec(db, "INSERT OR IGNORE INTO test VALUES (1, 'test1');", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT OR IGNORE INTO test VALUES (2, 'test2');", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT OR IGNORE INTO test VALUES (3, NULL);", NULL, NULL, NULL);

    // Use fuzzer input to control various parameters
    unsigned int mask = 0;
    int enable_extended = 0;
    
    if (Size > 0) {
        // Use first byte to determine trace mask
        mask = Data[0] & 0x0F; // Use lower 4 bits for mask
        
        // Use second byte to determine if extended result codes should be enabled
        if (Size > 1) {
            enable_extended = Data[1] & 0x01;
        }
    }

    // Test sqlite3_extended_result_codes
    sqlite3_extended_result_codes(db, enable_extended);

    // Test sqlite3_trace_v2
    sqlite3_trace_v2(db, mask, trace_callback, NULL);

    // Prepare a statement using fuzzer input as SQL (if there's enough data)
    if (Size > 2) {
        // Create a null-terminated string from fuzzer input for SQL
        // Use a maximum of 1024 bytes to avoid huge allocations
        size_t sql_size = Size - 2;
        if (sql_size > 1024) sql_size = 1024;
        
        char *sql = malloc(sql_size + 1);
        if (sql) {
            memcpy(sql, Data + 2, sql_size);
            sql[sql_size] = '\0';
            
            // Test sqlite3_prepare_v2
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
            
            free(sql);
            
            if (rc == SQLITE_OK && stmt != NULL) {
                // Test sqlite3_step
                while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                    int col_count = sqlite3_column_count(stmt);
                    
                    // Test sqlite3_column_type for each column
                    for (int i = 0; i < col_count; i++) {
                        sqlite3_column_type(stmt, i);
                    }
                }
                
                // Test sqlite3_reset
                sqlite3_reset(stmt);
                
                // Finalize the statement
                sqlite3_finalize(stmt);
            }
        }
    } else {
        // Test with a simple SQL statement when input is too small
        rc = sqlite3_prepare_v2(db, "SELECT * FROM test;", -1, &stmt, &tail);
        if (rc == SQLITE_OK && stmt != NULL) {
            // Test sqlite3_step
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                int col_count = sqlite3_column_count(stmt);
                
                // Test sqlite3_column_type for each column
                for (int i = 0; i < col_count; i++) {
                    sqlite3_column_type(stmt, i);
                }
            }
            
            // Test sqlite3_reset
            sqlite3_reset(stmt);
            
            // Finalize the statement
            sqlite3_finalize(stmt);
        }
    }

    // Clean up
    sqlite3_close(db);
    
    return 0;
}
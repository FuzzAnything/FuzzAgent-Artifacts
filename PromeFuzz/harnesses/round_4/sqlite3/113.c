// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int prepare_test_statement(sqlite3 *db, sqlite3_stmt **stmt, const char *sql) {
    return sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    static const char *test_sql = "SELECT ?1, ?2, ?3, ?4, ?5";
    static const char *test_sql2 = "SELECT ?1, ?2, ?3";
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Prepare first statement with 5 parameters
    if (prepare_test_statement(db, &stmt1, test_sql) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare second statement with 3 parameters
    if (prepare_test_statement(db, &stmt2, test_sql2) != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count for first statement
    int param_count = sqlite3_bind_parameter_count(stmt1);
    
    // Use fuzzer data to bind parameters to first statement
    if (Size > 0 && param_count > 0) {
        // Use first byte to determine binding strategy
        uint8_t strategy = Data[0];
        size_t data_offset = 1;
        
        for (int i = 1; i <= param_count && i <= 5; i++) {
            if (data_offset >= Size) break;
            
            int binding_type = (strategy >> ((i-1) * 2)) & 0x3;
            
            switch (binding_type) {
                case 0: // Bind blob64
                    if (data_offset + 8 <= Size) {
                        sqlite3_uint64 blob_size = *(sqlite3_uint64*)(Data + data_offset);
                        blob_size = blob_size % 1024; // Limit size for fuzzing
                        const void *blob_data = (data_offset + 8 + blob_size <= Size) ? 
                                               Data + data_offset + 8 : NULL;
                        sqlite3_bind_blob64(stmt1, i, blob_data, blob_size, SQLITE_TRANSIENT);
                        data_offset += 8 + (blob_data ? blob_size : 0);
                    }
                    break;
                    
                case 1: // Bind text16
                    if (data_offset + 4 <= Size) {
                        int text_len = *(int*)(Data + data_offset) % 256;
                        const void *text_data = (data_offset + 4 + text_len * 2 <= Size) ?
                                               Data + data_offset + 4 : NULL;
                        sqlite3_bind_text16(stmt1, i, text_data, text_len * 2, SQLITE_TRANSIENT);
                        data_offset += 4 + (text_data ? text_len * 2 : 0);
                    }
                    break;
                    
                case 2: // Bind int64
                    if (data_offset + 8 <= Size) {
                        sqlite3_int64 int_val = *(sqlite3_int64*)(Data + data_offset);
                        sqlite3_bind_int64(stmt1, i, int_val);
                        data_offset += 8;
                    }
                    break;
                    
                default: // Bind null (or skip)
                    sqlite3_bind_null(stmt1, i);
                    break;
            }
        }
    }
    
    // Test clear_bindings
    sqlite3_reset(stmt1);
    sqlite3_clear_bindings(stmt1);
    
    // Re-bind with different values
    if (Size > 0) {
        // Simple binding with modulo of data
        for (int i = 1; i <= param_count && i <= 5; i++) {
            size_t offset = (i * 13) % Size;
            sqlite3_bind_int64(stmt1, i, Data[offset]);
        }
    }
    
    // Test transfer_bindings (should fail since stmt2 has different parameter count)
    sqlite3_transfer_bindings(stmt1, stmt2);
    
    // Prepare another statement with same parameter count as stmt1
    sqlite3_stmt *stmt3 = NULL;
    if (prepare_test_statement(db, &stmt3, test_sql) == SQLITE_OK) {
        // This transfer should work
        sqlite3_transfer_bindings(stmt1, stmt3);
        sqlite3_finalize(stmt3);
    }
    
    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}
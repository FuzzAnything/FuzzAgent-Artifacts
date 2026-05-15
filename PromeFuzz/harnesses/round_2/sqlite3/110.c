// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
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

static sqlite3* create_test_db() {
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    
    // Create a test table
    const char* create_table_sql = 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB, text TEXT);";
    char* err_msg = NULL;
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NULL;
    }
    
    return db;
}

static sqlite3_stmt* prepare_test_stmt(sqlite3* db) {
    sqlite3_stmt* stmt = NULL;
    const char* sql = "INSERT INTO test (id, data, text) VALUES (?, ?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return stmt;
}

static void dummy_destructor(void* ptr) {
    // Do nothing - just a placeholder destructor
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Create database and prepared statements
    sqlite3* db = create_test_db();
    if (!db) {
        return 0;
    }
    
    sqlite3_stmt* stmt1 = prepare_test_stmt(db);
    sqlite3_stmt* stmt2 = prepare_test_stmt(db);
    if (!stmt1 || !stmt2) {
        if (stmt1) sqlite3_finalize(stmt1);
        if (stmt2) sqlite3_finalize(stmt2);
        sqlite3_close(db);
        return 0;
    }
    
    // Use the first byte to determine which API to test
    uint8_t api_selector = Data[0] % 6;
    
    switch (api_selector) {
        case 0: {
            // Test sqlite3_bind_null
            int param_index = (Data[1] % 3) + 1;  // Parameters are 1-indexed
            sqlite3_bind_null(stmt1, param_index);
            break;
        }
        
        case 1: {
            // Test sqlite3_transfer_bindings
            // First bind some values to stmt1
            for (int i = 1; i <= 3; i++) {
                if (i % 2 == 0) {
                    sqlite3_bind_null(stmt1, i);
                } else {
                    sqlite3_bind_int64(stmt1, i, i * 100);
                }
            }
            sqlite3_transfer_bindings(stmt1, stmt2);
            break;
        }
        
        case 2: {
            // Test sqlite3_clear_bindings
            // First bind some values
            for (int i = 1; i <= 3; i++) {
                sqlite3_bind_int64(stmt1, i, i * 200);
            }
            sqlite3_clear_bindings(stmt1);
            break;
        }
        
        case 3: {
            // Test sqlite3_bind_blob64
            int param_index = (Data[1] % 3) + 1;
            size_t blob_size = Size > 2 ? (Size - 2) : 0;
            const void* blob_data = Size > 2 ? Data + 2 : NULL;
            sqlite3_bind_blob64(stmt1, param_index, blob_data, 
                               (sqlite3_uint64)blob_size, dummy_destructor);
            break;
        }
        
        case 4: {
            // Test sqlite3_bind_text16
            int param_index = (Data[1] % 3) + 1;
            size_t text_size = Size > 2 ? (Size - 2) : 0;
            const void* text_data = Size > 2 ? Data + 2 : NULL;
            sqlite3_bind_text16(stmt1, param_index, text_data, 
                               (int)text_size, dummy_destructor);
            break;
        }
        
        case 5: {
            // Test sqlite3_bind_zeroblob64
            int param_index = (Data[1] % 3) + 1;
            sqlite3_uint64 zero_blob_size = 0;
            if (Size >= 10) {
                // Use bytes 2-9 to form a 64-bit size
                memcpy(&zero_blob_size, Data + 2, sizeof(zero_blob_size));
            } else {
                zero_blob_size = (sqlite3_uint64)Data[1] * 100;
            }
            sqlite3_bind_zeroblob64(stmt1, param_index, zero_blob_size);
            break;
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}
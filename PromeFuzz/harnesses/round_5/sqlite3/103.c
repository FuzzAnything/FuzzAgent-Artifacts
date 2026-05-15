// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - just a placeholder destructor
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t1(a, b, c, d, e);";
    const char *insert_sql = "INSERT INTO t1 VALUES(?, ?, ?, ?, ?);";
    const char *select_sql = "SELECT * FROM t1 WHERE a=? AND b=?;";
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare first statement
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt1, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare second statement
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt2, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count for stmt1
    int param_count = sqlite3_bind_parameter_count(stmt1);
    if (param_count <= 0) {
        sqlite3_finalize(stmt1);
        sqlite3_finalize(stmt2);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer data to determine binding parameters
    if (Size > 0) {
        // Determine which parameter to bind (1-indexed)
        int param_index = (Data[0] % param_count) + 1;
        if (param_index < 1) param_index = 1;
        
        // Determine operation based on second byte
        int operation = Size > 1 ? Data[1] % 6 : 0;
        
        switch (operation) {
            case 0: {
                // sqlite3_bind_blob64
                sqlite3_uint64 blob_size = 0;
                if (Size > 10) {
                    // Use some bytes to determine blob size
                    memcpy(&blob_size, Data + 2, sizeof(blob_size) < Size - 2 ? sizeof(blob_size) : Size - 2);
                    blob_size = blob_size % 65536; // Limit size for fuzzing
                }
                const void *blob_data = Size > 2 ? Data + 10 : NULL;
                size_t available_data = Size > 10 ? Size - 10 : 0;
                sqlite3_bind_blob64(stmt1, param_index, blob_data, 
                                   blob_size < available_data ? blob_size : available_data, 
                                   dummy_destructor);
                break;
            }
            case 1: {
                // sqlite3_bind_text16
                int text_len = 0;
                if (Size > 2) {
                    text_len = Data[2] * 2; // UTF-16 uses 2 bytes per character
                    text_len = text_len % 256; // Limit size
                }
                const void *text_data = Size > 3 ? Data + 3 : NULL;
                size_t available_data = Size > 3 ? Size - 3 : 0;
                sqlite3_bind_text16(stmt1, param_index, text_data,
                                   text_len < available_data ? text_len : available_data,
                                   dummy_destructor);
                break;
            }
            case 2: {
                // sqlite3_bind_zeroblob64
                sqlite3_uint64 zero_size = 0;
                if (Size > 2) {
                    memcpy(&zero_size, Data + 2, sizeof(zero_size) < Size - 2 ? sizeof(zero_size) : Size - 2);
                    zero_size = zero_size % 65536; // Limit size for fuzzing
                }
                sqlite3_bind_zeroblob64(stmt1, param_index, zero_size);
                break;
            }
            case 3: {
                // sqlite3_clear_bindings
                sqlite3_clear_bindings(stmt1);
                break;
            }
            case 4: {
                // sqlite3_transfer_bindings from stmt1 to stmt2
                // First bind something to stmt1
                if (Size > 2) {
                    sqlite3_bind_blob64(stmt1, 1, Data + 2, Size - 2 < 100 ? Size - 2 : 100, dummy_destructor);
                }
                sqlite3_transfer_bindings(stmt1, stmt2);
                break;
            }
            case 5: {
                // sqlite3_transfer_bindings from stmt2 to stmt1
                // First bind something to stmt2
                if (Size > 2) {
                    sqlite3_bind_blob64(stmt2, 1, Data + 2, Size - 2 < 100 ? Size - 2 : 100, dummy_destructor);
                }
                sqlite3_transfer_bindings(stmt2, stmt1);
                break;
            }
        }
        
        // Also test bind_parameter_count
        int count1 = sqlite3_bind_parameter_count(stmt1);
        int count2 = sqlite3_bind_parameter_count(stmt2);
        
        // Try to step the statement (ignore errors)
        sqlite3_step(stmt1);
        sqlite3_reset(stmt1);
        
        sqlite3_step(stmt2);
        sqlite3_reset(stmt2);
    }
    
    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}
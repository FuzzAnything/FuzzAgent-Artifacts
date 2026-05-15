// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - just a placeholder destructor
}

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?, ?, ?, ?, ?";
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement with parameters
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count to ensure valid indices
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer data to determine which functions to call and with what parameters
    // We'll use the first byte as a selector for different operations
    if (Size > 0) {
        uint8_t selector = Data[0];
        size_t data_offset = 1;
        
        // Calculate parameter index (1-based)
        int param_index = 1;
        if (Size > 1) {
            param_index = (Data[1] % param_count) + 1;
            data_offset = 2;
        }
        
        // Ensure we don't go out of bounds
        if (param_index < 1) param_index = 1;
        if (param_index > param_count) param_index = param_count;
        
        switch (selector % 6) {
            case 0:
                // Test sqlite3_bind_null
                sqlite3_bind_null(stmt, param_index);
                break;
                
            case 1:
                // Test sqlite3_bind_parameter_count (already called above)
                // Call it again for coverage
                sqlite3_bind_parameter_count(stmt);
                break;
                
            case 2:
                // Test sqlite3_bind_text16
                if (Size > data_offset) {
                    size_t text_size = Size - data_offset;
                    // Ensure even number of bytes for UTF-16
                    if (text_size % 2 != 0) text_size--;
                    if (text_size > 0) {
                        sqlite3_bind_text16(stmt, param_index, 
                                          Data + data_offset, 
                                          text_size, 
                                          SQLITE_TRANSIENT);
                    }
                }
                break;
                
            case 3:
                // Test sqlite3_clear_bindings
                sqlite3_clear_bindings(stmt);
                break;
                
            case 4:
                // Test sqlite3_bind_zeroblob
                if (Size > data_offset) {
                    int blob_size = Data[data_offset] % 1024; // Limit size
                    sqlite3_bind_zeroblob(stmt, param_index, blob_size);
                }
                break;
                
            case 5:
                // Test sqlite3_bind_blob
                if (Size > data_offset) {
                    size_t blob_size = Size - data_offset;
                    sqlite3_bind_blob(stmt, param_index, 
                                    Data + data_offset, 
                                    blob_size, 
                                    SQLITE_TRANSIENT);
                }
                break;
        }
        
        // Try to step through the statement to exercise the bindings
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // Test multiple bindings in sequence
        if (Size > 2) {
            // Bind null to first parameter
            sqlite3_bind_null(stmt, 1);
            
            // Bind text16 to second parameter if we have enough data
            if (Size > 10) {
                size_t text_size = Size > 20 ? 10 : Size - 10;
                if (text_size % 2 != 0) text_size--;
                if (text_size > 0 && param_count >= 2) {
                    sqlite3_bind_text16(stmt, 2, Data + 10, text_size, dummy_destructor);
                }
            }
            
            // Bind blob to third parameter if we have enough data
            if (Size > 20 && param_count >= 3) {
                size_t blob_size = Size - 20;
                if (blob_size > 0) {
                    sqlite3_bind_blob(stmt, 3, Data + 20, blob_size, SQLITE_STATIC);
                }
            }
            
            // Bind zeroblob to fourth parameter if available
            if (param_count >= 4) {
                int zero_size = (Size > 5) ? (Data[5] % 512) : 100;
                sqlite3_bind_zeroblob(stmt, 4, zero_size);
            }
            
            // Clear bindings and rebind
            sqlite3_clear_bindings(stmt);
            
            // Rebind with different values
            if (param_count >= 1) {
                sqlite3_bind_null(stmt, 1);
            }
            
            // Step again
            sqlite3_step(stmt);
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
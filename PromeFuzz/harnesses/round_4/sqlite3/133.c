// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
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
    // Do nothing, just a placeholder destructor
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *dummy_sql = "SELECT ?, ?, ?, ?, ?, ?";
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement with parameters
    rc = sqlite3_prepare_v2(db, dummy_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Use fuzzer data to determine binding parameters
    if (Size > 0) {
        // Use first byte to determine which function to test
        uint8_t func_selector = Data[0];
        size_t data_offset = 1;
        
        // Extract parameters from fuzzer data
        int bind_index = 1;
        if (param_count > 0 && Size > data_offset) {
            bind_index = (Data[data_offset] % param_count) + 1;
            data_offset++;
        }
        
        // Extract size values if needed
        int int_size = 0;
        sqlite3_uint64 uint64_size = 0;
        if (Size > data_offset + sizeof(int)) {
            memcpy(&int_size, Data + data_offset, sizeof(int));
            data_offset += sizeof(int);
        }
        if (Size > data_offset + sizeof(sqlite3_uint64)) {
            memcpy(&uint64_size, Data + data_offset, sizeof(sqlite3_uint64));
            data_offset += sizeof(sqlite3_uint64);
        }
        
        // Extract pointer value if needed
        void *dummy_pointer = NULL;
        if (Size > data_offset + sizeof(void*)) {
            memcpy(&dummy_pointer, Data + data_offset, sizeof(void*));
            data_offset += sizeof(void*);
        }
        
        // Extract blob data if needed
        void *blob_data = NULL;
        if (Size > data_offset) {
            size_t blob_size = Size - data_offset;
            if (blob_size > 0) {
                blob_data = malloc(blob_size);
                if (blob_data) {
                    memcpy(blob_data, Data + data_offset, blob_size);
                }
            }
        }
        
        // Test different functions based on selector
        switch (func_selector % 6) {
            case 0:
                sqlite3_clear_bindings(stmt);
                break;
            case 1:
                sqlite3_bind_zeroblob64(stmt, bind_index, uint64_size);
                break;
            case 2:
                sqlite3_bind_zeroblob(stmt, bind_index, int_size);
                break;
            case 3:
                // Just call bind_parameter_count - it's already called above
                // but call it again to exercise the code path
                sqlite3_bind_parameter_count(stmt);
                break;
            case 4:
                if (blob_data) {
                    sqlite3_bind_blob64(stmt, bind_index, blob_data, 
                                      (sqlite3_uint64)(Size - data_offset), 
                                      dummy_destructor);
                }
                break;
            case 5:
                sqlite3_bind_pointer(stmt, bind_index, dummy_pointer, 
                                   "dummy_type", dummy_destructor);
                break;
        }
        
        // Clean up allocated blob data
        if (blob_data) {
            free(blob_data);
        }
    }
    
    // Reset statement to allow rebinding
    sqlite3_reset(stmt);
    
    // Test clear bindings again
    sqlite3_clear_bindings(stmt);
    
    // Final cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
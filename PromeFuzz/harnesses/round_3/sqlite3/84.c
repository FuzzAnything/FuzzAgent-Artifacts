// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
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
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
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

static void dummy_destructor(void* ptr) {
    // Do nothing - the data is managed elsewhere
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    // Initialize database and statement
    sqlite3* db = NULL;
    sqlite3_stmt* stmt = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple prepared statement with parameters
    const char* sql = "SELECT ?, ?, ?, ?, ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count to ensure we bind within valid range
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzz data to determine binding operations
    // We'll use the first few bytes to decide what to do
    if (Size >= 1) {
        uint8_t operation = Data[0] % 6;
        size_t data_offset = 1;
        
        switch (operation) {
            case 0: {
                // sqlite3_bind_text16
                if (Size >= data_offset + 2) {
                    int param_idx = (Data[data_offset] % param_count) + 1;
                    data_offset++;
                    
                    // Use remaining data as UTF-16 text (simplified - just treat as bytes)
                    int text_len = (Size - data_offset) & ~1; // Ensure even number for UTF-16
                    if (text_len > 0) {
                        const void* text_data = Data + data_offset;
                        sqlite3_bind_text16(stmt, param_idx, text_data, text_len, SQLITE_TRANSIENT);
                    }
                }
                break;
            }
            case 1: {
                // sqlite3_bind_parameter_count - just call it
                sqlite3_bind_parameter_count(stmt);
                break;
            }
            case 2: {
                // sqlite3_bind_zeroblob
                if (Size >= data_offset + 2) {
                    int param_idx = (Data[data_offset] % param_count) + 1;
                    data_offset++;
                    int blob_size = Data[data_offset] * 256 + Data[data_offset + 1];
                    blob_size = blob_size % 65536; // Limit size
                    sqlite3_bind_zeroblob(stmt, param_idx, blob_size);
                }
                break;
            }
            case 3: {
                // sqlite3_bind_blob64
                if (Size >= data_offset + 1) {
                    int param_idx = (Data[data_offset] % param_count) + 1;
                    data_offset++;
                    
                    // Use remaining data as blob
                    sqlite3_uint64 blob_size = Size - data_offset;
                    if (blob_size > 0) {
                        const void* blob_data = Data + data_offset;
                        sqlite3_bind_blob64(stmt, param_idx, blob_data, blob_size, SQLITE_TRANSIENT);
                    }
                }
                break;
            }
            case 4: {
                // sqlite3_clear_bindings
                sqlite3_clear_bindings(stmt);
                break;
            }
            case 5: {
                // sqlite3_bind_null
                if (Size >= data_offset + 1) {
                    int param_idx = (Data[data_offset] % param_count) + 1;
                    sqlite3_bind_null(stmt, param_idx);
                }
                break;
            }
        }
    }
    
    // Try to execute the statement to explore more states
    sqlite3_step(stmt);
    
    // Reset and clear bindings for clean state
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    
    // Try another binding operation with different data pattern
    if (Size >= 2) {
        // Bind to multiple parameters
        for (int i = 1; i <= param_count && i <= 5; i++) {
            uint8_t bind_type = Data[(i-1) % Size] % 4;
            
            switch (bind_type) {
                case 0:
                    sqlite3_bind_null(stmt, i);
                    break;
                case 1: {
                    // Simple text binding (not text16 for variety)
                    const char* dummy_text = "fuzz";
                    sqlite3_bind_text(stmt, i, dummy_text, -1, SQLITE_TRANSIENT);
                    break;
                }
                case 2:
                    sqlite3_bind_zeroblob(stmt, i, 10);
                    break;
                case 3: {
                    // Integer binding
                    int value = Data[(i-1) % Size] * 256 + Data[i % Size];
                    sqlite3_bind_int(stmt, i, value);
                    break;
                }
            }
        }
        
        // Execute again
        sqlite3_step(stmt);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
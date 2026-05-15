// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?, ?1, ?2, :foo, $bar, @baz;";
    const char *tail = NULL;
    int rc;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Prepare statement with parameters
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzz data to determine actions
    if (Size > 0) {
        size_t offset = 0;
        
        // Get parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // Use first byte to determine which functions to call
        uint8_t action = Size > 0 ? Data[0] : 0;
        offset++;
        
        // Call bind functions based on fuzz input
        if (param_count > 0) {
            // Bind integer if we have enough data
            if (offset + 2 <= Size) {
                int idx = (Data[offset] % param_count) + 1;
                int value = (int)Data[offset + 1];
                sqlite3_bind_int(stmt, idx, value);
                offset += 2;
            }
            
            // Bind double if we have enough data
            if (offset + 9 <= Size) {
                int idx = (Data[offset] % param_count) + 1;
                double value;
                memcpy(&value, Data + offset + 1, sizeof(double));
                sqlite3_bind_double(stmt, idx, value);
                offset += 9;
            }
            
            // Bind int64 if we have enough data
            if (offset + 9 <= Size) {
                int idx = (Data[offset] % param_count) + 1;
                sqlite3_int64 value;
                memcpy(&value, Data + offset + 1, sizeof(sqlite3_int64));
                sqlite3_bind_int64(stmt, idx, value);
                offset += 9;
            }
            
            // Test parameter index lookup
            if (offset + 1 <= Size) {
                // Create a simple parameter name from fuzz data
                char param_name[10];
                size_t name_len = Size - offset < 9 ? Size - offset : 9;
                if (name_len > 0) {
                    memcpy(param_name, Data + offset, name_len);
                    param_name[name_len] = '\0';
                    sqlite3_bind_parameter_index(stmt, param_name);
                    offset += name_len;
                }
            }
            
            // Test parameter name retrieval
            if (offset < Size && param_count > 0) {
                int idx = (Data[offset] % param_count) + 1;
                sqlite3_bind_parameter_name(stmt, idx);
                offset++;
            }
        }
        
        // Reset statement to allow rebinding
        sqlite3_reset(stmt);
        
        // Try binding with different indices using remaining data
        while (offset + 1 <= Size && param_count > 0) {
            int idx = (Data[offset] % param_count) + 1;
            int value = (int)Data[offset];
            sqlite3_bind_int(stmt, idx, value);
            offset++;
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
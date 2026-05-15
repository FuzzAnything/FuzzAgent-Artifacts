// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql = "SELECT ?, ?2, :param, $var, @var;";
    const char *tail;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement with various parameter types
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Use fuzzer data to determine binding actions
    if (Size > 0 && param_count > 0) {
        // Use first byte to select parameter index (1-based)
        int param_idx = (Data[0] % param_count) + 1;
        
        // Use remaining data for binding values
        if (Size >= 2) {
            // Bind integer (using 1 byte)
            int int_val = Data[1];
            sqlite3_bind_int(stmt, param_idx, int_val);
        }
        
        if (Size >= 10) {
            // Bind 64-bit integer (using 8 bytes)
            sqlite3_int64 int64_val;
            memcpy(&int64_val, Data + 2, sizeof(sqlite3_int64));
            sqlite3_bind_int64(stmt, param_idx, int64_val);
        }
        
        if (Size >= 18) {
            // Bind double (using 8 bytes)
            double double_val;
            memcpy(&double_val, Data + 10, sizeof(double));
            sqlite3_bind_double(stmt, param_idx, double_val);
        }
        
        // Test parameter name retrieval
        const char *param_name = sqlite3_bind_parameter_name(stmt, param_idx);
        
        // Test parameter index lookup if we have a valid name
        if (param_name && Size >= 20) {
            // Use some fuzzer data to create a test name
            char test_name[32];
            size_t name_len = Size - 20;
            if (name_len > 31) name_len = 31;
            memcpy(test_name, Data + 18, name_len);
            test_name[name_len] = '\0';
            
            int found_idx = sqlite3_bind_parameter_index(stmt, test_name);
            // Try with the actual parameter name too
            if (param_name[0]) {
                found_idx = sqlite3_bind_parameter_index(stmt, param_name);
            }
        }
    }
    
    // Try binding to all parameters with fuzzer-derived values
    if (Size >= 3) {
        for (int i = 1; i <= param_count && i <= 10; i++) {
            // Cycle through fuzzer data for different values
            uint8_t val_idx = (i - 1) * 3;
            if (val_idx + 2 < Size) {
                int int_val = Data[val_idx];
                sqlite3_bind_int(stmt, i, int_val);
                
                // Also test with parameter count function
                int current_count = sqlite3_bind_parameter_count(stmt);
                
                // Test parameter name for this index
                const char *name = sqlite3_bind_parameter_name(stmt, i);
                
                // If we have a name, try to look it up
                if (name && name[0]) {
                    int idx = sqlite3_bind_parameter_index(stmt, name);
                }
            }
        }
    }
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
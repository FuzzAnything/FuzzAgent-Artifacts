// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>

static void dummy_bind_operations(sqlite3_stmt *stmt, const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Use some bytes to determine binding operations
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count <= 0) return;
    
    // Try binding with various indices and values derived from fuzzer input
    for (int i = 0; i < param_count && i < 10; i++) {
        int idx = (i % param_count) + 1;  // Parameters are 1-indexed
        
        // Bind int64 if we have enough data
        if (size >= sizeof(sqlite3_int64) + 1) {
            sqlite3_int64 val;
            memcpy(&val, data, sizeof(val));
            sqlite3_bind_int64(stmt, idx, val);
        }
        
        // Bind int if we have enough data
        if (size >= sizeof(int) + 1) {
            int val;
            memcpy(&val, data, sizeof(val));
            sqlite3_bind_int(stmt, idx, val);
        }
        
        // Bind double if we have enough data
        if (size >= sizeof(double) + 1) {
            double val;
            memcpy(&val, data, sizeof(val));
            sqlite3_bind_double(stmt, idx, val);
        }
        
        // Test parameter name lookup
        const char *name = sqlite3_bind_parameter_name(stmt, idx);
        if (name) {
            sqlite3_bind_parameter_index(stmt, name);
        }
    }
    
    // Test with some dummy parameter names
    sqlite3_bind_parameter_index(stmt, ":dummy");
    sqlite3_bind_parameter_index(stmt, "?1");
    sqlite3_bind_parameter_index(stmt, "@param");
    sqlite3_bind_parameter_index(stmt, "$var");
}

int LLVMFuzzerTestOneInput_100(const uint8_t *data, size_t size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    // Initialize database once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        initialized = 1;
    }
    
    if (!db) return 0;
    
    // Create a simple table
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, value REAL, data TEXT);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    
    // Prepare a statement with parameters
    const char *sql = "INSERT INTO test VALUES (?, ?, ?);";
    sqlite3_stmt *stmt = NULL;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Perform various bind operations
        dummy_bind_operations(stmt, data, size);
        
        // Clean up
        sqlite3_finalize(stmt);
    }
    
    // Prepare another statement with named parameters
    const char *named_sql = "INSERT INTO test VALUES (:id, @value, $data);";
    if (sqlite3_prepare_v2(db, named_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Test parameter count
        int count = sqlite3_bind_parameter_count(stmt);
        
        // Test parameter name functions
        for (int i = 1; i <= count && i <= 5; i++) {
            const char *name = sqlite3_bind_parameter_name(stmt, i);
            if (name) {
                int idx = sqlite3_bind_parameter_index(stmt, name);
                if (idx > 0 && size >= sizeof(int)) {
                    int val;
                    memcpy(&val, data, sizeof(int));
                    sqlite3_bind_int(stmt, idx, val);
                    
                    if (size >= sizeof(sqlite3_int64)) {
                        sqlite3_int64 val64;
                        memcpy(&val64, data, sizeof(val64));
                        sqlite3_bind_int64(stmt, idx, val64);
                    }
                    
                    if (size >= sizeof(double)) {
                        double dval;
                        memcpy(&dval, data, sizeof(dval));
                        sqlite3_bind_double(stmt, idx, dval);
                    }
                }
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Reset database for next iteration
    sqlite3_exec(db, "DELETE FROM test;", NULL, NULL, NULL);
    
    return 0;
}
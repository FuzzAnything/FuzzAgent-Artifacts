// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_exec(sqlite3* db, const char* sql) {
    sqlite3_exec(db, sql, NULL, NULL, NULL);
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy table for testing
    dummy_exec(db, "CREATE TABLE test (id INTEGER, value REAL, data BLOB);");
    
    // Prepare a statement with various parameter types
    const char *sql = "INSERT INTO test VALUES (?, :named1, @named2);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Test sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Use fuzzer data to determine binding indices and values
    if (Size >= 1) {
        // Test sqlite3_bind_parameter_index with a dummy name
        int idx = sqlite3_bind_parameter_index(stmt, ":named1");
        if (idx > 0 && idx <= param_count) {
            // Bind integer using first byte
            sqlite3_bind_int(stmt, idx, (int)Data[0]);
        }
    }
    
    if (Size >= 2) {
        // Test binding at index 1 (first parameter)
        int bind_idx = 1;
        if (bind_idx <= param_count) {
            // Bind integer using second byte
            sqlite3_bind_int(stmt, bind_idx, (int)Data[1]);
        }
    }
    
    if (Size >= 10) {
        // Test sqlite3_bind_double with bytes 2-9 interpreted as double
        double dbl_val;
        memcpy(&dbl_val, Data + 2, sizeof(double));
        int bind_idx = 2;
        if (bind_idx <= param_count) {
            sqlite3_bind_double(stmt, bind_idx, dbl_val);
        }
    }
    
    if (Size >= 18) {
        // Test sqlite3_bind_int64 with bytes 10-17 interpreted as int64
        sqlite3_int64 int64_val;
        memcpy(&int64_val, Data + 10, sizeof(sqlite3_int64));
        int bind_idx = 3;
        if (bind_idx <= param_count) {
            sqlite3_bind_int64(stmt, bind_idx, int64_val);
        }
    }
    
    // Test sqlite3_bind_parameter_name for all parameters
    for (int i = 1; i <= param_count; i++) {
        const char *name = sqlite3_bind_parameter_name(stmt, i);
        (void)name; // Suppress unused variable warning
    }
    
    // Try to execute the statement (ignore errors)
    sqlite3_step(stmt);
    
    // Clean up
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
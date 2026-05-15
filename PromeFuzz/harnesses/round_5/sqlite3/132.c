// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
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

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc != 1) return;
    
    sqlite3_value *val = argv[0];
    
    // Test all target functions
    int bytes16 = sqlite3_value_bytes16(val);
    const void *blob = sqlite3_value_blob(val);
    int type = sqlite3_value_type(val);
    int int_val = sqlite3_value_int(val);
    int num_type = sqlite3_value_numeric_type(val);
    
    // Use results to avoid optimization
    (void)bytes16;
    (void)blob;
    (void)type;
    (void)int_val;
    (void)num_type;
    
    // Test sqlite3_result_value
    sqlite3_result_value(ctx, val);
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    static sqlite3_stmt *stmt = NULL;
    
    // Initialize database once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Register test function
        sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                               test_function, NULL, NULL);
        
        // Prepare a statement that uses our function
        const char *sql = "SELECT test_func(?)";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        initialized = 1;
    }
    
    if (!db || !stmt) return 0;
    
    // Reset statement
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    
    // Bind fuzzer data as different types to explore various code paths
    if (Size > 0) {
        // Try binding as blob
        sqlite3_bind_blob(stmt, 1, Data, Size > 1024 ? 1024 : Size, SQLITE_TRANSIENT);
        
        // Execute
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get result
            sqlite3_value *result = sqlite3_column_value(stmt, 0);
            if (result) {
                // Test functions on result value
                int bytes16 = sqlite3_value_bytes16(result);
                const void *blob = sqlite3_value_blob(result);
                int type = sqlite3_value_type(result);
                int int_val = sqlite3_value_int(result);
                int num_type = sqlite3_value_numeric_type(result);
                
                (void)bytes16;
                (void)blob;
                (void)type;
                (void)int_val;
                (void)num_type;
            }
        }
        
        // Reset and try binding as text
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        
        // Create null-terminated string for text binding
        char *text_data = malloc(Size + 1);
        if (text_data) {
            memcpy(text_data, Data, Size > 1024 ? 1024 : Size);
            text_data[Size > 1024 ? 1024 : Size] = '\0';
            sqlite3_bind_text(stmt, 1, text_data, -1, SQLITE_TRANSIENT);
            free(text_data);
            
            sqlite3_step(stmt); // Ignore result
        }
        
        // Reset and try binding as integer
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        
        if (Size >= sizeof(int)) {
            int int_data;
            memcpy(&int_data, Data, sizeof(int));
            sqlite3_bind_int(stmt, 1, int_data);
            sqlite3_step(stmt); // Ignore result
        }
        
        // Reset and try binding as double
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        
        if (Size >= sizeof(double)) {
            double double_data;
            memcpy(&double_data, Data, sizeof(double));
            sqlite3_bind_double(stmt, 1, double_data);
            sqlite3_step(stmt); // Ignore result
        }
        
        // Reset and try binding as null
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_bind_null(stmt, 1);
        sqlite3_step(stmt); // Ignore result
    }
    
    return 0;
}
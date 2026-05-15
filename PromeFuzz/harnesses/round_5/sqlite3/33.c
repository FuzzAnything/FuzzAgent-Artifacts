// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called to test the API functions
    if (argc < 1) return;
    
    sqlite3_value *value = argv[0];
    
    // Call the target functions in the specified order
    const unsigned char *text_result = sqlite3_value_text(value);
    const void *blob_result = sqlite3_value_blob(value);
    int bytes_result = sqlite3_value_bytes(value);
    
    // Use the results to influence the int64 result
    sqlite3_int64 result = 0;
    if (text_result) {
        result += (sqlite3_int64)text_result[0];
    }
    if (blob_result && bytes_result > 0) {
        result += ((const unsigned char *)blob_result)[0];
    }
    result += bytes_result;
    
    sqlite3_result_int64(context, result);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    // Initialize database once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Register our dummy function
        sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, 
                               dummy_function, NULL, NULL);
        initialized = 1;
    }
    
    if (!db) return 0;
    
    // Create a statement that calls our function with a bound value
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT dummy_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    // Bind the fuzzer input as a blob
    if (sqlite3_bind_blob(stmt, 1, Data, (int)Size, SQLITE_STATIC) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return 0;
    }
    
    // Execute the statement - this will call our dummy_function
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 0;
    }
    
    sqlite3_finalize(stmt);
    
    // Also test with text binding
    stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind as text if we have at least 1 byte
        if (Size > 0) {
            // Ensure null termination for text
            char *text_data = malloc(Size + 1);
            if (text_data) {
                memcpy(text_data, Data, Size);
                text_data[Size] = '\0';
                sqlite3_bind_text(stmt, 1, text_data, -1, free);
                
                rc = sqlite3_step(stmt);
                if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
                    // Don't free text_data here - SQLite will free it
                    sqlite3_finalize(stmt);
                    return 0;
                }
            } else {
                sqlite3_finalize(stmt);
                return 0;
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test with integer binding
    stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_int64 int_value = 0;
        if (Size >= sizeof(sqlite3_int64)) {
            memcpy(&int_value, Data, sizeof(sqlite3_int64));
        } else if (Size > 0) {
            int_value = Data[0];
        }
        sqlite3_bind_int64(stmt, 1, int_value);
        
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return 0;
        }
        sqlite3_finalize(stmt);
    }
    
    // Test with double binding
    stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        double double_value = 0.0;
        if (Size >= sizeof(double)) {
            memcpy(&double_value, Data, sizeof(double));
        } else if (Size > 0) {
            double_value = (double)Data[0];
        }
        sqlite3_bind_double(stmt, 1, double_value);
        
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return 0;
        }
        sqlite3_finalize(stmt);
    }
    
    // Test with NULL binding
    stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_null(stmt, 1);
        
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return 0;
        }
        sqlite3_finalize(stmt);
    }
    
    return 0;
}
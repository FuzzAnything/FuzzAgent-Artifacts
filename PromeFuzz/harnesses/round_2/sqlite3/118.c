// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:78471:25 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:78471:25 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Static UDF function definitions
static void test_udf(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc == 1) {
        // Test sqlite3_result_int64 with a protected value
        sqlite3_int64 val = sqlite3_value_int64(argv[0]);
        sqlite3_result_int64(ctx, val);
    }
}

static void test_udf2(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc == 1) {
        // Test sqlite3_result_value with a protected value
        sqlite3_result_value(ctx, argv[0]);
    }
}

static void test_udf3(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc == 1) {
        // Test sqlite3_value_bytes
        int bytes = sqlite3_value_bytes(argv[0]);
        sqlite3_result_int(ctx, bytes);
    }
}

static void test_udf4(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc == 1) {
        // Test sqlite3_value_blob
        const void *blob = sqlite3_value_blob(argv[0]);
        if (blob) {
            int bytes = sqlite3_value_bytes(argv[0]);
            if (bytes > 0) {
                // Just use the blob pointer to avoid unused variable warning
                (void)blob;
            }
        }
    }
}

static void test_udf5(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc == 1) {
        // Test sqlite3_value_int
        int val = sqlite3_value_int(argv[0]);
        sqlite3_result_int(ctx, val);
    }
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    static sqlite3 *db = NULL;
    static int initialized = 0;
    
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Register test UDFs
        sqlite3_create_function(db, "test_udf", 1, SQLITE_UTF8, NULL, test_udf, NULL, NULL);
        sqlite3_create_function(db, "test_udf2", 1, SQLITE_UTF8, NULL, test_udf2, NULL, NULL);
        sqlite3_create_function(db, "test_udf3", 1, SQLITE_UTF8, NULL, test_udf3, NULL, NULL);
        sqlite3_create_function(db, "test_udf4", 1, SQLITE_UTF8, NULL, test_udf4, NULL, NULL);
        sqlite3_create_function(db, "test_udf5", 1, SQLITE_UTF8, NULL, test_udf5, NULL, NULL);
        
        initialized = 1;
    }
    
    if (!db) {
        return 0;
    }
    
    // Test with different data types and encodings
    for (int test_case = 0; test_case < 5; test_case++) {
        sqlite3_stmt *stmt = NULL;
        const char *sql = NULL;
        
        switch (test_case) {
            case 0:
                // Test with blob data
                sql = "SELECT test_udf(?)";
                break;
            case 1:
                // Test with text data
                sql = "SELECT test_udf2(?)";
                break;
            case 2:
                // Test with integer data
                sql = "SELECT test_udf3(?)";
                break;
            case 3:
                // Test with blob access
                sql = "SELECT test_udf4(?)";
                break;
            case 4:
                // Test with integer conversion
                sql = "SELECT test_udf5(?)";
                break;
        }
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            // Bind the fuzzer data based on test case
            if (Size > 0) {
                int bind_size = (int)(Size > 100 ? 100 : Size);
                
                switch (test_case) {
                    case 0:
                        // Bind as blob
                        sqlite3_bind_blob(stmt, 1, Data, bind_size, SQLITE_TRANSIENT);
                        break;
                    case 1:
                        // Bind as text
                        sqlite3_bind_text(stmt, 1, (const char*)Data, bind_size, SQLITE_TRANSIENT);
                        break;
                    case 2:
                        // Bind as integer (use first 8 bytes if available)
                        if (Size >= sizeof(sqlite3_int64)) {
                            sqlite3_int64 int_val = 0;
                            memcpy(&int_val, Data, sizeof(sqlite3_int64));
                            sqlite3_bind_int64(stmt, 1, int_val);
                        } else {
                            sqlite3_bind_int64(stmt, 1, Size);
                        }
                        break;
                    case 3:
                        // Bind as blob for blob test
                        sqlite3_bind_blob(stmt, 1, Data, bind_size, SQLITE_TRANSIENT);
                        break;
                    case 4:
                        // Bind as integer for int test
                        if (Size >= sizeof(int)) {
                            int int_val = 0;
                            memcpy(&int_val, Data, sizeof(int));
                            sqlite3_bind_int(stmt, 1, int_val);
                        } else {
                            sqlite3_bind_int(stmt, 1, (int)Size);
                        }
                        break;
                }
            } else {
                // Bind NULL for empty input
                sqlite3_bind_null(stmt, 1);
            }
            
            // Execute the statement - this will call our UDF with protected values
            sqlite3_step(stmt);
            
            // Clean up
            sqlite3_finalize(stmt);
        }
    }
    
    // Additional direct tests with protected values
    // Create a protected value using sqlite3_bind_value approach
    sqlite3_stmt *stmt2;
    if (sqlite3_prepare_v2(db, "SELECT ?", -1, &stmt2, NULL) == SQLITE_OK) {
        // Create a temporary value to bind
        sqlite3_value *temp_value = NULL;
        
        if (Size > 0) {
            // Create a value using sqlite3_bind_* and then get it back
            sqlite3_bind_blob(stmt2, 1, Data, (int)(Size > 50 ? 50 : Size), SQLITE_TRANSIENT);
            sqlite3_step(stmt2);
            
            // Get the column value (unprotected)
            sqlite3_value *unprotected = sqlite3_column_value(stmt2, 0);
            
            // Duplicate it to get a protected value
            if (unprotected) {
                temp_value = sqlite3_value_dup(unprotected);
            }
            
            sqlite3_reset(stmt2);
        }
        
        // Test with the protected value if we got one
        if (temp_value) {
            // Test all the target functions
            int bytes = sqlite3_value_bytes(temp_value);
            (void)bytes;
            
            sqlite3_int64 int64_val = sqlite3_value_int64(temp_value);
            (void)int64_val;
            
            const void *blob = sqlite3_value_blob(temp_value);
            (void)blob;
            
            int int_val = sqlite3_value_int(temp_value);
            (void)int_val;
            
            // Clean up
            sqlite3_value_free(temp_value);
        }
        
        sqlite3_finalize(stmt2);
    }
    
    return 0;
}
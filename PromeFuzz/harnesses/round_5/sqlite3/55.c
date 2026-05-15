// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Global database handle for cleanup
static sqlite3 *db = NULL;

// Custom function that will be registered with SQLite
static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called by SQLite and provides a valid sqlite3_context
    // We'll use the fuzzer data passed via a blob parameter
    
    if (argc < 1) {
        sqlite3_result_null(context);
        return;
    }
    
    // Get the fuzzer data from the first argument (should be a blob)
    const uint8_t *data = (const uint8_t *)sqlite3_value_blob(argv[0]);
    int size = sqlite3_value_bytes(argv[0]);
    
    if (size <= 0 || data == NULL) {
        sqlite3_result_null(context);
        return;
    }
    
    // Use the first byte to select which API to test
    uint8_t selector = data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0:
            // Test sqlite3_result_value - use the second argument if available
            if (argc >= 2) {
                sqlite3_result_value(context, argv[1]);
            } else {
                sqlite3_result_null(context);
            }
            break;
            
        case 1:
            // Test sqlite3_result_null
            sqlite3_result_null(context);
            break;
            
        case 2:
            // Test sqlite3_result_double
            if ((size_t)size >= offset + sizeof(double)) {
                double d;
                memcpy(&d, data + offset, sizeof(double));
                sqlite3_result_double(context, d);
            } else {
                sqlite3_result_double(context, 0.0);
            }
            break;
            
        case 3:
            // Test sqlite3_result_subtype
            // First set a result value, then set subtype
            sqlite3_result_int(context, 42);
            if ((size_t)size >= offset + sizeof(unsigned int)) {
                unsigned int subtype;
                memcpy(&subtype, data + offset, sizeof(unsigned int));
                sqlite3_result_subtype(context, subtype);
            } else {
                sqlite3_result_subtype(context, data[0]);
            }
            break;
            
        case 4:
            // Test sqlite3_result_blob
            if ((size_t)size > offset) {
                size_t blob_size = size - offset;
                if (blob_size > 1000) blob_size = 1000; // Limit size
                sqlite3_result_blob(context, data + offset, (int)blob_size, SQLITE_TRANSIENT);
            } else {
                sqlite3_result_blob(context, "dummy", 5, SQLITE_STATIC);
            }
            break;
            
        case 5:
            // Test sqlite3_result_error_nomem
            sqlite3_result_error_nomem(context);
            break;
            
        default:
            sqlite3_result_null(context);
            break;
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    // Initialize database if not already done
    if (db == NULL) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Register our test function
        sqlite3_create_function(db, "test_fuzz_func", 2, SQLITE_UTF8 | SQLITE_RESULT_SUBTYPE, 
                               NULL, test_function, NULL, NULL);
    }
    
    // Create a dummy sqlite3_value for the second argument
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *dummy_value = NULL;
    
    // Prepare a statement to create a dummy value
    if (sqlite3_prepare_v2(db, "SELECT ?", -1, &stmt, NULL) == SQLITE_OK) {
        // Bind some data to create a value
        sqlite3_bind_text(stmt, 1, "test", 4, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get the value from the first column
            dummy_value = sqlite3_column_value(stmt, 0);
            
            // Now call our test function using SQL
            sqlite3_stmt *call_stmt = NULL;
            const char *sql = "SELECT test_fuzz_func(?, ?)";
            
            if (sqlite3_prepare_v2(db, sql, -1, &call_stmt, NULL) == SQLITE_OK) {
                // Bind the fuzzer data as first parameter (blob)
                sqlite3_bind_blob(call_stmt, 1, Data, 
                                 (Size > 1000) ? 1000 : (int)Size, SQLITE_STATIC);
                
                // Bind the dummy value as second parameter
                if (dummy_value) {
                    // We need to create a copy of the value
                    sqlite3_value *value_copy = sqlite3_value_dup(dummy_value);
                    if (value_copy) {
                        sqlite3_bind_value(call_stmt, 2, value_copy);
                        sqlite3_value_free(value_copy);
                    } else {
                        sqlite3_bind_null(call_stmt, 2);
                    }
                } else {
                    sqlite3_bind_null(call_stmt, 2);
                }
                
                // Execute the function
                sqlite3_step(call_stmt);
                
                // Clean up
                sqlite3_finalize(call_stmt);
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Reset database state
    sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
    
    return 0;
}

// Cleanup function
__attribute__((destructor))
static void cleanup() {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}
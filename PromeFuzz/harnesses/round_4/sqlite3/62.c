// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_double at sqlite3.c:78465:19 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_double at sqlite3.c:78465:19 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This function is just to provide a valid context
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    // Initialize database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy table and insert some data
    char *err_msg = NULL;
    sqlite3_exec(db, "CREATE TABLE test (col1 BLOB, col2 TEXT, col3 INTEGER, col4 REAL);", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Prepare a statement to get a protected sqlite3_value
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, "INSERT INTO test VALUES (?, ?, ?, ?)", -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the fuzzer data as different types
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, Size > 1000 ? 1000 : Size, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, (const char*)Data, Size > 1000 ? 1000 : Size, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, Size);
            sqlite3_bind_double(stmt, 4, (double)Size);
        } else {
            // Use some default values if input is empty
            sqlite3_bind_blob(stmt, 1, "dummy", 5, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, "dummy", 5, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, 42);
            sqlite3_bind_double(stmt, 4, 3.14);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Now query the data to get sqlite3_value objects
    if (sqlite3_prepare_v2(db, "SELECT col1, col2, col3, col4 FROM test", -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column values as unprotected sqlite3_value objects
            sqlite3_value *value1 = sqlite3_column_value(stmt, 0);
            sqlite3_value *value2 = sqlite3_column_value(stmt, 1);
            sqlite3_value *value3 = sqlite3_column_value(stmt, 2);
            sqlite3_value *value4 = sqlite3_column_value(stmt, 3);
            
            // Create protected copies using sqlite3_value_dup
            sqlite3_value *protected1 = sqlite3_value_dup(value1);
            sqlite3_value *protected2 = sqlite3_value_dup(value2);
            sqlite3_value *protected3 = sqlite3_value_dup(value3);
            sqlite3_value *protected4 = sqlite3_value_dup(value4);
            
            // Test sqlite3_value_text16be on protected values
            if (protected2) {
                const void *text16be = sqlite3_value_text16be(protected2);
                // Use the result to avoid optimization
                if (text16be) {
                    // Do nothing, just call the function
                }
            }
            
            // Test sqlite3_value_double
            if (protected4) {
                double d = sqlite3_value_double(protected4);
                // Use the result
                if (d != 0.0) {
                    // Do nothing
                }
            }
            
            // Test sqlite3_value_blob
            if (protected1) {
                const void *blob = sqlite3_value_blob(protected1);
                int bytes = sqlite3_value_bytes(protected1);
                // Use the results
                if (blob && bytes > 0) {
                    // Do nothing
                }
            }
            
            // Test sqlite3_value_int
            if (protected3) {
                int i = sqlite3_value_int(protected3);
                // Use the result
                if (i != 0) {
                    // Do nothing
                }
            }
            
            // Test sqlite3_result_value by creating a function context
            sqlite3_context *ctx = NULL;
            // We need to create a minimal function context
            // Create a user-defined function to get a valid context
            sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, dummy_function, NULL, NULL);
            
            // Prepare a statement that uses the function
            sqlite3_stmt *stmt2 = NULL;
            if (sqlite3_prepare_v2(db, "SELECT dummy_func(col1) FROM test", -1, &stmt2, NULL) == SQLITE_OK) {
                // Execute to trigger the function
                sqlite3_step(stmt2);
                sqlite3_finalize(stmt2);
            }
            
            // Free duplicated values
            if (protected1) sqlite3_value_free(protected1);
            if (protected2) sqlite3_value_free(protected2);
            if (protected3) sqlite3_value_free(protected3);
            if (protected4) sqlite3_value_free(protected4);
        }
        sqlite3_finalize(stmt);
    }
    
    // Additional test: create more diverse values
    // Create a statement with bound values
    sqlite3_stmt *bind_stmt = NULL;
    if (sqlite3_prepare_v2(db, "SELECT ?1, ?2, ?3, ?4", -1, &bind_stmt, NULL) == SQLITE_OK) {
        // Bind different types of values
        if (Size > 0) {
            // Use fuzzer data for binding
            int use_size = Size > 500 ? 500 : Size;
            
            // Bind as blob
            sqlite3_bind_blob(bind_stmt, 1, Data, use_size, SQLITE_STATIC);
            
            // Bind as text
            sqlite3_bind_text(bind_stmt, 2, (const char*)Data, use_size, SQLITE_STATIC);
            
            // Bind as integer (use size modulo 1000)
            sqlite3_bind_int(bind_stmt, 3, (int)(Size % 1000));
            
            // Bind as double (use size as double)
            sqlite3_bind_double(bind_stmt, 4, (double)Size);
        }
        
        if (sqlite3_step(bind_stmt) == SQLITE_ROW) {
            // Get values and test the APIs again
            sqlite3_value *val1 = sqlite3_column_value(bind_stmt, 0);
            sqlite3_value *val2 = sqlite3_column_value(bind_stmt, 1);
            
            // Create protected copies
            sqlite3_value *pval1 = sqlite3_value_dup(val1);
            sqlite3_value *pval2 = sqlite3_value_dup(val2);
            
            if (pval1) {
                // Test text16be on blob value (should convert)
                const void *text = sqlite3_value_text16be(pval1);
                if (text) {
                    // Do nothing
                }
                
                // Test blob extraction
                const void *blob = sqlite3_value_blob(pval1);
                if (blob) {
                    // Do nothing
                }
                
                sqlite3_value_free(pval1);
            }
            
            if (pval2) {
                // Test double conversion on text
                double d = sqlite3_value_double(pval2);
                // Test integer conversion on text
                int i = sqlite3_value_int(pval2);
                
                sqlite3_value_free(pval2);
            }
        }
        
        sqlite3_finalize(bind_stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This is a dummy function that will be registered to create contexts
    // It doesn't need to do anything for our fuzzing purposes
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    // Initialize database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy table and insert some data to work with
    char *err_msg = NULL;
    const char *create_table_sql = "CREATE TABLE test (id INTEGER, data BLOB, text TEXT);";
    if (sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some data using the fuzzer input
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql = "INSERT INTO test VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind some values - use fuzzer data for blob and text
        sqlite3_bind_int(stmt, 1, 1);
        
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 2, Data, Size > 1024 ? 1024 : (int)Size, SQLITE_TRANSIENT);
            
            // Convert first part of data to text if possible
            size_t text_len = Size > 512 ? 512 : Size;
            char *text_buf = malloc(text_len + 1);
            if (text_buf) {
                memcpy(text_buf, Data, text_len);
                text_buf[text_len] = '\0';
                sqlite3_bind_text(stmt, 3, text_buf, -1, SQLITE_TRANSIENT);
                free(text_buf);
            } else {
                sqlite3_bind_text(stmt, 3, "test", -1, SQLITE_TRANSIENT);
            }
        } else {
            sqlite3_bind_blob(stmt, 2, "test", 4, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, "test", -1, SQLITE_TRANSIENT);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Query the data to get sqlite3_value objects
    const char *select_sql = "SELECT id, data, text FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get unprotected values from columns (these need to be duplicated to be protected)
            sqlite3_value *unprotected_values[3];
            unprotected_values[0] = sqlite3_column_value(stmt, 0);
            unprotected_values[1] = sqlite3_column_value(stmt, 1);
            unprotected_values[2] = sqlite3_column_value(stmt, 2);
            
            // Create protected values by duplicating the unprotected ones
            sqlite3_value *protected_values[3];
            for (int i = 0; i < 3; i++) {
                protected_values[i] = sqlite3_value_dup(unprotected_values[i]);
            }
            
            // Now test the target API functions with protected values
            for (int i = 0; i < 3; i++) {
                if (protected_values[i]) {
                    // Test sqlite3_value_numeric_type
                    int numeric_type = sqlite3_value_numeric_type(protected_values[i]);
                    
                    // Test sqlite3_value_type
                    int value_type = sqlite3_value_type(protected_values[i]);
                    
                    // Test sqlite3_value_int
                    int int_value = sqlite3_value_int(protected_values[i]);
                    
                    // Test sqlite3_value_blob
                    const void *blob_data = sqlite3_value_blob(protected_values[i]);
                    if (blob_data) {
                        // Can't do much with the blob pointer, but we got it
                    }
                    
                    // Test sqlite3_value_bytes16
                    int bytes16 = sqlite3_value_bytes16(protected_values[i]);
                    
                    // Free the protected value
                    sqlite3_value_free(protected_values[i]);
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test sqlite3_result_value by creating a custom function context
    // First, register a dummy function
    sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, dummy_function, NULL, NULL);
    
    // Prepare a statement that uses the function
    const char *func_sql = "SELECT dummy_func(?)";
    if (sqlite3_prepare_v2(db, func_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind a value
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, Size > 256 ? 256 : (int)Size, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_int(stmt, 1, 42);
        }
        
        // Step to execute (this will call our dummy function)
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
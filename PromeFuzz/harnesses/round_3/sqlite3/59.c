// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:78478:18 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:78478:18 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - used for MEM_Dyn values
}

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Dummy function for sqlite3_result_value testing
    if (argc > 0) {
        sqlite3_result_value(ctx, argv[0]);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a table and insert some data to get values from
    char *err_msg = NULL;
    const char *create_table = "CREATE TABLE test (id INTEGER, name TEXT, data BLOB);";
    if (sqlite3_exec(db, create_table, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Insert data using the fuzzer input
    sqlite3_stmt *stmt;
    const char *insert = "INSERT INTO test VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db, insert, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, (int)(Size % 100));
        
        // Bind text data
        char *text_data = (char*)malloc(Size + 1);
        if (text_data) {
            memcpy(text_data, Data, Size);
            text_data[Size] = '\0';
            sqlite3_bind_text(stmt, 2, text_data, -1, SQLITE_TRANSIENT);
            free(text_data);
        }
        
        // Bind blob data
        sqlite3_bind_blob(stmt, 3, Data, (int)Size, SQLITE_TRANSIENT);
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Query data to get sqlite3_value objects
    const char *select = "SELECT id, name, data FROM test;";
    if (sqlite3_prepare_v2(db, select, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get values using sqlite3_column_value (unprotected)
            sqlite3_value *val1 = sqlite3_column_value(stmt, 0);
            sqlite3_value *val2 = sqlite3_column_value(stmt, 1);
            sqlite3_value *val3 = sqlite3_column_value(stmt, 2);
            
            // Duplicate the values to get protected values
            sqlite3_value *dup_val1 = sqlite3_value_dup(val1);
            sqlite3_value *dup_val2 = sqlite3_value_dup(val2);
            sqlite3_value *dup_val3 = sqlite3_value_dup(val3);
            
            // Test sqlite3_value_text
            if (dup_val2) {
                const unsigned char *text_result = sqlite3_value_text(dup_val2);
                (void)text_result;
            }
            
            // Test sqlite3_value_text16le
            if (dup_val2) {
                const void *text16le_result = sqlite3_value_text16le(dup_val2);
                (void)text16le_result;
            }
            
            // Test sqlite3_value_blob
            if (dup_val3) {
                const void *blob_result = sqlite3_value_blob(dup_val3);
                (void)blob_result;
            }
            
            // Test sqlite3_value_pointer
            // First create a pointer value
            sqlite3_stmt *stmt2;
            const char *select_ptr = "SELECT pointer(?);";
            if (sqlite3_prepare_v2(db, select_ptr, -1, &stmt2, NULL) == SQLITE_OK) {
                // Bind a pointer
                int dummy = 42;
                sqlite3_bind_pointer(stmt2, 1, &dummy, "test_pointer", NULL);
                if (sqlite3_step(stmt2) == SQLITE_ROW) {
                    sqlite3_value *ptr_val = sqlite3_column_value(stmt2, 0);
                    sqlite3_value *dup_ptr_val = sqlite3_value_dup(ptr_val);
                    if (dup_ptr_val) {
                        void *pointer_result = sqlite3_value_pointer(dup_ptr_val, "test_pointer");
                        (void)pointer_result;
                        
                        // Test with different type strings
                        if (Size > 10) {
                            char type_buf[11];
                            size_t type_len = Size > 10 ? 10 : Size;
                            for (size_t i = 0; i < type_len; i++) {
                                type_buf[i] = (char)(Data[i] % 128);
                            }
                            type_buf[type_len] = '\0';
                            sqlite3_value_pointer(dup_ptr_val, type_buf);
                        }
                        
                        sqlite3_value_free(dup_ptr_val);
                    }
                }
                sqlite3_finalize(stmt2);
            }
            
            // Test sqlite3_result_value by creating a user-defined function
            sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, test_function, NULL, NULL);
            
            sqlite3_stmt *stmt3;
            const char *call_func = "SELECT test_func(name) FROM test;";
            if (sqlite3_prepare_v2(db, call_func, -1, &stmt3, NULL) == SQLITE_OK) {
                sqlite3_step(stmt3);
                sqlite3_finalize(stmt3);
            }
            
            // Free duplicated values
            if (dup_val1) sqlite3_value_free(dup_val1);
            if (dup_val2) sqlite3_value_free(dup_val2);
            if (dup_val3) sqlite3_value_free(dup_val3);
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
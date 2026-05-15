// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:78478:18 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Empty destructor for testing
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    // Create dummy database connection
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a protected sqlite3_value using sqlite3_bind_pointer
    sqlite3_stmt *stmt = NULL;
    const char *create_sql = "CREATE TABLE test (id INTEGER);";
    if (sqlite3_exec(db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    const char *insert_sql = "INSERT INTO test VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy pointer value from fuzzer data
    void *dummy_ptr = Size > 0 ? (void *)(uintptr_t)Data[0] : NULL;
    
    // Bind pointer with subtype
    const char *subtype = "test_type";
    sqlite3_bind_pointer(stmt, 1, dummy_ptr, subtype, dummy_destructor);
    
    // Execute to create the value
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    
    // Get the value back using sqlite3_column_value (unprotected)
    const char *select_sql = "SELECT id FROM test;";
    sqlite3_stmt *select_stmt = NULL;
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    if (sqlite3_step(select_stmt) == SQLITE_ROW) {
        // Get unprotected value
        sqlite3_value *unprotected_val = sqlite3_column_value(select_stmt, 0);
        
        // Create a protected value by duplicating
        sqlite3_value *protected_val = sqlite3_value_dup(unprotected_val);
        
        if (protected_val) {
            // Test sqlite3_value_pointer with various subtype strings
            const char *subtypes[] = {"test_type", "other_type", "", NULL};
            for (int i = 0; subtypes[i] != NULL; i++) {
                void *result = sqlite3_value_pointer(protected_val, subtypes[i]);
                (void)result; // Use result to avoid compiler warnings
            }
            
            // Test sqlite3_value_text16
            const void *text16_result = sqlite3_value_text16(protected_val);
            (void)text16_result;
            
            // Test sqlite3_value_blob
            const void *blob_result = sqlite3_value_blob(protected_val);
            (void)blob_result;
            
            // Test sqlite3_value_text16be
            const void *text16be_result = sqlite3_value_text16be(protected_val);
            (void)text16be_result;
            
            // Test sqlite3_value_text16le
            const void *text16le_result = sqlite3_value_text16le(protected_val);
            (void)text16le_result;
            
            // Test sqlite3_result_value - need a context
            // Create a simple scalar function context
            sqlite3_create_function(
                db,
                "test_func",
                1,
                SQLITE_UTF8,
                NULL,
                NULL,
                NULL,
                NULL
            );
            
            // Prepare a statement that uses the function
            const char *func_sql = "SELECT test_func(id) FROM test;";
            sqlite3_stmt *func_stmt = NULL;
            if (sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, NULL) == SQLITE_OK) {
                // This would normally be called from within the function implementation
                // We can't easily call sqlite3_result_value without being in a function callback
                sqlite3_finalize(func_stmt);
            }
            
            // Free the duplicated value
            sqlite3_value_free(protected_val);
        }
    }
    
    // Cleanup
    sqlite3_finalize(select_stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
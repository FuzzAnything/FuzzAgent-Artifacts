// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_double at sqlite3.c:78465:19 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_double at sqlite3.c:78465:19 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_double at sqlite3.c:78465:19 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_double at sqlite3.c:78465:19 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This function does nothing but exists to create a valid context
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *value = NULL;
    sqlite3_value *protected_value = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        goto cleanup;
    }
    
    // Create a dummy table and insert some data
    rc = sqlite3_exec(db, "CREATE TABLE test (data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        goto cleanup;
    }
    
    // Prepare a statement to bind our fuzz data
    rc = sqlite3_prepare_v2(db, "INSERT INTO test VALUES (?);", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        goto cleanup;
    }
    
    // Bind the fuzz data as a blob
    rc = sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1024 ? 1024 : Size), SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        goto cleanup;
    }
    
    // Execute the insert
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        goto cleanup;
    }
    
    // Reset and prepare for SELECT
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    rc = sqlite3_prepare_v2(db, "SELECT data FROM test;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        goto cleanup;
    }
    
    // Get a value from the result (unprotected)
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        value = sqlite3_column_value(stmt, 0);
        
        // Create a protected value from the unprotected one using sqlite3_value_dup
        protected_value = sqlite3_value_dup(value);
        if (protected_value) {
            // Test sqlite3_value_blob
            const void *blob_data = sqlite3_value_blob(protected_value);
            (void)blob_data;
            
            // Test sqlite3_value_double
            double dval = sqlite3_value_double(protected_value);
            (void)dval;
            
            // Test sqlite3_value_text16le
            const void *text16le = sqlite3_value_text16le(protected_value);
            (void)text16le;
            
            // Test sqlite3_value_text16be
            const void *text16be = sqlite3_value_text16be(protected_value);
            (void)text16be;
            
            // Test sqlite3_value_text16
            const void *text16 = sqlite3_value_text16(protected_value);
            (void)text16;
            
            // Create a dummy context for sqlite3_result_value
            // We'll use a custom function to get a valid context
            sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, 
                                   dummy_function, NULL, NULL);
            
            sqlite3_stmt *stmt2 = NULL;
            rc = sqlite3_prepare_v2(db, "SELECT dummy_func(data) FROM test;", -1, &stmt2, NULL);
            if (rc == SQLITE_OK) {
                rc = sqlite3_step(stmt2);
                if (rc == SQLITE_ROW) {
                    // We can't easily get the context here, but we've exercised the other functions
                }
                sqlite3_finalize(stmt2);
            }
            
            // Test with different encodings by converting the value
            sqlite3_value *text_value = sqlite3_value_dup(value);
            if (text_value) {
                // Try to get text representation
                const char *text = (const char *)sqlite3_value_text(text_value);
                (void)text;
                
                // Test the text16 functions again on text value
                const void *text16le2 = sqlite3_value_text16le(text_value);
                (void)text16le2;
                
                const void *text16be2 = sqlite3_value_text16be(text_value);
                (void)text16be2;
                
                const void *text162 = sqlite3_value_text16(text_value);
                (void)text162;
                
                sqlite3_value_free(text_value);
            }
            
            sqlite3_value_free(protected_value);
        }
    }
    
    // Test with NULL value
    sqlite3_value *null_value = sqlite3_value_dup(NULL);
    if (null_value) {
        const void *null_blob = sqlite3_value_blob(null_value);
        (void)null_blob;
        
        double null_double = sqlite3_value_double(null_value);
        (void)null_double;
        
        sqlite3_value_free(null_value);
    }
    
    // Test with integer value
    sqlite3_stmt *int_stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT 123;", -1, &int_stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(int_stmt);
        if (rc == SQLITE_ROW) {
            sqlite3_value *int_value = sqlite3_value_dup(sqlite3_column_value(int_stmt, 0));
            if (int_value) {
                const void *int_blob = sqlite3_value_blob(int_value);
                (void)int_blob;
                
                double int_double = sqlite3_value_double(int_value);
                (void)int_double;
                
                sqlite3_value_free(int_value);
            }
        }
        sqlite3_finalize(int_stmt);
    }
    
    // Test with text value
    sqlite3_stmt *text_stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT 'test string';", -1, &text_stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(text_stmt);
        if (rc == SQLITE_ROW) {
            sqlite3_value *text_value = sqlite3_value_dup(sqlite3_column_value(text_stmt, 0));
            if (text_value) {
                const void *text_blob = sqlite3_value_blob(text_value);
                (void)text_blob;
                
                const void *text16le_val = sqlite3_value_text16le(text_value);
                (void)text16le_val;
                
                sqlite3_value_free(text_value);
            }
        }
        sqlite3_finalize(text_stmt);
    }
    
    // Test with real value
    sqlite3_stmt *real_stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT 3.14159;", -1, &real_stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(real_stmt);
        if (rc == SQLITE_ROW) {
            sqlite3_value *real_value = sqlite3_value_dup(sqlite3_column_value(real_stmt, 0));
            if (real_value) {
                double real_double = sqlite3_value_double(real_value);
                (void)real_double;
                
                const void *real_blob = sqlite3_value_blob(real_value);
                (void)real_blob;
                
                sqlite3_value_free(real_value);
            }
        }
        sqlite3_finalize(real_stmt);
    }

cleanup:
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}
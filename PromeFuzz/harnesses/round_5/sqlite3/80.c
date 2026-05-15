// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:173641:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:173641:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_file_write(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc;
    
    // Initialize database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Write data to dummy file for potential file operations
    dummy_file_write(Data, Size);
    
    // Test sqlite3_prepare16_v2 with fuzzer input as UTF-16 SQL
    // Note: We need to ensure the input has at least 2 bytes for UTF-16
    if (Size >= 2) {
        // Use the fuzzer input as UTF-16 SQL statement
        // Ensure null termination for UTF-16 string
        size_t utf16_size = Size;
        if (utf16_size % 2 != 0) {
            utf16_size--;  // Make it even for UTF-16
        }
        
        // Prepare statement
        rc = sqlite3_prepare16_v2(db, Data, utf16_size, &stmt, &pzTail);
        
        // Test sqlite3_db_handle if statement was created
        if (stmt) {
            sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
            // db_from_stmt should be the same as db
            
            // Test sqlite3_column_name if we have a valid statement
            // Try to get column name for column 0
            const char *col_name = sqlite3_column_name(stmt, 0);
            // col_name may be NULL if no columns or error, which is OK
            
            // Test sqlite3_total_changes on the database
            int total_changes = sqlite3_total_changes(db);
            // This is just to exercise the API, ignore the value
            
            // Test sqlite3_error_offset
            int error_offset = sqlite3_error_offset(db);
            // This is just to exercise the API, ignore the value
            
            // Test sqlite3_finalize
            int finalize_rc = sqlite3_finalize(stmt);
            // Reset stmt to NULL after finalizing
            stmt = NULL;
            
            // Try to create another statement with different SQL
            // Use a simple CREATE TABLE statement in UTF-16
            static const uint16_t simple_sql[] = {
                'C','R','E','A','T','E',' ','T','A','B','L','E',' ',
                't','1','(','a',',','b',')',';',0
            };
            rc = sqlite3_prepare16_v2(db, simple_sql, sizeof(simple_sql)-2, &stmt, &pzTail);
            
            if (stmt) {
                // Test the APIs again with this new statement
                sqlite3_db_handle(stmt);
                sqlite3_column_name(stmt, 0);
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
        
        // Test error offset again after potential errors
        sqlite3_error_offset(db);
        
        // Test total changes again
        sqlite3_total_changes(db);
    }
    
    // Test edge cases
    
    // Test sqlite3_finalize with NULL (should be harmless)
    sqlite3_finalize(NULL);
    
    // Test sqlite3_db_handle with NULL (should return NULL)
    sqlite3_db_handle(NULL);
    
    // Test sqlite3_column_name with NULL statement
    const char *col_name_null = sqlite3_column_name(NULL, 0);
    
    // Test sqlite3_prepare16_v2 with various inputs
    // Try with NULL SQL pointer
    if (Size > 0) {
        rc = sqlite3_prepare16_v2(db, NULL, 0, &stmt, &pzTail);
        // This should fail, but we don't care about the result
    }
    
    // Try with zero length
    if (Size > 0) {
        const void *empty_sql = Data;
        rc = sqlite3_prepare16_v2(db, empty_sql, 0, &stmt, &pzTail);
    }
    
    // Try with negative length (means read until null terminator)
    if (Size >= 2) {
        // Create a null-terminated UTF-16 string from the data
        size_t copy_size = Size;
        if (copy_size % 2 != 0) copy_size--;
        uint8_t *null_term_data = malloc(copy_size + 2);
        if (null_term_data) {
            memcpy(null_term_data, Data, copy_size);
            null_term_data[copy_size] = 0;
            null_term_data[copy_size + 1] = 0;
            rc = sqlite3_prepare16_v2(db, null_term_data, -1, &stmt, &pzTail);
            free(null_term_data);
            
            if (stmt) {
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
    }
    
    // Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Test total changes one more time before closing
    sqlite3_total_changes(db);
    
    sqlite3_close(db);
    
    return 0;
}
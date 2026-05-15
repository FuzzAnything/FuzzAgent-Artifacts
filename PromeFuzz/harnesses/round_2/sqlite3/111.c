// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:173641:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
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
#include <stdio.h>

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Test sqlite3_total_changes on fresh connection
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes; // Use result to avoid unused variable warning
    
    // Try to prepare a statement using the fuzzer input as UTF-16 SQL
    // Note: We need at least 2 bytes for UTF-16 (even if it's just null terminator)
    if (Size >= 2) {
        // Ensure the input has a null terminator for UTF-16
        size_t utf16_size = Size;
        if (utf16_size % 2 != 0) {
            utf16_size--; // Make it even for UTF-16
        }
        
        // Try to prepare the statement
        rc = sqlite3_prepare16_v2(db, (const void*)Data, (int)utf16_size, &stmt, &pzTail);
        
        // Test sqlite3_error_offset regardless of prepare result
        int error_offset = sqlite3_error_offset(db);
        (void)error_offset;
        
        if (rc == SQLITE_OK && stmt != NULL) {
            // Test sqlite3_db_handle
            sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
            (void)db_from_stmt;
            
            // Test sqlite3_column_name for first column (if any)
            const char *col_name = sqlite3_column_name(stmt, 0);
            (void)col_name;
            
            // Try to get column names for a few columns
            for (int i = 0; i < 5; i++) {
                const char *name = sqlite3_column_name(stmt, i);
                (void)name;
            }
            
            // Test sqlite3_finalize
            int finalize_rc = sqlite3_finalize(stmt);
            (void)finalize_rc;
            stmt = NULL; // Statement is now finalized
        } else if (stmt != NULL) {
            // Prepare failed but stmt might have been allocated
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        
        // Test sqlite3_total_changes again after operations
        total_changes = sqlite3_total_changes(db);
        
        // Try to prepare another statement with different parameters
        if (Size >= 4) {
            // Use a subset of the data
            size_t subset_size = Size / 2;
            if (subset_size % 2 != 0) subset_size--;
            
            sqlite3_stmt *stmt2 = NULL;
            rc = sqlite3_prepare16_v2(db, (const void*)Data, (int)subset_size, &stmt2, NULL);
            if (rc == SQLITE_OK && stmt2 != NULL) {
                // Test sqlite3_db_handle on second statement
                sqlite3 *db2 = sqlite3_db_handle(stmt2);
                (void)db2;
                
                // Test column names
                for (int i = 0; i < 3; i++) {
                    const char *name = sqlite3_column_name(stmt2, i);
                    (void)name;
                }
                
                sqlite3_finalize(stmt2);
            }
        }
    }
    
    // Test with a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Clean up
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    
    return 0;
}
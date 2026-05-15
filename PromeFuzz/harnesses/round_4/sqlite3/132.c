// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    // Create a dummy file for database
    const char *db_file = "./dummy_file";
    
    // Initialize database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(db_file, &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a prepared statement
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // Use the input data as SQL statement (UTF-8)
    // Ensure we have at least 1 byte for flags
    if (Size > 0) {
        // Extract prepFlags from first byte
        unsigned int prepFlags = (unsigned int)Data[0];
        const char *zSql = (const char*)(Data + 1);
        int nByte = (Size - 1 > 0) ? (int)(Size - 1) : 0;
        
        // Prepare statement with sqlite3_prepare_v3
        rc = sqlite3_prepare_v3(db, zSql, nByte, prepFlags, &stmt, &tail);
        
        // If preparation succeeded, test other functions
        if (stmt != NULL) {
            // Test sqlite3_normalized_sql
            const char *norm_sql = sqlite3_normalized_sql(stmt);
            (void)norm_sql; // Use result to avoid unused variable warning
            
            // Test sqlite3_stmt_status with various operations
            // Try different operation codes (0-10 are common SQLITE_STMTSTATUS values)
            for (int op = 0; op <= 10; op++) {
                int status_val = sqlite3_stmt_status(stmt, op, 0); // Don't reset
                (void)status_val;
                status_val = sqlite3_stmt_status(stmt, op, 1); // Reset after reading
                (void)status_val;
            }
            
            // Test sqlite3_stmt_scanstatus if compiled with SQLITE_ENABLE_STMT_SCANSTATUS
            #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
            // Try different scan status operations (0-5 are common SQLITE_SCANSTAT values)
            for (int idx = -1; idx <= 5; idx++) {
                for (int scan_op = 0; scan_op <= 5; scan_op++) {
                    int out_val = 0;
                    double out_double = 0.0;
                    const char *out_str = NULL;
                    
                    // Try with different output types
                    rc = sqlite3_stmt_scanstatus(stmt, idx, scan_op, &out_val);
                    rc = sqlite3_stmt_scanstatus(stmt, idx, scan_op, &out_double);
                    rc = sqlite3_stmt_scanstatus(stmt, idx, scan_op, &out_str);
                }
            }
            
            // Test sqlite3_stmt_scanstatus_reset
            sqlite3_stmt_scanstatus_reset(stmt);
            #endif
            
            // Finalize the statement
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test sqlite3_prepare16_v2 with UTF-16 encoding
    // Convert some of the input to UTF-16 (simplified - just treat as raw bytes)
    if (Size > 0) {
        // Allocate buffer for UTF-16 data (add null terminator)
        size_t utf16_size = Size * 2 + 2;
        void *zSql16 = malloc(utf16_size);
        if (zSql16 != NULL) {
            // Simple conversion: copy bytes and add null terminators
            memset(zSql16, 0, utf16_size);
            memcpy(zSql16, Data, Size > utf16_size/2 ? utf16_size/2 : Size);
            
            const void *tail16 = NULL;
            rc = sqlite3_prepare16_v2(db, zSql16, (int)Size * 2, &stmt, &tail16);
            
            if (stmt != NULL) {
                // Test functions on UTF-16 prepared statement
                const char *norm_sql16 = sqlite3_normalized_sql(stmt);
                (void)norm_sql16;
                
                // Test status functions
                for (int op = 0; op <= 5; op++) {
                    int status_val = sqlite3_stmt_status(stmt, op, 0);
                    (void)status_val;
                }
                
                #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
                // Try a few scanstatus calls
                int dummy_out = 0;
                rc = sqlite3_stmt_scanstatus(stmt, 0, 0, &dummy_out);
                sqlite3_stmt_scanstatus_reset(stmt);
                #endif
                
                sqlite3_finalize(stmt);
            }
            
            free(zSql16);
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    // Remove dummy file
    remove(db_file);
    
    return 0;
}
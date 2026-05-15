// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:80267:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:80267:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *pStmt = NULL;
    const char *zTail = NULL;
    int rc;
    
    // Open an in-memory database for fuzzing
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Use the fuzz input as SQL statement
    const char *zSql = (const char *)Data;
    int nByte;
    
    // Handle size conversion safely
    if (Size == 0) {
        nByte = 0;
    } else if (Size <= 1048576) { // Limit to 1MB to avoid excessive memory usage
        nByte = (int)Size;
    } else {
        nByte = 1048576; // Cap at 1MB
    }
    
    // Prepare statement from fuzz input
    rc = sqlite3_prepare_v2(db, zSql, nByte, &pStmt, &zTail);
    
    if (rc == SQLITE_OK && pStmt != NULL) {
        // Test sqlite3_stmt_isexplain
        int explain_type = sqlite3_stmt_isexplain(pStmt);
        (void)explain_type; // Use result to avoid unused variable warning
        
        // Test sqlite3_column_count
        int col_count = sqlite3_column_count(pStmt);
        (void)col_count; // Use result to avoid unused variable warning
        
        // Test sqlite3_expired
        int expired = sqlite3_expired(pStmt);
        (void)expired; // Use result to avoid unused variable warning
        
        // Test sqlite3_stmt_scanstatus_v2 with different parameters
        // We try various indices and operations to explore different code paths
        for (int idx = 0; idx < 5; idx++) {
            for (int op = 0; op < 5; op++) {
                int flags = 0;
                void *pOut = NULL;
                
                // Try different flag combinations
                if (idx % 2 == 0) {
                    flags = 1; // Use a non-zero flag value
                }
                
                // Allocate some memory for output if needed
                int out_value = 0;
                pOut = &out_value;
                
                // Call scanstatus - ignore return value as it may fail
                sqlite3_stmt_scanstatus_v2(pStmt, idx, op, flags, pOut);
            }
        }
        
        // Try to step the statement to change its state
        rc = sqlite3_step(pStmt);
        
        // Test sqlite3_reset
        int reset_rc = sqlite3_reset(pStmt);
        (void)reset_rc; // Use result to avoid unused variable warning
        
        // Finalize the statement
        sqlite3_finalize(pStmt);
    } else {
        // If prepare failed, pStmt might be NULL
        // Test sqlite3_stmt_isexplain with NULL pointer
        int null_explain = sqlite3_stmt_isexplain(NULL);
        (void)null_explain; // Use result to avoid unused variable warning
        
        // Test sqlite3_column_count with NULL pointer
        int null_col_count = sqlite3_column_count(NULL);
        (void)null_col_count; // Use result to avoid unused variable warning
        
        // Test sqlite3_expired with NULL pointer
        int null_expired = sqlite3_expired(NULL);
        (void)null_expired; // Use result to avoid unused variable warning
        
        // Test sqlite3_reset with NULL pointer
        int null_reset = sqlite3_reset(NULL);
        (void)null_reset; // Use result to avoid unused variable warning
    }
    
    // Close the database
    sqlite3_close(db);
    
    return 0;
}
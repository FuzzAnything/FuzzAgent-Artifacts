// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:173158:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int trace_callback(unsigned mask, void *ctx, void *p, void *x) {
    // Simple trace callback that does nothing
    return 0;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Enable extended result codes (use first byte as flag)
    int enable_extended = (Size > 0) ? (Data[0] & 1) : 0;
    sqlite3_extended_result_codes(db, enable_extended);
    
    // Set up trace (use second byte as mask)
    unsigned trace_mask = (Size > 1) ? Data[1] : 0;
    if (trace_mask != 0) {
        sqlite3_trace_v2(db, trace_mask, trace_callback, NULL);
    }
    
    // Prepare SQL statement from fuzzer input
    // Skip first two bytes if they were used for flags
    const char *sql = (const char *)Data;
    int sql_len = (int)Size;
    if (Size > 2) {
        sql = (const char *)(Data + 2);
        sql_len = (int)(Size - 2);
    }
    
    // Ensure the SQL string is null-terminated
    char *sql_copy = malloc(sql_len + 1);
    if (!sql_copy) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql_copy, sql, sql_len);
    sql_copy[sql_len] = '\0';
    
    // Prepare the statement
    rc = sqlite3_prepare_v2(db, sql_copy, -1, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        // Execute the statement multiple times to explore different states
        for (int i = 0; i < 3; i++) {
            int step_rc;
            do {
                step_rc = sqlite3_step(stmt);
                
                if (step_rc == SQLITE_ROW) {
                    // Get column count
                    int col_count = sqlite3_column_count(stmt);
                    
                    // Check column types for each column
                    for (int col = 0; col < col_count; col++) {
                        sqlite3_column_type(stmt, col);
                    }
                }
            } while (step_rc == SQLITE_ROW);
            
            // Reset the statement for next iteration
            sqlite3_reset(stmt);
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    free(sql_copy);
    sqlite3_close(db);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:132612:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_busy_handler(void *data, int count) {
    // Return 0 to indicate we don't want to retry
    return 0;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    static const char *dummy_file = "./dummy_file";
    
    // Initialize database
    sqlite3 *db = NULL;
    if (sqlite3_open(dummy_file, &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set busy handler to prevent hangs
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    // Prepare UTF-16 SQL statement from fuzzer input
    // We need at least 2 bytes for UTF-16 encoding
    if (Size < 2) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create UTF-16 encoded SQL from input data
    // Ensure even number of bytes for UTF-16
    size_t utf16_size = Size - (Size % 2);
    void *utf16_sql = malloc(utf16_size);
    if (!utf16_sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(utf16_sql, Data, utf16_size);
    
    // Prepare statements using different variants
    sqlite3_stmt *stmt = NULL;
    const void *tail = NULL;
    
    // Test sqlite3_prepare16
    int rc1 = sqlite3_prepare16(db, utf16_sql, (int)utf16_size, &stmt, &tail);
    if (rc1 == SQLITE_OK && stmt) {
        // Test sqlite3_data_count after stepping
        int step_rc = sqlite3_step(stmt);
        if (step_rc == SQLITE_ROW) {
            int data_count = sqlite3_data_count(stmt);
            (void)data_count; // Use result to avoid unused variable warning
        }
        
        // Test sqlite3_stmt_status with various operations
        for (int op = 0; op <= 5; op++) {
            for (int reset = 0; reset <= 1; reset++) {
                int status = sqlite3_stmt_status(stmt, op, reset);
                (void)status;
            }
        }
        
        // Test sqlite3_clear_bindings
        sqlite3_clear_bindings(stmt);
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Test sqlite3_prepare16_v2
    int rc2 = sqlite3_prepare16_v2(db, utf16_sql, (int)utf16_size, &stmt, &tail);
    if (rc2 == SQLITE_OK && stmt) {
        // Step and check data count
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int data_count = sqlite3_data_count(stmt);
            (void)data_count;
        }
        
        // Test various statement status operations
        for (int op = 0; op <= 5; op++) {
            sqlite3_stmt_status(stmt, op, 0);
            sqlite3_stmt_status(stmt, op, 1);
        }
        
        // Clear bindings
        sqlite3_clear_bindings(stmt);
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Test sqlite3_prepare16_v3 with different flags
    unsigned int prep_flags[] = {0, 1, 2, 4}; // Common prepare flags
    for (size_t i = 0; i < sizeof(prep_flags)/sizeof(prep_flags[0]); i++) {
        int rc3 = sqlite3_prepare16_v3(db, utf16_sql, (int)utf16_size, 
                                      prep_flags[i], &stmt, &tail);
        if (rc3 == SQLITE_OK && stmt) {
            // Step statement
            int step_result = sqlite3_step(stmt);
            if (step_result == SQLITE_ROW) {
                int data_count = sqlite3_data_count(stmt);
                (void)data_count;
            }
            
            // Test statement status
            for (int op = 0; op <= 5; op++) {
                sqlite3_stmt_status(stmt, op, 0);
            }
            
            // Clear bindings
            sqlite3_clear_bindings(stmt);
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test with negative nByte (null-terminated string)
    // Add null terminator for UTF-16 (two zero bytes)
    void *null_term_sql = malloc(utf16_size + 2);
    if (null_term_sql) {
        memcpy(null_term_sql, Data, utf16_size);
        memset((char*)null_term_sql + utf16_size, 0, 2);
        
        int rc4 = sqlite3_prepare16(db, null_term_sql, -1, &stmt, &tail);
        if (rc4 == SQLITE_OK && stmt) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int data_count = sqlite3_data_count(stmt);
                (void)data_count;
            }
            
            // Test statement status with reset
            sqlite3_stmt_status(stmt, 0, 1);
            sqlite3_stmt_status(stmt, 1, 1);
            
            sqlite3_clear_bindings(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        
        free(null_term_sql);
    }
    
    // Cleanup
    free(utf16_sql);
    sqlite3_close(db);
    
    return 0;
}
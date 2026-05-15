// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:132612:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Ensure we have at least 2 bytes for UTF-16 (minimum for null terminator)
    if (Size < 2) {
        sqlite3_close(db);
        return 0;
    }
    
    // Test sqlite3_prepare16
    rc = sqlite3_prepare16(db, Data, Size, &stmt, &pzTail);
    if (rc == SQLITE_OK && stmt) {
        // Get parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // Try binding zeroblob to parameters if any exist
        for (int i = 1; i <= param_count; i++) {
            sqlite3_bind_zeroblob(stmt, i, Size % 1024);  // Use Size mod 1024 for blob size
        }
        
        // Clear bindings
        sqlite3_clear_bindings(stmt);
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Test sqlite3_prepare16_v2
    rc = sqlite3_prepare16_v2(db, Data, Size, &stmt, &pzTail);
    if (rc == SQLITE_OK && stmt) {
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // Try different binding patterns
        for (int i = 1; i <= param_count; i++) {
            if (i % 2 == 0) {
                sqlite3_bind_zeroblob(stmt, i, (Size + i) % 512);
            }
        }
        
        sqlite3_clear_bindings(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Test sqlite3_prepare16_v3 with different flags
    unsigned int prepFlags[] = {0, SQLITE_PREPARE_PERSISTENT, SQLITE_PREPARE_NORMALIZE};
    
    for (size_t i = 0; i < sizeof(prepFlags) / sizeof(prepFlags[0]); i++) {
        rc = sqlite3_prepare16_v3(db, Data, Size, prepFlags[i], &stmt, &pzTail);
        if (rc == SQLITE_OK && stmt) {
            int param_count = sqlite3_bind_parameter_count(stmt);
            
            // Bind zeroblob to all parameters
            for (int j = 1; j <= param_count; j++) {
                sqlite3_bind_zeroblob(stmt, j, (Size * j) % 256);
            }
            
            // Test clear bindings multiple times
            sqlite3_clear_bindings(stmt);
            sqlite3_clear_bindings(stmt);  // Call twice to test idempotency
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test with negative nByte (auto-detect length)
    if (Size > 0) {
        // Create a null-terminated UTF-16 string
        size_t utf16_size = Size;
        if (utf16_size % 2 != 0) utf16_size--;  // Ensure even size for UTF-16
        
        if (utf16_size >= 2) {
            uint8_t *null_term_data = malloc(utf16_size + 2);
            if (null_term_data) {
                memcpy(null_term_data, Data, utf16_size);
                null_term_data[utf16_size] = 0;
                null_term_data[utf16_size + 1] = 0;
                
                rc = sqlite3_prepare16_v2(db, null_term_data, -1, &stmt, &pzTail);
                if (rc == SQLITE_OK && stmt) {
                    int param_count = sqlite3_bind_parameter_count(stmt);
                    
                    // Bind zeroblob with varying sizes
                    for (int i = 1; i <= param_count; i++) {
                        sqlite3_bind_zeroblob(stmt, i, i * 10);
                    }
                    
                    sqlite3_clear_bindings(stmt);
                    sqlite3_finalize(stmt);
                    stmt = NULL;
                }
                
                free(null_term_data);
            }
        }
    }
    
    // Test with partial input using pzTail
    if (Size > 4) {
        rc = sqlite3_prepare16_v2(db, Data, Size / 2, &stmt, &pzTail);
        if (rc == SQLITE_OK && stmt) {
            int param_count = sqlite3_bind_parameter_count(stmt);
            
            // Bind some parameters
            for (int i = 1; i <= param_count && i <= 5; i++) {
                sqlite3_bind_zeroblob(stmt, i, 100);
            }
            
            sqlite3_clear_bindings(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Clean up
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    
    return 0;
}
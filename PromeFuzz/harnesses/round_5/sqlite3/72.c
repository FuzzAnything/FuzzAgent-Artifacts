// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:170771:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    // Initialize database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Create a null-terminated copy of the input for functions that need it
    char *sql_copy = NULL;
    if (Size > 0) {
        sql_copy = (char *)malloc(Size + 1);
        if (sql_copy) {
            memcpy(sql_copy, Data, Size);
            sql_copy[Size] = '\0';
        }
    } else {
        // Handle empty input
        sql_copy = (char *)malloc(1);
        if (sql_copy) {
            sql_copy[0] = '\0';
        }
    }
    
    if (!sql_copy) {
        sqlite3_close(db);
        return 0;
    }
    
    // Test sqlite3_complete
    if (sql_copy) {
        sqlite3_complete(sql_copy);
    }
    
    // Test sqlite3_complete16
    if (sql_copy) {
        // Convert to UTF-16 (simple approach - just treat as raw bytes)
        // Note: This may not be valid UTF-16, but that's okay for fuzzing
        void *sql16 = malloc((Size + 1) * 2);
        if (sql16) {
            // Simple byte replication for fuzzing purposes
            for (size_t i = 0; i < Size; i++) {
                ((char *)sql16)[i * 2] = sql_copy[i];
                ((char *)sql16)[i * 2 + 1] = 0;
            }
            // Null terminate UTF-16 string
            ((char *)sql16)[Size * 2] = 0;
            ((char *)sql16)[Size * 2 + 1] = 0;
            
            sqlite3_complete16(sql16);
            free(sql16);
        }
    }
    
    // Test sqlite3_prepare
    if (db && sql_copy) {
        sqlite3_stmt *stmt = NULL;
        const char *tail = NULL;
        
        // Test with different nByte values
        int nByte_options[] = {-1, (int)Size, (int)Size/2, 0};
        for (size_t i = 0; i < sizeof(nByte_options)/sizeof(nByte_options[0]); i++) {
            int nByte = nByte_options[i];
            if (nByte > (int)Size) continue;
            
            rc = sqlite3_prepare(db, sql_copy, nByte, &stmt, &tail);
            if (rc == SQLITE_OK && stmt) {
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
    }
    
    // Test sqlite3_prepare_v3
    if (db && sql_copy) {
        sqlite3_stmt *stmt = NULL;
        const char *tail = NULL;
        
        // Test with different prepFlags
        unsigned int flags_options[] = {0, 1, 2, 4};
        for (size_t i = 0; i < sizeof(flags_options)/sizeof(flags_options[0]); i++) {
            rc = sqlite3_prepare_v3(db, sql_copy, -1, flags_options[i], &stmt, &tail);
            if (rc == SQLITE_OK && stmt) {
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
    }
    
    // Test sqlite3_prepare16
    if (db && sql_copy) {
        sqlite3_stmt *stmt = NULL;
        const void *tail = NULL;
        
        // Create UTF-16 version
        void *sql16 = malloc((Size + 1) * 2);
        if (sql16) {
            for (size_t i = 0; i < Size; i++) {
                ((char *)sql16)[i * 2] = sql_copy[i];
                ((char *)sql16)[i * 2 + 1] = 0;
            }
            ((char *)sql16)[Size * 2] = 0;
            ((char *)sql16)[Size * 2 + 1] = 0;
            
            int nByte_options[] = {-1, (int)Size * 2, (int)Size, 0};
            for (size_t i = 0; i < sizeof(nByte_options)/sizeof(nByte_options[0]); i++) {
                int nByte = nByte_options[i];
                if (nByte > (int)Size * 2) continue;
                
                rc = sqlite3_prepare16(db, sql16, nByte, &stmt, &tail);
                if (rc == SQLITE_OK && stmt) {
                    sqlite3_finalize(stmt);
                    stmt = NULL;
                }
            }
            
            free(sql16);
        }
    }
    
    // Test sqlite3_prepare16_v2
    if (db && sql_copy) {
        sqlite3_stmt *stmt = NULL;
        const void *tail = NULL;
        
        // Create UTF-16 version
        void *sql16 = malloc((Size + 1) * 2);
        if (sql16) {
            for (size_t i = 0; i < Size; i++) {
                ((char *)sql16)[i * 2] = sql_copy[i];
                ((char *)sql16)[i * 2 + 1] = 0;
            }
            ((char *)sql16)[Size * 2] = 0;
            ((char *)sql16)[Size * 2 + 1] = 0;
            
            rc = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &tail);
            if (rc == SQLITE_OK && stmt) {
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
            
            free(sql16);
        }
    }
    
    // Cleanup
    if (sql_copy) {
        free(sql_copy);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}
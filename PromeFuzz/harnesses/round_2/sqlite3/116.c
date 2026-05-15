// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:170771:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_prepare_functions(const uint8_t *data, size_t size, sqlite3 *db) {
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    const char *pzTail8 = NULL;
    
    // Test sqlite3_prepare16
    if (size > 0) {
        sqlite3_prepare16(db, (const void*)data, (int)size, &stmt, &pzTail);
        if (stmt) {
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test sqlite3_prepare (legacy)
    if (size > 0) {
        // Create a null-terminated string for safety
        char *sql = malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            sqlite3_prepare(db, sql, (int)size, &stmt, &pzTail8);
            free(sql);
            if (stmt) {
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
    }
    
    // Test sqlite3_prepare16_v2
    if (size > 0) {
        sqlite3_prepare16_v2(db, (const void*)data, (int)size, &stmt, &pzTail);
        if (stmt) {
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test sqlite3_prepare_v3
    if (size > 0) {
        // Create a null-terminated string for safety
        char *sql = malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            // Try different prepFlags values
            unsigned int flags[] = {0, 1, 2, 4};
            for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
                sqlite3_prepare_v3(db, sql, (int)size, flags[i], &stmt, &pzTail8);
                if (stmt) {
                    sqlite3_finalize(stmt);
                    stmt = NULL;
                }
            }
            free(sql);
        }
    }
}

static void fuzz_complete_functions(const uint8_t *data, size_t size) {
    // Test sqlite3_complete
    if (size > 0) {
        char *sql = malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            sqlite3_complete(sql);
            free(sql);
        }
    }
    
    // Test sqlite3_complete16
    if (size > 0) {
        // Ensure we have an even number of bytes for UTF-16
        size_t utf16_size = size;
        if (utf16_size % 2 != 0) {
            utf16_size--;
        }
        if (utf16_size > 0) {
            // Create null-terminated UTF-16 string
            uint8_t *utf16_sql = malloc(utf16_size + 2);
            if (utf16_sql) {
                memcpy(utf16_sql, data, utf16_size);
                utf16_sql[utf16_size] = 0;
                utf16_sql[utf16_size + 1] = 0;
                sqlite3_complete16(utf16_sql);
                free(utf16_sql);
            }
        }
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Test prepare functions
    fuzz_prepare_functions(data, size, db);
    
    // Test complete functions
    fuzz_complete_functions(data, size);
    
    // Close database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}
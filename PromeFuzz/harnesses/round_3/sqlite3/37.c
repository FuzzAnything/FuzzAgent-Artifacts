// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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

static void dummy_error_handler(void *arg, int err_code, const char *err_msg) {
    // Dummy error handler to satisfy compiler
    (void)arg;
    (void)err_code;
    (void)err_msg;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Limit input size to prevent excessive memory usage
    if (Size > 1024) {
        return 0;
    }

    // Create a dummy database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK) {
        // If we can't open a database, we'll use a NULL pointer for some calls
        // to test error handling
        sqlite3_errcode(NULL);
        sqlite3_extended_errcode(NULL);
        sqlite3_errmsg(NULL);
        
        // Still try to use mprintf with the fuzz data
        if (Size > 0) {
            char *format_str = (char*)malloc(Size + 1);
            if (format_str) {
                memcpy(format_str, Data, Size);
                format_str[Size] = '\0';
                char *result = sqlite3_mprintf("%s", format_str);
                if (result) {
                    sqlite3_free(result);
                }
                free(format_str);
            }
        }
        return 0;
    }

    // Set up some database state to potentially generate errors
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a, b);", NULL, NULL, NULL);
    
    // Use fuzz data to create a potentially invalid SQL statement
    if (Size > 0) {
        char *sql = (char*)malloc(Size + 50);
        if (sql) {
            // Create SQL that might succeed or fail based on fuzz input
            memcpy(sql, "INSERT INTO t1 VALUES(", 22);
            size_t copy_len = Size < 20 ? Size : 20;
            memcpy(sql + 22, Data, copy_len);
            sql[22 + copy_len] = ')';
            sql[23 + copy_len] = ';';
            sql[24 + copy_len] = '\0';
            
            // Execute potentially invalid SQL to generate error state
            char *err_msg = NULL;
            sqlite3_exec(db, sql, NULL, NULL, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
            }
            free(sql);
        }
    }

    // Call target functions in specified order
    
    // 1. sqlite3_errcode
    int errcode = sqlite3_errcode(db);
    (void)errcode; // Use result to avoid unused variable warning
    
    // 2. sqlite3_mprintf - use fuzz data as format string content
    char *mprintf_result = NULL;
    if (Size > 0) {
        // Create a format string using fuzz data
        char *format_content = (char*)malloc(Size + 1);
        if (format_content) {
            memcpy(format_content, Data, Size);
            format_content[Size] = '\0';
            
            // Try different format specifiers
            mprintf_result = sqlite3_mprintf("Test: %s", format_content);
            if (mprintf_result) {
                sqlite3_free(mprintf_result);
                mprintf_result = NULL;
            }
            
            // Try with SQLite-specific format specifiers
            mprintf_result = sqlite3_mprintf("%Q", format_content);
            if (mprintf_result) {
                sqlite3_free(mprintf_result);
                mprintf_result = NULL;
            }
            
            free(format_content);
        }
    } else {
        // Test with empty input
        mprintf_result = sqlite3_mprintf("Empty input test");
        if (mprintf_result) {
            sqlite3_free(mprintf_result);
        }
    }
    
    // 3. sqlite3_errmsg
    const char *errmsg = sqlite3_errmsg(db);
    (void)errmsg; // Use result to avoid unused variable warning
    
    // 4. sqlite3_extended_errcode
    int extended_errcode = sqlite3_extended_errcode(db);
    (void)extended_errcode; // Use result to avoid unused variable warning
    
    // Additional fuzzing: try to trigger different error states
    // by performing invalid operations
    
    // Try to prepare an invalid statement
    sqlite3_stmt *stmt = NULL;
    if (Size > 0) {
        char *invalid_sql = (char*)malloc(Size + 1);
        if (invalid_sql) {
            memcpy(invalid_sql, Data, Size);
            invalid_sql[Size] = '\0';
            sqlite3_prepare_v2(db, invalid_sql, -1, &stmt, NULL);
            free(invalid_sql);
        }
    }
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Try to set error handler
    sqlite3_exec(db, "PRAGMA invalid_pragma;", NULL, NULL, NULL);
    
    // Call error functions again after generating a new error
    sqlite3_errcode(db);
    sqlite3_errmsg(db);
    sqlite3_extended_errcode(db);
    
    // Test with NULL db pointer
    sqlite3_errcode(NULL);
    sqlite3_errmsg(NULL);
    sqlite3_extended_errcode(NULL);
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
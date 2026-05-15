// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:132612:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create an in-memory database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Create a simple table for testing
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);", NULL, NULL, NULL);
    
    // Prepare statements using different APIs
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    
    // Use first byte to control which APIs to test
    uint8_t api_selector = Data[0];
    size_t sql_offset = 1;
    
    if (sql_offset >= Size) {
        sqlite3_close(db);
        return 0;
    }
    
    // Extract SQL string from fuzzer input
    size_t sql_len = Size - sql_offset;
    const char *sql_utf8 = (const char *)(Data + sql_offset);
    
    // Convert to UTF-16 for UTF-16 APIs
    size_t utf16_len = sql_len * 2 + 2;
    void *sql_utf16 = malloc(utf16_len);
    if (sql_utf16 == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Simple ASCII to UTF-16LE conversion
    for (size_t i = 0; i < sql_len; i++) {
        ((uint8_t *)sql_utf16)[i*2] = sql_utf8[i];
        ((uint8_t *)sql_utf16)[i*2 + 1] = 0;
    }
    // Null terminate
    ((uint8_t *)sql_utf16)[sql_len*2] = 0;
    ((uint8_t *)sql_utf16)[sql_len*2 + 1] = 0;
    
    // Test different prepare APIs based on selector
    if (api_selector & 0x01) {
        // sqlite3_prepare
        rc = sqlite3_prepare(db, sql_utf8, (int)sql_len, &stmt, NULL);
        if (rc == SQLITE_OK && stmt != NULL) {
            int readonly = sqlite3_stmt_readonly(stmt);
            (void)readonly;
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    if (api_selector & 0x02) {
        // sqlite3_prepare16
        rc = sqlite3_prepare16(db, sql_utf16, (int)(sql_len * 2), &stmt, &pzTail);
        if (rc == SQLITE_OK && stmt != NULL) {
            int readonly = sqlite3_stmt_readonly(stmt);
            (void)readonly;
            
            // Test sqlite3_stmt_status with different operations
            for (int op = 0; op <= 10; op++) {
                int status = sqlite3_stmt_status(stmt, op, (op % 2));
                (void)status;
            }
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    if (api_selector & 0x04) {
        // sqlite3_prepare16_v2
        rc = sqlite3_prepare16_v2(db, sql_utf16, (int)(sql_len * 2), &stmt, &pzTail);
        if (rc == SQLITE_OK && stmt != NULL) {
            int readonly = sqlite3_stmt_readonly(stmt);
            (void)readonly;
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    if (api_selector & 0x08) {
        // sqlite3_prepare16_v3 with different flags
        unsigned int prepFlags = (unsigned int)(Data[0] << 8) | Data[1 % Size];
        rc = sqlite3_prepare16_v3(db, sql_utf16, (int)(sql_len * 2), prepFlags, &stmt, &pzTail);
        if (rc == SQLITE_OK && stmt != NULL) {
            int readonly = sqlite3_stmt_readonly(stmt);
            (void)readonly;
            
            // Test sqlite3_stmt_status with reset flag variations
            for (int reset = 0; reset < 2; reset++) {
                for (int op = 0; op <= 10; op++) {
                    int status = sqlite3_stmt_status(stmt, op, reset);
                    (void)status;
                }
            }
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test with different nByte values
    if (api_selector & 0x10) {
        // Test with negative nByte (read to null terminator)
        rc = sqlite3_prepare(db, sql_utf8, -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt != NULL) {
            int readonly = sqlite3_stmt_readonly(stmt);
            (void)readonly;
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        
        // Test with zero nByte
        rc = sqlite3_prepare(db, sql_utf8, 0, &stmt, NULL);
        if (rc == SQLITE_OK && stmt != NULL) {
            int readonly = sqlite3_stmt_readonly(stmt);
            (void)readonly;
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        
        // Test with positive nByte less than actual length
        if (sql_len > 10) {
            rc = sqlite3_prepare(db, sql_utf8, 10, &stmt, NULL);
            if (rc == SQLITE_OK && stmt != NULL) {
                int readonly = sqlite3_stmt_readonly(stmt);
                (void)readonly;
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
    }
    
    // Test with file-based database
    if (api_selector & 0x20) {
        sqlite3 *file_db = NULL;
        rc = sqlite3_open("./dummy_file", &file_db);
        if (rc == SQLITE_OK && file_db != NULL) {
            // Create table
            sqlite3_exec(file_db, "CREATE TABLE IF NOT EXISTS fuzz (data BLOB);", NULL, NULL, NULL);
            
            // Prepare statement
            rc = sqlite3_prepare(file_db, sql_utf8, (int)sql_len, &stmt, NULL);
            if (rc == SQLITE_OK && stmt != NULL) {
                int readonly = sqlite3_stmt_readonly(stmt);
                (void)readonly;
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
            
            sqlite3_close(file_db);
            remove("./dummy_file");
        }
    }
    
    free(sql_utf16);
    sqlite3_close(db);
    
    return 0;
}
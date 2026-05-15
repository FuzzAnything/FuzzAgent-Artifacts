// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:132612:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "sqlite3.h"

static void dummy_busy_handler(void *data, int count) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
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
    
    // Set a busy handler to avoid potential hangs
    sqlite3_busy_handler(db, (int(*)(void*,int))dummy_busy_handler, NULL);
    
    // Create a dummy table to have something to query
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(id INTEGER, name TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some dummy data
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'test1');", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(2, 'test2');", NULL, NULL, NULL);
    
    // Calculate safe size for SQL statements (capped at INT_MAX)
    int safe_size = (int)(Size > (size_t)INT_MAX ? INT_MAX : Size);
    
    // Test sqlite3_prepare16_v3
    if (safe_size >= 2) {  // Need at least 2 bytes for UTF-16
        unsigned int prepFlags = 0;
        if (Size > 0) prepFlags = Data[0] % 4;  // Use first byte for flags (0-3)
        
        rc = sqlite3_prepare16_v3(db, (const void*)Data, 
                                  safe_size, 
                                  prepFlags, &stmt, &pzTail);
        
        if (rc == SQLITE_OK && stmt != NULL) {
            // Test sqlite3_stmt_readonly
            int readonly = sqlite3_stmt_readonly(stmt);
            (void)readonly;  // Use result to avoid unused variable warning
            
            // Test sqlite3_column_count
            int col_count = sqlite3_column_count(stmt);
            (void)col_count;
            
            // Test sqlite3_stmt_status with various operations
            // SQLITE_STMTSTATUS_FULLSCAN_STEP = 1, SQLITE_STMTSTATUS_SORT = 2, etc.
            int resetFlg = (Size > 1) ? (Data[1] & 1) : 0;
            for (int op = 1; op <= 5; op++) {
                int status = sqlite3_stmt_status(stmt, op, resetFlg);
                (void)status;
            }
            
            // Try to step through the statement to explore more states
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Access some columns if they exist
                for (int i = 0; i < sqlite3_column_count(stmt); i++) {
                    int col_type = sqlite3_column_type(stmt, i);
                    (void)col_type;
                }
            }
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test sqlite3_prepare16
    if (safe_size >= 2) {
        rc = sqlite3_prepare16(db, (const void*)Data, 
                               safe_size, 
                               &stmt, &pzTail);
        
        if (rc == SQLITE_OK && stmt != NULL) {
            // Exercise the statement
            int readonly = sqlite3_stmt_readonly(stmt);
            (void)readonly;
            
            int col_count = sqlite3_column_count(stmt);
            (void)col_count;
            
            // Try stepping
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Do nothing, just step through
            }
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test sqlite3_prepare16_v2
    if (safe_size >= 2) {
        rc = sqlite3_prepare16_v2(db, (const void*)Data, 
                                  safe_size, 
                                  &stmt, &pzTail);
        
        if (rc == SQLITE_OK && stmt != NULL) {
            // Exercise the statement
            int readonly = sqlite3_stmt_readonly(stmt);
            (void)readonly;
            
            int col_count = sqlite3_column_count(stmt);
            (void)col_count;
            
            // Test stmt_status with different reset flags
            int resetFlg = (Size > 2) ? (Data[2] & 1) : 0;
            for (int op = 1; op <= 5; op++) {
                int status = sqlite3_stmt_status(stmt, op, resetFlg);
                (void)status;
            }
            
            // Try stepping
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Do nothing, just step through
            }
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test with a file-based database as well
    const char *filename = "./dummy_file";
    sqlite3 *file_db = NULL;
    rc = sqlite3_open(filename, &file_db);
    if (rc == SQLITE_OK && file_db != NULL) {
        // Create table and insert data
        sqlite3_exec(file_db, "CREATE TABLE IF NOT EXISTS t2(x INTEGER, y TEXT);", NULL, NULL, NULL);
        sqlite3_exec(file_db, "INSERT INTO t2 VALUES(10, 'file1');", NULL, NULL, NULL);
        
        if (safe_size >= 2) {
            rc = sqlite3_prepare16_v2(file_db, (const void*)Data, 
                                      safe_size, 
                                      &stmt, &pzTail);
            
            if (rc == SQLITE_OK && stmt != NULL) {
                // Test all the functions
                int readonly = sqlite3_stmt_readonly(stmt);
                (void)readonly;
                
                int col_count = sqlite3_column_count(stmt);
                (void)col_count;
                
                int resetFlg = (Size > 3) ? (Data[3] & 1) : 0;
                for (int op = 1; op <= 5; op++) {
                    int status = sqlite3_stmt_status(stmt, op, resetFlg);
                    (void)status;
                }
                
                // Try stepping
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    // Do nothing
                }
                
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
        
        sqlite3_close(file_db);
        // Remove the dummy file
        remove(filename);
    }
    
    // Clean up
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    
    return 0;
}
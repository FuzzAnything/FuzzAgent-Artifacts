// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    // Initialize database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table
    char *err_msg = NULL;
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare some statements
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt3 = NULL;
    
    // Prepare first statement (SELECT)
    rc = sqlite3_prepare_v2(db, "SELECT * FROM test;", -1, &stmt1, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare second statement (INSERT)
    rc = sqlite3_prepare_v2(db, "INSERT INTO test (data) VALUES (?);", -1, &stmt2, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare third statement (UPDATE)
    rc = sqlite3_prepare_v2(db, "UPDATE test SET data = ? WHERE id = 1;", -1, &stmt3, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_finalize(stmt2);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer input to determine which operations to perform
    if (Size > 0) {
        uint8_t operation = Data[0] % 6;
        size_t data_offset = (Size > 1) ? 1 : 0;
        
        switch (operation) {
            case 0: {
                // Test sqlite3_reset
                if (stmt1) {
                    // First step the statement to change its state
                    sqlite3_step(stmt1);
                    int reset_rc = sqlite3_reset(stmt1);
                    // Return value can be checked but we don't need to act on it
                    (void)reset_rc;
                }
                break;
            }
            case 1: {
                // Test sqlite3_db_handle
                sqlite3 *db_handle = NULL;
                if (stmt1) {
                    db_handle = sqlite3_db_handle(stmt1);
                }
                if (db_handle) {
                    // Use the handle for something simple
                    sqlite3_stmt *temp_stmt = NULL;
                    sqlite3_prepare_v2(db_handle, "SELECT 1;", -1, &temp_stmt, NULL);
                    if (temp_stmt) {
                        sqlite3_finalize(temp_stmt);
                    }
                }
                break;
            }
            case 2: {
                // Test sqlite3_column_count
                int col_count = 0;
                if (stmt1) {
                    col_count = sqlite3_column_count(stmt1);
                }
                if (stmt2) {
                    col_count = sqlite3_column_count(stmt2);
                }
                (void)col_count; // Use the value to avoid unused warning
                break;
            }
            case 3: {
                // Test sqlite3_finalize
                if (stmt3) {
                    int finalize_rc = sqlite3_finalize(stmt3);
                    stmt3 = NULL; // Set to NULL after finalizing
                    (void)finalize_rc;
                }
                break;
            }
            case 4: {
                // Test sqlite3_interrupt
                sqlite3_interrupt(db);
                break;
            }
            case 5: {
                // Test sqlite3_next_stmt
                sqlite3_stmt *next_stmt = NULL;
                if (stmt1) {
                    next_stmt = sqlite3_next_stmt(db, stmt1);
                } else {
                    next_stmt = sqlite3_next_stmt(db, NULL);
                }
                
                // Try to get all statements in a loop
                sqlite3_stmt *current = NULL;
                while ((current = sqlite3_next_stmt(db, current)) != NULL) {
                    // Just iterate through them
                }
                break;
            }
        }
        
        // Try binding some data if we have input
        if (data_offset < Size && stmt2) {
            size_t bind_size = Size - data_offset;
            if (bind_size > 0) {
                // Limit size to something reasonable
                if (bind_size > 1024) bind_size = 1024;
                sqlite3_bind_text(stmt2, 1, (const char*)Data + data_offset, 
                                 (int)bind_size, SQLITE_STATIC);
                sqlite3_step(stmt2);
                sqlite3_reset(stmt2);
            }
        }
    }
    
    // Cleanup - finalize any remaining statements
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (stmt3) sqlite3_finalize(stmt3);
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}
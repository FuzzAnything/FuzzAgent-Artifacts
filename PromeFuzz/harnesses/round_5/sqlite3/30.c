// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
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

static void cleanup_database(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
    }
}

static void cleanup_statement(sqlite3_stmt* stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    sqlite3* db = NULL;
    sqlite3_stmt* stmt = NULL;
    const char* err_msg = NULL;
    int rc;
    int changes;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        cleanup_database(db);
        return 0;
    }
    
    // Create a simple table for testing
    const char* create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        cleanup_database(db);
        return 0;
    }
    
    // Prepare a statement for testing
    const char* insert_sql = "INSERT INTO test (id, name) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        cleanup_statement(stmt);
        cleanup_database(db);
        return 0;
    }
    
    // First sqlite3_reset call
    rc = sqlite3_reset(stmt);
    
    // Prepare text data for binding
    // Use a safe approach to avoid reading beyond Data buffer
    char* text_data = NULL;
    if (Size > 0) {
        text_data = (char*)malloc(Size + 1);
        if (text_data) {
            memcpy(text_data, Data, Size);
            text_data[Size] = '\0';
        }
    } else {
        text_data = (char*)malloc(1);
        if (text_data) {
            text_data[0] = '\0';
        }
    }
    
    if (!text_data) {
        cleanup_statement(stmt);
        cleanup_database(db);
        return 0;
    }
    
    // sqlite3_bind_text call
    // Use parameter index 1 for id and 2 for name
    sqlite3_bind_int(stmt, 1, (int)(Size % 1000));  // Use Size to generate some id
    rc = sqlite3_bind_text(stmt, 2, text_data, -1, SQLITE_TRANSIENT);
    
    free(text_data);
    
    // sqlite3_step call
    rc = sqlite3_step(stmt);
    
    // Handle different return codes from sqlite3_step
    if (rc == SQLITE_ROW) {
        // This shouldn't happen for INSERT, but handle it
    } else if (rc == SQLITE_DONE) {
        // INSERT completed successfully
    } else {
        // Some error occurred
    }
    
    // sqlite3_errmsg call
    err_msg = sqlite3_errmsg(db);
    
    // Second sqlite3_reset call
    rc = sqlite3_reset(stmt);
    
    // sqlite3_changes call
    changes = sqlite3_changes(db);
    
    // Additional fuzzing: try different parameter values for bind_text
    // Reset and bind with different parameters
    rc = sqlite3_reset(stmt);
    
    // Bind with empty string
    rc = sqlite3_bind_text(stmt, 2, "", 0, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    
    // Reset and bind with NULL
    rc = sqlite3_reset(stmt);
    rc = sqlite3_bind_text(stmt, 2, NULL, 0, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    
    // Clean up
    cleanup_statement(stmt);
    
    // Test with a different statement
    const char* select_sql = "SELECT * FROM test WHERE name LIKE ?;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        // Reset
        rc = sqlite3_reset(stmt);
        
        // Bind with wildcard pattern
        rc = sqlite3_bind_text(stmt, 1, "%test%", 6, SQLITE_STATIC);
        
        // Step through results
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Just step through, don't process results
        }
        
        // Get error message
        err_msg = sqlite3_errmsg(db);
        
        // Reset again
        rc = sqlite3_reset(stmt);
        
        // Get changes (should be 0 for SELECT)
        changes = sqlite3_changes(db);
        
        cleanup_statement(stmt);
    }
    
    cleanup_database(db);
    
    return 0;
}
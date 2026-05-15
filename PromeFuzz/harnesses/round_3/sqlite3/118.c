// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void fuzz_prepare_and_execute(sqlite3 *db, const uint8_t *sql_data, size_t sql_size) {
    if (sql_size == 0) return;
    
    // Create a null-terminated SQL string from the fuzzer input
    char *sql = malloc(sql_size + 1);
    if (!sql) return;
    
    memcpy(sql, sql_data, sql_size);
    sql[sql_size] = '\0';
    
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK && stmt) {
        // Test sqlite3_db_handle
        sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
        if (db_from_stmt != db) {
            // This should not happen, but we note it
        }
        
        // Test sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        (void)col_count; // Use the value to avoid unused variable warning
        
        // Execute the statement a few times
        for (int i = 0; i < 3; i++) {
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
                break;
            }
        }
        
        // Test sqlite3_reset
        sqlite3_reset(stmt);
        
        // Test sqlite3_finalize
        sqlite3_finalize(stmt);
    }
    
    free(sql);
}

static void fuzz_iterate_statements(sqlite3 *db) {
    // Test sqlite3_next_stmt iteration
    sqlite3_stmt *stmt = NULL;
    while ((stmt = sqlite3_next_stmt(db, stmt)) != NULL) {
        // For each statement found, test various functions
        sqlite3 *db_handle = sqlite3_db_handle(stmt);
        if (db_handle != db) {
            // Should not happen
        }
        
        int col_count = sqlite3_column_count(stmt);
        (void)col_count;
        
        // Try to reset the statement
        sqlite3_reset(stmt);
    }
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Open an in-memory database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    // Create a simple table for testing
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);"
        "CREATE TABLE IF NOT EXISTS test2 (x INTEGER, y TEXT);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    // Insert some test data
    sqlite3_exec(db, 
        "INSERT INTO test (name) VALUES ('test1'), ('test2'), ('test3');"
        "INSERT INTO test2 (x, y) VALUES (1, 'a'), (2, 'b'), (3, 'c');",
        NULL, NULL, NULL);
    
    // Use fuzzer input to create SQL statements
    size_t sql_size = Size > 1024 ? 1024 : Size;
    fuzz_prepare_and_execute(db, Data, sql_size);
    
    // Test sqlite3_next_stmt with various starting points
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_prepare_v2(db, "SELECT * FROM test", -1, &stmt1, NULL);
    
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_prepare_v2(db, "SELECT * FROM test2", -1, &stmt2, NULL);
    
    // Test sqlite3_next_stmt starting from NULL (should return first statement)
    sqlite3_stmt *first_stmt = sqlite3_next_stmt(db, NULL);
    (void)first_stmt;
    
    // Test sqlite3_next_stmt starting from stmt1 (should return stmt2 or NULL)
    sqlite3_stmt *next_stmt = sqlite3_next_stmt(db, stmt1);
    (void)next_stmt;
    
    // Test sqlite3_next_stmt starting from stmt2 (should return NULL)
    sqlite3_stmt *null_stmt = sqlite3_next_stmt(db, stmt2);
    (void)null_stmt;
    
    // Test sqlite3_db_handle with NULL input
    sqlite3 *null_db_handle = sqlite3_db_handle(NULL);
    (void)null_db_handle;
    
    // Test sqlite3_column_count on various statements
    if (stmt1) {
        int col_count1 = sqlite3_column_count(stmt1);
        (void)col_count1;
    }
    
    if (stmt2) {
        int col_count2 = sqlite3_column_count(stmt2);
        (void)col_count2;
    }
    
    // Test sqlite3_interrupt
    sqlite3_interrupt(db);
    
    // Create another statement and step through it while interrupted
    sqlite3_stmt *stmt3 = NULL;
    sqlite3_prepare_v2(db, "SELECT * FROM test ORDER BY id", -1, &stmt3, NULL);
    if (stmt3) {
        sqlite3_step(stmt3);
        sqlite3_reset(stmt3);
    }
    
    // Test sqlite3_finalize with NULL
    sqlite3_finalize(NULL);
    
    // Test sqlite3_reset with NULL
    sqlite3_reset(NULL);
    
    // Finalize all statements
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (stmt3) sqlite3_finalize(stmt3);
    
    // Iterate through any remaining statements
    fuzz_iterate_statements(db);
    
    // Close the database
    sqlite3_close(db);
    
    return 0;
}
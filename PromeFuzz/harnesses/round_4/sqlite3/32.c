// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *sql_text = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy table and insert some data for testing
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, value INTEGER);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare first statement
    const char *select_sql = "SELECT id, value FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt1, NULL);
    if (rc != SQLITE_OK || stmt1 == NULL) {
        // If prepare fails, try with a simple statement that always works
        const char *simple_sql = "SELECT 1, 2;";
        rc = sqlite3_prepare_v2(db, simple_sql, -1, &stmt1, NULL);
        if (rc != SQLITE_OK || stmt1 == NULL) {
            sqlite3_close(db);
            return 0;
        }
    }
    
    // First sqlite3_sql call
    sql_text = sqlite3_sql(stmt1);
    
    // First sqlite3_step call
    rc = sqlite3_step(stmt1);
    
    // Second sqlite3_sql call
    sql_text = sqlite3_sql(stmt1);
    
    // First sqlite3_column_int call (if we have a row)
    if (rc == SQLITE_ROW) {
        int col1 = sqlite3_column_int(stmt1, 0);
        // Second sqlite3_column_int call
        int col2 = sqlite3_column_int(stmt1, 1);
    }
    
    // sqlite3_finalize for first statement
    rc = sqlite3_finalize(stmt1);
    
    // sqlite3_db_release_memory
    rc = sqlite3_db_release_memory(db);
    
    // Prepare second statement
    // Use fuzzer input to create SQL if we have enough data
    if (Size > 0) {
        // Create a temporary buffer for SQL
        char *sql_buffer = malloc(Size + 1);
        if (sql_buffer) {
            memcpy(sql_buffer, Data, Size);
            sql_buffer[Size] = '\0';
            
            // Try to prepare with fuzzer input
            rc = sqlite3_prepare_v2(db, sql_buffer, -1, &stmt2, NULL);
            free(sql_buffer);
            
            if (rc != SQLITE_OK || stmt2 == NULL) {
                // Fallback to simple statement
                const char *fallback_sql = "SELECT 3, 4;";
                rc = sqlite3_prepare_v2(db, fallback_sql, -1, &stmt2, NULL);
            }
        } else {
            // Memory allocation failed, use simple statement
            const char *simple_sql = "SELECT 3, 4;";
            rc = sqlite3_prepare_v2(db, simple_sql, -1, &stmt2, NULL);
        }
    } else {
        // No fuzzer data, use simple statement
        const char *simple_sql = "SELECT 3, 4;";
        rc = sqlite3_prepare_v2(db, simple_sql, -1, &stmt2, NULL);
    }
    
    if (rc != SQLITE_OK || stmt2 == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Third sqlite3_sql call
    sql_text = sqlite3_sql(stmt2);
    
    // Second sqlite3_step call
    rc = sqlite3_step(stmt2);
    
    // Fourth sqlite3_sql call
    sql_text = sqlite3_sql(stmt2);
    
    // sqlite3_finalize for second statement
    rc = sqlite3_finalize(stmt2);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
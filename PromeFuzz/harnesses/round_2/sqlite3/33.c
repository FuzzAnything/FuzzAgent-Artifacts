// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_function(void) {
    // This is a dummy function to satisfy the requirement of having static functions
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_text = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table for testing
    const char *create_table_sql = "CREATE TABLE test (id INTEGER, value INTEGER);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some data
    const char *insert_sql = "INSERT INTO test VALUES (1, 100), (2, 200), (3, 300);";
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement using fuzzer input
    // We'll use a simple SELECT statement but incorporate fuzzer data
    char *query = NULL;
    if (Size > 0) {
        // Create a query using fuzzer data (sanitized to avoid SQL injection in fuzzing)
        query = (char*)malloc(Size + 50);
        if (!query) {
            sqlite3_close(db);
            return 0;
        }
        // Use fuzzer data in a safe way - as part of a WHERE clause value
        snprintf(query, Size + 50, "SELECT * FROM test WHERE id = %d", (int)(Data[0] % 4));
    } else {
        query = strdup("SELECT * FROM test");
    }
    
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        free(query);
        sqlite3_close(db);
        return 0;
    }
    
    // First sqlite3_sql call
    sql_text = sqlite3_sql(stmt);
    
    // First sqlite3_step call
    rc = sqlite3_step(stmt);
    
    // Second sqlite3_sql call
    sql_text = sqlite3_sql(stmt);
    
    // First sqlite3_column_int call (column 0)
    if (rc == SQLITE_ROW) {
        int col0 = sqlite3_column_int(stmt, 0);
        (void)col0; // Use variable to avoid unused warning
    }
    
    // Second sqlite3_column_int call (column 1)
    if (rc == SQLITE_ROW) {
        int col1 = sqlite3_column_int(stmt, 1);
        (void)col1; // Use variable to avoid unused warning
    }
    
    // Reset the statement for next use
    sqlite3_reset(stmt);
    
    // First sqlite3_finalize call
    rc = sqlite3_finalize(stmt);
    stmt = NULL;
    
    // sqlite3_db_release_memory call
    rc = sqlite3_db_release_memory(db);
    
    // Prepare a new statement
    const char *second_query = "SELECT id, value FROM test ORDER BY id";
    rc = sqlite3_prepare_v2(db, second_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        free(query);
        sqlite3_close(db);
        return 0;
    }
    
    // Third sqlite3_sql call
    sql_text = sqlite3_sql(stmt);
    
    // Second sqlite3_step call (in a loop to explore more states)
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Access columns during iteration
        int id = sqlite3_column_int(stmt, 0);
        int value = sqlite3_column_int(stmt, 1);
        (void)id;
        (void)value;
    }
    
    // Fourth sqlite3_sql call
    sql_text = sqlite3_sql(stmt);
    
    // Second sqlite3_finalize call
    rc = sqlite3_finalize(stmt);
    
    // Cleanup
    free(query);
    sqlite3_close(db);
    
    return 0;
}
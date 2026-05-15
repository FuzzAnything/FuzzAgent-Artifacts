// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void create_test_database(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return;
    
    // Create tables with various column types
    const char *create_sql = 
        "CREATE TABLE main_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);"
        "CREATE TEMP TABLE temp_table (temp_id INTEGER, temp_data TEXT);"
        "CREATE TABLE attached_db.other_table (col1 INTEGER, col2 TEXT);"
        "INSERT INTO main_table VALUES (1, 'test1', 3.14);"
        "INSERT INTO main_table VALUES (2, 'test2', 2.71);"
        "INSERT INTO temp_table VALUES (1, 'temp_data');"
        "INSERT INTO attached_db.other_table VALUES (10, 'attached_data');";
    
    rc = sqlite3_exec(*db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) return;
    
    // Attach another in-memory database
    rc = sqlite3_exec(*db, "ATTACH DATABASE ':memory:' AS attached_db", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return;
    
    // Prepare a complex query that joins tables from different databases
    const char *select_sql = 
        "SELECT m.id, m.name, t.temp_data, o.col2 "
        "FROM main_table m "
        "LEFT JOIN temp_table t ON m.id = t.temp_id "
        "LEFT JOIN attached_db.other_table o ON m.id + 9 = o.col1";
    
    rc = sqlite3_prepare_v2(*db, select_sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) return;
    
    // Step to get the first row
    rc = sqlite3_step(*stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(*stmt);
        *stmt = NULL;
    }
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Create test database and prepared statement
    create_test_database(&db, &stmt);
    if (!db || !stmt) {
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer input to determine column index and other parameters
    int col_index = 0;
    if (Size > 0) {
        col_index = Data[0] % 4;  // We have 4 columns in our query
    }
    
    // Call all target functions with the prepared statement
    const void *result1 = sqlite3_column_database_name16(stmt, col_index);
    const void *result2 = sqlite3_column_decltype16(stmt, col_index);
    const void *result3 = sqlite3_column_text16(stmt, col_index);
    const void *result4 = sqlite3_column_origin_name16(stmt, col_index);
    const void *result5 = sqlite3_column_table_name16(stmt, col_index);
    const void *result6 = sqlite3_column_name16(stmt, col_index);
    
    // Try different column indices based on fuzzer input
    if (Size > 1) {
        int alt_col_index = Data[1] % 8;  // Try out of range indices too
        sqlite3_column_database_name16(stmt, alt_col_index);
        sqlite3_column_decltype16(stmt, alt_col_index);
        sqlite3_column_text16(stmt, alt_col_index);
        sqlite3_column_origin_name16(stmt, alt_col_index);
        sqlite3_column_table_name16(stmt, alt_col_index);
        sqlite3_column_name16(stmt, alt_col_index);
    }
    
    // Try stepping through more rows if available
    if (Size > 2) {
        int steps = Data[2] % 3;
        for (int i = 0; i < steps; i++) {
            int rc = sqlite3_step(stmt);
            if (rc != SQLITE_ROW) break;
            
            // Call functions again with different column indices
            int new_col = (col_index + i) % 4;
            sqlite3_column_database_name16(stmt, new_col);
            sqlite3_column_decltype16(stmt, new_col);
            sqlite3_column_text16(stmt, new_col);
            sqlite3_column_origin_name16(stmt, new_col);
            sqlite3_column_table_name16(stmt, new_col);
            sqlite3_column_name16(stmt, new_col);
        }
        
        // Reset statement to explore different states
        sqlite3_reset(stmt);
        sqlite3_step(stmt);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
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

static void initialize_database_and_statement(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    /* Create a table with various column types */
    const char *create_sql = 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB, "
        "timestamp DATETIME"
        ");";
    rc = sqlite3_exec(*db, create_sql, 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return;
    }
    
    /* Insert some sample data */
    const char *insert_sql = 
        "INSERT INTO test_table (name, value, data, timestamp) "
        "VALUES ('test_name', 3.14159, x'010203', '2024-01-01 12:00:00');";
    rc = sqlite3_exec(*db, insert_sql, 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return;
    }
    
    /* Prepare a SELECT statement */
    const char *select_sql = "SELECT * FROM test_table;";
    rc = sqlite3_prepare_v2(*db, select_sql, -1, stmt, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        *stmt = NULL;
        return;
    }
    
    /* Step once to get valid results */
    rc = sqlite3_step(*stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(*stmt);
        sqlite3_close(*db);
        *stmt = NULL;
        *db = NULL;
    }
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    /* Initialize database and prepared statement */
    initialize_database_and_statement(&db, &stmt);
    if (!db || !stmt) {
        return 0;
    }
    
    /* Get column count */
    int column_count = sqlite3_column_count(stmt);
    if (column_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    /* Use fuzzer data to determine which columns to query */
    for (size_t i = 0; i < Size && i < 10; i++) {
        int col_index = Data[i] % column_count;
        
        /* Call all target functions with the computed column index */
        (void)sqlite3_column_database_name16(stmt, col_index);
        (void)sqlite3_column_decltype16(stmt, col_index);
        (void)sqlite3_column_origin_name16(stmt, col_index);
        (void)sqlite3_column_text16(stmt, col_index);
        (void)sqlite3_column_table_name16(stmt, col_index);
        (void)sqlite3_column_name16(stmt, col_index);
    }
    
    /* Also test with some fixed column indices */
    (void)sqlite3_column_database_name16(stmt, 0);
    (void)sqlite3_column_database_name16(stmt, column_count - 1);
    (void)sqlite3_column_decltype16(stmt, 0);
    (void)sqlite3_column_decltype16(stmt, column_count - 1);
    (void)sqlite3_column_origin_name16(stmt, 0);
    (void)sqlite3_column_origin_name16(stmt, column_count - 1);
    (void)sqlite3_column_text16(stmt, 0);
    (void)sqlite3_column_text16(stmt, column_count - 1);
    (void)sqlite3_column_table_name16(stmt, 0);
    (void)sqlite3_column_table_name16(stmt, column_count - 1);
    (void)sqlite3_column_name16(stmt, 0);
    (void)sqlite3_column_name16(stmt, column_count - 1);
    
    /* Test with out-of-bounds indices (should return NULL) */
    (void)sqlite3_column_database_name16(stmt, column_count);
    (void)sqlite3_column_database_name16(stmt, -1);
    (void)sqlite3_column_decltype16(stmt, column_count);
    (void)sqlite3_column_decltype16(stmt, -1);
    (void)sqlite3_column_origin_name16(stmt, column_count);
    (void)sqlite3_column_origin_name16(stmt, -1);
    (void)sqlite3_column_text16(stmt, column_count);
    (void)sqlite3_column_text16(stmt, -1);
    (void)sqlite3_column_table_name16(stmt, column_count);
    (void)sqlite3_column_table_name16(stmt, -1);
    (void)sqlite3_column_name16(stmt, column_count);
    (void)sqlite3_column_name16(stmt, -1);
    
    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
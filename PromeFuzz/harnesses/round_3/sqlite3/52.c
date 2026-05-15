// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;
    
    // Create a table with some columns
    rc = sqlite3_exec(db, 
        "CREATE TABLE test1 (id INTEGER PRIMARY KEY, name TEXT, value REAL);"
        "CREATE TABLE test2 (id INTEGER PRIMARY KEY, data BLOB);"
        "ATTACH DATABASE ':memory:' AS 'aux';"
        "CREATE TABLE aux.test3 (x INTEGER, y TEXT);", 
        callback, 0, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    
    // Insert some dummy data
    rc = sqlite3_exec(db,
        "INSERT INTO test1 VALUES (1, 'test', 3.14);"
        "INSERT INTO test2 VALUES (1, x'010203');"
        "INSERT INTO aux.test3 VALUES (42, 'hello');", 
        callback, 0, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    
    // Prepare a SELECT statement using fuzzer data to influence the query
    char query[256];
    int query_type = Data[0] % 4;
    
    switch (query_type) {
        case 0:
            snprintf(query, sizeof(query), 
                "SELECT test1.id, test1.name, test2.data FROM test1 JOIN test2 ON test1.id = test2.id");
            break;
        case 1:
            snprintf(query, sizeof(query),
                "SELECT * FROM test1");
            break;
        case 2:
            snprintf(query, sizeof(query),
                "SELECT id as col1, name as col2 FROM test1");
            break;
        case 3:
            snprintf(query, sizeof(query),
                "SELECT test1.id, aux.test3.x FROM test1, aux.test3");
            break;
        default:
            snprintf(query, sizeof(query), "SELECT * FROM test1");
    }
    
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &tail);
    if (rc != SQLITE_OK) goto cleanup;
    
    // Step once to get results
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int col_count = sqlite3_column_count(stmt);
        
        // Use fuzzer data to determine which column to test
        int col_index = 0;
        if (col_count > 0) {
            col_index = Data[0] % col_count;
        }
        
        // Test all target functions
        const char *db_name = sqlite3_column_database_name(stmt, col_index);
        int col_type = sqlite3_column_type(stmt, col_index);
        const char *table_name = sqlite3_column_table_name(stmt, col_index);
        const char *col_name = sqlite3_column_name(stmt, col_index);
        const char *origin_name = sqlite3_column_origin_name(stmt, col_index);
        const char *decltype = sqlite3_column_decltype(stmt, col_index);
        
        // Use the results to avoid optimization
        if (db_name) (void)db_name[0];
        (void)col_type;
        if (table_name) (void)table_name[0];
        if (col_name) (void)col_name[0];
        if (origin_name) (void)origin_name[0];
        if (decltype) (void)decltype[0];
        
        // Test with different encodings if available
        #ifdef SQLITE_ENABLE_COLUMN_METADATA
        const void *db_name16 = sqlite3_column_database_name16(stmt, col_index);
        const void *table_name16 = sqlite3_column_table_name16(stmt, col_index);
        const void *col_name16 = sqlite3_column_name16(stmt, col_index);
        const void *origin_name16 = sqlite3_column_origin_name16(stmt, col_index);
        if (db_name16) (void)((const char*)db_name16)[0];
        if (table_name16) (void)((const char*)table_name16)[0];
        if (col_name16) (void)((const char*)col_name16)[0];
        if (origin_name16) (void)((const char*)origin_name16)[0];
        #endif
    }
    
    // Test with different column indices
    if (sqlite3_column_count(stmt) > 0) {
        for (int i = 0; i < sqlite3_column_count(stmt) && i < 3; i++) {
            (void)sqlite3_column_type(stmt, i);
            (void)sqlite3_column_name(stmt, i);
        }
    }
    
    // Reset and test again
    sqlite3_reset(stmt);
    
    // Test with out-of-bounds column indices
    if (sqlite3_column_count(stmt) > 0) {
        int invalid_idx = sqlite3_column_count(stmt) + 1;
        (void)sqlite3_column_type(stmt, invalid_idx);
        (void)sqlite3_column_name(stmt, invalid_idx);
    }

cleanup:
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    
    return 0;
}
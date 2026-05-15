// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_expanded_sql at sqlite3.c:80388:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
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

static void dummy_bind_parameters(sqlite3_stmt *stmt, const uint8_t *data, size_t size) {
    // Simple parameter binding using fuzzer data
    if (size < 1) return;
    
    int param_count = sqlite3_bind_parameter_count(stmt);
    for (int i = 0; i < param_count && i < (int)size; i++) {
        if (data[i] % 3 == 0) {
            sqlite3_bind_int(stmt, i + 1, data[i]);
        } else if (data[i] % 3 == 1) {
            sqlite3_bind_text(stmt, i + 1, "fuzz", 4, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, i + 1);
        }
    }
}

static int create_dummy_database(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return rc;
    
    // Create a simple table with various column types
    const char *create_sql = 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT, value REAL, data BLOB);"
        "INSERT INTO test VALUES (1, 'test1', 3.14, x'010203');"
        "INSERT INTO test VALUES (2, 'test2', 2.71, x'040506');"
        "INSERT INTO test VALUES (3, NULL, NULL, NULL);";
    
    rc = sqlite3_exec(*db, create_sql, NULL, NULL, NULL);
    return rc;
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    // Create in-memory database
    rc = create_dummy_database(&db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare statements with different SQL patterns
    const char *sql_queries[] = {
        "SELECT * FROM test",
        "SELECT id, name FROM test WHERE id > ?",
        "SELECT COUNT(*) FROM test",
        "SELECT name, value FROM test WHERE name LIKE ?",
        "SELECT * FROM test ORDER BY id DESC"
    };
    
    int num_queries = sizeof(sql_queries) / sizeof(sql_queries[0]);
    int query_idx = Size > 0 ? Data[0] % num_queries : 0;
    
    rc = sqlite3_prepare_v2(db, sql_queries[query_idx], -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind parameters if the statement has them
    dummy_bind_parameters(stmt, Data, Size);
    
    // Step through results
    int step_result = sqlite3_step(stmt);
    if (step_result == SQLITE_ROW || step_result == SQLITE_DONE) {
        // Test sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        
        // Test sqlite3_column_name for each column
        for (int i = 0; i < col_count; i++) {
            const char *col_name = sqlite3_column_name(stmt, i);
            (void)col_name; // Use variable to avoid unused warning
        }
        
        // Test sqlite3_column_type for each column
        if (step_result == SQLITE_ROW) {
            for (int i = 0; i < col_count; i++) {
                int col_type = sqlite3_column_type(stmt, i);
                (void)col_type;
            }
        }
        
        // Test sqlite3_sql
        const char *original_sql = sqlite3_sql(stmt);
        (void)original_sql;
        
        // Test sqlite3_expanded_sql
        char *expanded_sql = sqlite3_expanded_sql(stmt);
        if (expanded_sql) {
            sqlite3_free(expanded_sql);
        }
        
        // Test sqlite3_normalized_sql
        const char *normalized_sql = sqlite3_normalized_sql(stmt);
        (void)normalized_sql;
        
        // Reset statement and try again with different column indices
        sqlite3_reset(stmt);
        
        // Use fuzzer data to select column indices
        if (Size > 1 && col_count > 0) {
            for (size_t i = 1; i < Size && i < 10; i++) {
                int col_idx = Data[i] % col_count;
                const char *name = sqlite3_column_name(stmt, col_idx);
                (void)name;
                
                if (step_result == SQLITE_ROW) {
                    int type = sqlite3_column_type(stmt, col_idx);
                    (void)type;
                }
            }
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
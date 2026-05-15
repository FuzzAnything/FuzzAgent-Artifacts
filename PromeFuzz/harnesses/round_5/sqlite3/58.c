// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void execute_query(sqlite3 *db, const char *sql) {
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process rows if needed
        }
        sqlite3_finalize(stmt);
    }
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    static const char *dummy_file = "./dummy_file";
    
    // Write fuzzer data to dummy file
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create tables with various schemas
    execute_query(db, "CREATE TABLE t1(id INTEGER PRIMARY KEY, name TEXT, value REAL);");
    execute_query(db, "CREATE TABLE t2(x INTEGER, y TEXT, z BLOB);");
    execute_query(db, "CREATE TABLE t3(a INTEGER, b TEXT, c REAL, d BLOB);");
    
    // Insert some data
    execute_query(db, "INSERT INTO t1(name, value) VALUES('test1', 1.5);");
    execute_query(db, "INSERT INTO t1(name, value) VALUES('test2', 2.7);");
    execute_query(db, "INSERT INTO t2(x, y) VALUES(10, 'hello');");
    execute_query(db, "INSERT INTO t3(a, b, c) VALUES(100, 'world', 3.14);");
    
    // Prepare various SELECT statements
    const char *queries[] = {
        "SELECT * FROM t1",
        "SELECT id, name FROM t1",
        "SELECT t1.id, t2.y FROM t1, t2",
        "SELECT t1.name AS alias1, t2.y AS alias2 FROM t1, t2",
        "SELECT COUNT(*), MAX(value) FROM t1",
        "SELECT id+1, name||'suffix' FROM t1",
        "SELECT * FROM t3"
    };
    
    int num_queries = sizeof(queries) / sizeof(queries[0]);
    
    for (int q = 0; q < num_queries; q++) {
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, queries[q], -1, &stmt, NULL) == SQLITE_OK) {
            // Step once to get column metadata
            sqlite3_step(stmt);
            
            int col_count = sqlite3_column_count(stmt);
            if (col_count > 0) {
                // Use fuzzer data to determine which column to test
                int col_idx = 0;
                if (Size > 0) {
                    col_idx = Data[0] % col_count;
                }
                
                // Test all target functions
                const char *name = sqlite3_column_name(stmt, col_idx);
                const char *origin = sqlite3_column_origin_name(stmt, col_idx);
                const char *db_name = sqlite3_column_database_name(stmt, col_idx);
                const char *table_name = sqlite3_column_table_name(stmt, col_idx);
                int col_type = sqlite3_column_type(stmt, col_idx);
                const char *decl_type = sqlite3_column_decltype(stmt, col_idx);
                
                // Use the results to avoid optimization
                (void)name;
                (void)origin;
                (void)db_name;
                (void)table_name;
                (void)col_type;
                (void)decl_type;
                
                // Test with different column indices
                for (int i = 0; i < col_count && i < 5; i++) {
                    sqlite3_column_name(stmt, i);
                    sqlite3_column_origin_name(stmt, i);
                    sqlite3_column_database_name(stmt, i);
                    sqlite3_column_table_name(stmt, i);
                    sqlite3_column_type(stmt, i);
                    sqlite3_column_decltype(stmt, i);
                }
            }
            
            sqlite3_finalize(stmt);
        }
    }
    
    // Test with NULL statement (should be handled gracefully)
    sqlite3_column_name(NULL, 0);
    sqlite3_column_origin_name(NULL, 0);
    sqlite3_column_database_name(NULL, 0);
    sqlite3_column_table_name(NULL, 0);
    sqlite3_column_type(NULL, 0);
    sqlite3_column_decltype(NULL, 0);
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
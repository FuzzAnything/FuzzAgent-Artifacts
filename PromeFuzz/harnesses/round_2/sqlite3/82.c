// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void create_test_database(sqlite3 **db) {
    if (sqlite3_open(":memory:", db) != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    const char *create_table_sql = 
        "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);"
        "CREATE TABLE another_table (col1 INTEGER, col2 TEXT);"
        "INSERT INTO test_table (name, value) VALUES ('test1', 1.5), ('test2', 2.7);"
        "INSERT INTO another_table (col1, col2) VALUES (100, 'hello'), (200, 'world');";
    
    char *err_msg = NULL;
    sqlite3_exec(*db, create_table_sql, NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // Create in-memory database with test tables
    create_test_database(&db);
    if (!db) {
        return 0;
    }
    
    // Use fuzzer input as SQL query (ensure null termination)
    char *sql = NULL;
    if (Size > 0) {
        sql = (char *)malloc(Size + 1);
        if (!sql) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql, Data, Size);
        sql[Size] = '\0';
    } else {
        sql = strdup("SELECT * FROM test_table;");
    }
    
    // Test sqlite3_prepare (legacy version)
    int rc = sqlite3_prepare(db, sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        // Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            
            // Test each column
            for (int i = 0; i < col_count; i++) {
                // Test sqlite3_column_table_name
                const char *table_name = sqlite3_column_table_name(stmt, i);
                
                // Test sqlite3_column_type
                int col_type = sqlite3_column_type(stmt, i);
                
                // Test sqlite3_column_name
                const char *col_name = sqlite3_column_name(stmt, i);
                
                // Use the results to avoid optimization
                (void)table_name;
                (void)col_type;
                (void)col_name;
            }
            
            // Test sqlite3_column_count again
            int count = sqlite3_column_count(stmt);
            (void)count;
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Test sqlite3_prepare_v3 with different flags
    unsigned int prep_flags = 0;
    if (Size > 0) {
        // Use first byte to determine flags (if available)
        prep_flags = (Size >= 1) ? (Data[0] & 0x0F) : 0;
    }
    
    rc = sqlite3_prepare_v3(db, sql, -1, prep_flags, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        // Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            
            for (int i = 0; i < col_count; i++) {
                // Test all target functions again
                const char *table_name = sqlite3_column_table_name(stmt, i);
                int col_type = sqlite3_column_type(stmt, i);
                const char *col_name = sqlite3_column_name(stmt, i);
                
                (void)table_name;
                (void)col_type;
                (void)col_name;
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Try with different SQL statements based on fuzzer input
    if (Size > 10) {
        // Create a statement that joins tables to test table_name function
        const char *join_sql = "SELECT test_table.id, test_table.name, another_table.col2 "
                               "FROM test_table JOIN another_table ON test_table.id = another_table.col1";
        rc = sqlite3_prepare_v3(db, join_sql, -1, 0, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int col_count = sqlite3_column_count(stmt);
                for (int i = 0; i < col_count; i++) {
                    const char *table_name = sqlite3_column_table_name(stmt, i);
                    int col_type = sqlite3_column_type(stmt, i);
                    const char *col_name = sqlite3_column_name(stmt, i);
                    
                    (void)table_name;
                    (void)col_type;
                    (void)col_name;
                }
            }
            sqlite3_finalize(stmt);
        }
    }
    
    // Cleanup
    free(sql);
    sqlite3_close(db);
    
    return 0;
}
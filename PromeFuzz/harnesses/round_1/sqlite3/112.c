// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
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

static void setup_database(sqlite3** db) {
    if (sqlite3_open(":memory:", db) != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    const char* create_table_sql = 
        "CREATE TABLE test_table (id INTEGER, name TEXT, value REAL);"
        "CREATE TABLE other_table (data BLOB);"
        "ATTACH DATABASE ':memory:' AS attached_db;"
        "CREATE TABLE attached_db.attached_table (x INTEGER);";
    
    char* err_msg = NULL;
    if (sqlite3_exec(*db, create_table_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(*db);
        *db = NULL;
    }
}

static void cleanup_database(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
    }
}

static sqlite3_stmt* prepare_statement(sqlite3* db, const uint8_t* data, size_t size) {
    if (!db || size == 0) return NULL;
    
    const char* base_queries[] = {
        "SELECT id, name FROM test_table",
        "SELECT * FROM test_table",
        "SELECT test_table.id, other_table.data FROM test_table, other_table",
        "SELECT id AS alias_id, name AS alias_name FROM test_table",
        "SELECT 1+2, id FROM test_table",
        "SELECT attached_table.x FROM attached_table",
        "SELECT COUNT(*), MAX(id) FROM test_table"
    };
    
    size_t query_idx = data[0] % (sizeof(base_queries) / sizeof(base_queries[0]));
    const char* query = base_queries[query_idx];
    
    sqlite3_stmt* stmt = NULL;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }
    
    return stmt;
}

static void test_column_functions(sqlite3_stmt* stmt) {
    if (!stmt) return;
    
    int col_count = sqlite3_column_count(stmt);
    if (col_count <= 0) return;
    
    for (int i = 0; i < col_count; i++) {
        const char* decltype = sqlite3_column_decltype(stmt, i);
        const char* name = sqlite3_column_name(stmt, i);
        const char* db_name = sqlite3_column_database_name(stmt, i);
        const char* table_name = sqlite3_column_table_name(stmt, i);
        const char* origin_name = sqlite3_column_origin_name(stmt, i);
        
        if (decltype) {
            volatile char c = decltype[0];
            (void)c;
        }
        if (name) {
            volatile char c = name[0];
            (void)c;
        }
        if (db_name) {
            volatile char c = db_name[0];
            (void)c;
        }
        if (table_name) {
            volatile char c = table_name[0];
            (void)c;
        }
        if (origin_name) {
            volatile char c = origin_name[0];
            (void)c;
        }
    }
    
    int step_result = sqlite3_step(stmt);
    if (step_result == SQLITE_ROW) {
        for (int i = 0; i < col_count; i++) {
            int col_type = sqlite3_column_type(stmt, i);
            volatile int t = col_type;
            (void)t;
            
            const char* decltype = sqlite3_column_decltype(stmt, i);
            const char* name = sqlite3_column_name(stmt, i);
            const char* db_name = sqlite3_column_database_name(stmt, i);
            const char* table_name = sqlite3_column_table_name(stmt, i);
            const char* origin_name = sqlite3_column_origin_name(stmt, i);
            
            if (decltype) {
                volatile char c = decltype[0];
                (void)c;
            }
            if (name) {
                volatile char c = name[0];
                (void)c;
            }
            if (db_name) {
                volatile char c = db_name[0];
                (void)c;
            }
            if (table_name) {
                volatile char c = table_name[0];
                (void)c;
            }
            if (origin_name) {
                volatile char c = origin_name[0];
                (void)c;
            }
        }
    }
    
    sqlite3_reset(stmt);
    
    for (int i = 0; i < col_count; i++) {
        const char* decltype = sqlite3_column_decltype(stmt, i);
        const char* name = sqlite3_column_name(stmt, i);
        const char* db_name = sqlite3_column_database_name(stmt, i);
        const char* table_name = sqlite3_column_table_name(stmt, i);
        const char* origin_name = sqlite3_column_origin_name(stmt, i);
        
        if (decltype) {
            volatile char c = decltype[0];
            (void)c;
        }
        if (name) {
            volatile char c = name[0];
            (void)c;
        }
        if (db_name) {
            volatile char c = db_name[0];
            (void)c;
        }
        if (table_name) {
            volatile char c = table_name[0];
            (void)c;
        }
        if (origin_name) {
            volatile char c = origin_name[0];
            (void)c;
        }
    }
}

int LLVMFuzzerTestOneInput_112(const uint8_t* data, size_t size) {
    static int initialized = 0;
    static const uint8_t dummy_data[] = {0};
    
    if (size == 0) {
        data = dummy_data;
        size = sizeof(dummy_data);
    }
    
    sqlite3* db = NULL;
    setup_database(&db);
    if (!db) return 0;
    
    sqlite3_stmt* stmt = prepare_statement(db, data, size);
    if (stmt) {
        test_column_functions(stmt);
        sqlite3_finalize(stmt);
    }
    
    cleanup_database(db);
    return 0;
}
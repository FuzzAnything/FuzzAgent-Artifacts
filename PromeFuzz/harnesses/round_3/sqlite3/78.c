// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:80388:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int prepare_test_statement(sqlite3 *db, sqlite3_stmt **stmt, const uint8_t *data, size_t size) {
    const char *sql_queries[] = {
        "SELECT ?, ?, ?",
        "SELECT :name, $value, @param",
        "SELECT ?1, ?2, ?5",
        "UPDATE test SET col = ?",
        "SELECT * FROM test",
        "SELECT col1, col2 AS alias FROM test WHERE id = ?"
    };
    
    if (size == 0) return SQLITE_ERROR;
    
    int query_idx = data[0] % (sizeof(sql_queries) / sizeof(sql_queries[0]));
    const char *sql = sql_queries[query_idx];
    
    return sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        sqlite3_exec(db, "CREATE TABLE test(id INTEGER, col1 TEXT, col2 BLOB)", NULL, NULL, NULL);
        initialized = 1;
    }
    
    sqlite3_stmt *stmt = NULL;
    
    if (prepare_test_statement(db, &stmt, Data, Size) != SQLITE_OK) {
        return 0;
    }
    
    if (stmt == NULL) {
        return 0;
    }
    
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    if (param_count > 0 && Size > 1) {
        for (int i = 0; i < param_count && i < 10; i++) {
            size_t offset = (i + 1) % Size;
            int type = Data[offset] % 5;
            
            switch (type) {
                case 0:
                    sqlite3_bind_int(stmt, i + 1, (int)Data[offset]);
                    break;
                case 1:
                    sqlite3_bind_text(stmt, i + 1, (const char*)Data, 
                                     Size > 100 ? 100 : (int)Size, SQLITE_TRANSIENT);
                    break;
                case 2:
                    sqlite3_bind_blob(stmt, i + 1, Data, 
                                     Size > 100 ? 100 : (int)Size, SQLITE_TRANSIENT);
                    break;
                case 3:
                    sqlite3_bind_null(stmt, i + 1);
                    break;
                case 4:
                    sqlite3_bind_double(stmt, i + 1, (double)Data[offset]);
                    break;
            }
        }
    }
    
    int col_count = sqlite3_column_count(stmt);
    
    for (int i = 0; i < col_count && i < 10; i++) {
        const char *col_name = sqlite3_column_name(stmt, i);
        (void)col_name;
    }
    
    const char *normalized_sql = sqlite3_normalized_sql(stmt);
    (void)normalized_sql;
    
    const char *original_sql = sqlite3_sql(stmt);
    (void)original_sql;
    
    char *expanded_sql = sqlite3_expanded_sql(stmt);
    if (expanded_sql != NULL) {
        sqlite3_free(expanded_sql);
    }
    
    sqlite3_finalize(stmt);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:80388:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:80388:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int prepare_statement(sqlite3 *db, const char *sql, sqlite3_stmt **stmt) {
    return sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
}

static void cleanup_statement(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    // Limit input size to avoid excessive memory usage
    if (Size > 1024) {
        return 0;
    }

    // Create null-terminated SQL string from fuzz input
    char *sql = (char *)malloc(Size + 1);
    if (!sql) {
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';

    // Open in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        free(sql);
        return 0;
    }

    // Prepare statement
    sqlite3_stmt *stmt = NULL;
    int prep_result = prepare_statement(db, sql, &stmt);
    
    // Only test the API functions if statement preparation was successful
    if (prep_result == SQLITE_OK && stmt != NULL) {
        // 1. Test sqlite3_bind_parameter_count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // 2. Test sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        
        // 3. Test sqlite3_column_name for each column (if any)
        for (int i = 0; i < col_count; i++) {
            const char *col_name = sqlite3_column_name(stmt, i);
            // col_name may be NULL on memory error, but we don't need to handle it
            (void)col_name;
        }
        
        // 4. Test sqlite3_normalized_sql
        const char *normalized = sqlite3_normalized_sql(stmt);
        (void)normalized;
        
        // 5. Test sqlite3_sql
        const char *original_sql = sqlite3_sql(stmt);
        (void)original_sql;
        
        // 6. Test sqlite3_expanded_sql - caller must free the result
        char *expanded = sqlite3_expanded_sql(stmt);
        if (expanded) {
            sqlite3_free(expanded);
        }
        
        // Try to explore more states by binding some parameters
        if (param_count > 0) {
            // Bind at most 5 parameters to avoid excessive loops
            int bind_limit = param_count > 5 ? 5 : param_count;
            for (int i = 1; i <= bind_limit; i++) {
                // Use deterministic values from fuzz input for binding
                int value = i % 3;
                switch (value) {
                    case 0:
                        sqlite3_bind_null(stmt, i);
                        break;
                    case 1:
                        sqlite3_bind_int(stmt, i, i * 100);
                        break;
                    case 2:
                        sqlite3_bind_text(stmt, i, "fuzz", 4, SQLITE_TRANSIENT);
                        break;
                }
            }
            
            // After binding, test the functions again
            param_count = sqlite3_bind_parameter_count(stmt);
            col_count = sqlite3_column_count(stmt);
            
            // Test sqlite3_expanded_sql again after binding
            expanded = sqlite3_expanded_sql(stmt);
            if (expanded) {
                sqlite3_free(expanded);
            }
        }
    }
    
    // Cleanup
    cleanup_statement(stmt);
    cleanup_db(db);
    free(sql);
    
    return 0;
}
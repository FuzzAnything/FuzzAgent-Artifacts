// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:173641:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:173641:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_busy_handler(void *data, int count) {
    return 0;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc;
    const char *col_name = NULL;
    sqlite3 *db_handle = NULL;
    int total_changes = 0;
    int error_offset = -1;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    const char *insert_sql = "INSERT INTO test VALUES (1, 'test1'), (2, 'test2'), (3, 'test3');";
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    if (Size >= 2) {
        size_t utf16_size = Size + 2;
        void *zSql = malloc(utf16_size);
        if (!zSql) {
            sqlite3_close(db);
            return 0;
        }
        
        memcpy(zSql, Data, Size);
        if (Size >= utf16_size) {
            ((uint8_t*)zSql)[utf16_size-2] = 0;
            ((uint8_t*)zSql)[utf16_size-1] = 0;
        } else {
            ((uint8_t*)zSql)[Size] = 0;
            if (Size+1 < utf16_size) ((uint8_t*)zSql)[Size+1] = 0;
        }
        
        rc = sqlite3_prepare16_v2(db, zSql, (int)Size, &stmt, &pzTail);
        
        free(zSql);
        
        if (rc != SQLITE_OK) {
            error_offset = sqlite3_error_offset(db);
        }
        
        if (stmt) {
            db_handle = sqlite3_db_handle(stmt);
            
            int col_count = sqlite3_column_count(stmt);
            if (col_count > 0) {
                int col_index = (Data[0] % (col_count + 3)) - 1;
                col_name = sqlite3_column_name(stmt, col_index);
                
                col_name = sqlite3_column_name(stmt, 0);
                if (col_count > 1) {
                    col_name = sqlite3_column_name(stmt, 1);
                }
                if (col_count > 2) {
                    col_name = sqlite3_column_name(stmt, col_count - 1);
                }
            }
            
            rc = sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    total_changes = sqlite3_total_changes(db);
    
    if (Size > 0) {
        const char *select_sql = "SELECT * FROM test;";
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int col_count = sqlite3_column_count(stmt);
                for (int i = 0; i < col_count; i++) {
                    col_name = sqlite3_column_name(stmt, i);
                }
            }
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    error_offset = sqlite3_error_offset(db);
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    sqlite3_finalize(NULL);
    
    sqlite3_close(db);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
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
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_busy_handler(void *data, int count) {
    return 1;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a, b, c);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    rc = sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2, 3);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    size_t utf16_size = Size * 2;
    uint8_t *utf16_data = (uint8_t*)malloc(utf16_size + 2);
    if (!utf16_data) {
        sqlite3_close(db);
        return 0;
    }
    
    for (size_t i = 0; i < Size; i++) {
        utf16_data[i*2] = Data[i];
        utf16_data[i*2 + 1] = 0;
    }
    
    if (utf16_size >= 2) {
        utf16_data[utf16_size] = 0;
        utf16_data[utf16_size + 1] = 0;
    }
    
    rc = sqlite3_prepare16_v2(db, utf16_data, (int)(utf16_size), &stmt, &pzTail);
    
    if (rc != SQLITE_OK) {
        int offset = sqlite3_error_offset(db);
        (void)offset;
    }
    
    free(utf16_data);
    
    sqlite3_stmt *valid_stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT a, b, c FROM t1;", -1, &valid_stmt, NULL);
    
    if (rc == SQLITE_OK && valid_stmt != NULL) {
        rc = sqlite3_step(valid_stmt);
        if (rc == SQLITE_ROW) {
            for (int i = 0; i < sqlite3_column_count(valid_stmt); i++) {
                const char *col_name = sqlite3_column_name(valid_stmt, i);
                (void)col_name;
            }
            
            if (Size > 0) {
                int col_index = Data[0] % 10;
                const char *col_name = sqlite3_column_name(valid_stmt, col_index);
                (void)col_name;
            }
        }
        
        sqlite3 *db_from_stmt = sqlite3_db_handle(valid_stmt);
        (void)db_from_stmt;
        
        sqlite3 *null_db = sqlite3_db_handle(NULL);
        (void)null_db;
        
        sqlite3_finalize(valid_stmt);
    }
    
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;
    
    if (stmt != NULL) {
        rc = sqlite3_finalize(stmt);
        (void)rc;
    }
    
    sqlite3_finalize(NULL);
    
    if (Size > 0) {
        uint8_t op = Data[0];
        
        if (op % 3 == 0) {
            sqlite3_exec(db, "INSERT INTO t1 VALUES(4, 5, 6);", NULL, NULL, NULL);
        } else if (op % 3 == 1) {
            sqlite3_exec(db, "UPDATE t1 SET a = 10 WHERE a = 1;", NULL, NULL, NULL);
        } else {
            sqlite3_exec(db, "DELETE FROM t1 WHERE a = 1;", NULL, NULL, NULL);
        }
        
        total_changes = sqlite3_total_changes(db);
        (void)total_changes;
    }
    
    sqlite3_close(db);
    
    return 0;
}
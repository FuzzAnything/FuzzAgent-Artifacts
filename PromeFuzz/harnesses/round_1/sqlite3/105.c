// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:173641:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

static int dummy_busy_handler(void *data, int count) {
    return 0;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);"
        "INSERT OR IGNORE INTO test_table VALUES (1, 'test1'), (2, 'test2');";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    
    size_t utf16_size = Size;
    if (utf16_size % 2 != 0) {
        utf16_size++;
    }
    
    uint16_t *utf16_data = (uint16_t *)malloc(utf16_size + 2);
    if (utf16_data == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    size_t copy_size = Size > utf16_size ? utf16_size : Size;
    memcpy(utf16_data, Data, copy_size);
    utf16_data[utf16_size/2] = 0;
    
    rc = sqlite3_prepare16_v2(db, utf16_data, utf16_size, &stmt, &pzTail);
    
    if (rc != SQLITE_OK) {
        int error_offset = sqlite3_error_offset(db);
        (void)error_offset;
    }
    
    if (stmt != NULL) {
        sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
        (void)db_from_stmt;
        
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count && i < 10; i++) {
                const char *col_name = sqlite3_column_name(stmt, i);
                (void)col_name;
            }
        }
        
        sqlite3_reset(stmt);
        
        const uint16_t select_sql[] = { 'S', 'E', 'L', 'E', 'C', 'T', ' ', '*', ' ', 'F', 'R', 'O', 'M', ' ', 't', 'e', 's', 't', '_', 't', 'a', 'b', 'l', 'e', ';', 0 };
        sqlite3_stmt *stmt2 = NULL;
        rc = sqlite3_prepare16_v2(db, select_sql, sizeof(select_sql), &stmt2, NULL);
        if (rc == SQLITE_OK && stmt2 != NULL) {
            while (sqlite3_step(stmt2) == SQLITE_ROW) {
                int col_count = sqlite3_column_count(stmt2);
                for (int i = 0; i < col_count; i++) {
                    const char *col_name = sqlite3_column_name(stmt2, i);
                    (void)col_name;
                }
            }
            sqlite3_finalize(stmt2);
        }
        
        rc = sqlite3_finalize(stmt);
        (void)rc;
    }
    
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;
    
    const char *insert_sql = "INSERT OR IGNORE INTO test_table VALUES (3, 'test3');";
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    
    total_changes = sqlite3_total_changes(db);
    (void)total_changes;
    
    free(utf16_data);
    
    sqlite3_finalize(NULL);
    
    sqlite3_close(db);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
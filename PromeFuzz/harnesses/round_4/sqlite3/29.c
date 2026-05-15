// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    sqlite3_blob *blob = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Create a simple table for blob operations
    const char *create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    if (sqlite3_exec(db, create_table, NULL, NULL, &errmsg) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Insert a dummy row for blob operations
    const char *insert_row = "INSERT OR IGNORE INTO test (id, data) VALUES (1, x'01020304');";
    if (sqlite3_exec(db, insert_row, NULL, NULL, &errmsg) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Prepare a dummy statement for sqlite3_finalize
    const char *select_sql = "SELECT * FROM test;";
    sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    
    // 1. sqlite3_finalize
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    // Use fuzz data for SQL command if we have enough data
    char *sql_cmd = NULL;
    if (Size > 0) {
        sql_cmd = (char*)malloc(Size + 1);
        if (sql_cmd) {
            memcpy(sql_cmd, Data, Size);
            sql_cmd[Size] = '\0';
            
            // 2. sqlite3_exec
            sqlite3_exec(db, sql_cmd, dummy_callback, NULL, &errmsg);
            
            free(sql_cmd);
        }
    } else {
        // Use a dummy SQL command if no data
        sqlite3_exec(db, "SELECT 1;", dummy_callback, NULL, &errmsg);
    }
    
    // 3. First sqlite3_mprintf
    // Use fuzz data for format string if available
    if (Size > 0) {
        char format[256];
        size_t format_len = Size < 255 ? Size : 255;
        memcpy(format, Data, format_len);
        format[format_len] = '\0';
        str1 = sqlite3_mprintf("%s", format);
    } else {
        str1 = sqlite3_mprintf("test string %d", 123);
    }
    
    // 4. Second sqlite3_mprintf
    str2 = sqlite3_mprintf("another string %s", "test");
    
    // 5. First sqlite3_free
    sqlite3_free(str1);
    
    // 6. Second sqlite3_free
    sqlite3_free(str2);
    
    // 7. sqlite3_blob_open
    // Try to open blob on the row we inserted earlier
    sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob);
    
cleanup:
    if (blob) {
        sqlite3_blob_close(blob);
    }
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}
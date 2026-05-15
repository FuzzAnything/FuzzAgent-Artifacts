// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_realloc at sqlite3.c:17642:18 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Initialize sqlite3 database and statement
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table and insert some data to have something to query
    const char *create_table = "CREATE TABLE test (id INTEGER, name TEXT);";
    char *err_msg = NULL;
    if (sqlite3_exec(db, create_table, NULL, NULL, &err_msg) != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Insert a row
    const char *insert_data = "INSERT INTO test VALUES (1, 'test');";
    if (sqlite3_exec(db, insert_data, NULL, NULL, &err_msg) != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a SELECT statement
    const char *select_sql = "SELECT * FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // 1. sqlite3_realloc
    // Use first 4 bytes of input as initial size if available
    int realloc_size = (Size >= 4) ? *(int*)Data : 100;
    void *ptr = sqlite3_realloc(NULL, realloc_size);
    
    // 2. sqlite3_step
    int step_result = sqlite3_step(stmt);
    
    // 3. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    
    // 4. sqlite3_mprintf
    // Use input data for format string if available
    const char *format_str = (Size > 0) ? (const char*)Data : "%s";
    // Create a safe copy of the format string to ensure null termination
    char *safe_format = NULL;
    if (Size > 0) {
        safe_format = (char*)malloc(Size + 1);
        if (safe_format) {
            memcpy(safe_format, Data, Size);
            safe_format[Size] = '\0';
            format_str = safe_format;
        }
    }
    
    char *mprintf_result = sqlite3_mprintf(format_str, "test");
    if (safe_format) free(safe_format);
    
    // 5. sqlite3_column_text
    const unsigned char *column_text = NULL;
    if (col_count > 0 && step_result == SQLITE_ROW) {
        column_text = sqlite3_column_text(stmt, 0);
    }
    
    // 6. sqlite3_free
    sqlite3_free(mprintf_result);
    sqlite3_free(ptr);
    
    // 7. sqlite3_reset
    int reset_result = sqlite3_reset(stmt);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
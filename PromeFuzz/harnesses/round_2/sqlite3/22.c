// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Initialize database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Create dynamic string object
    sqlite3_str *str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer data for string operations
    // Ensure we have at least some data
    if (Size == 0) {
        sqlite3_str_finish(str);
        sqlite3_close(db);
        return 0;
    }
    
    // Create null-terminated strings from fuzzer data
    size_t copy_size = Size > 1024 ? 1024 : Size;
    char *format_str = (char *)malloc(copy_size + 1);
    char *append_str = (char *)malloc(copy_size + 1);
    
    if (!format_str || !append_str) {
        free(format_str);
        free(append_str);
        sqlite3_str_finish(str);
        sqlite3_close(db);
        return 0;
    }
    
    memcpy(format_str, Data, copy_size);
    format_str[copy_size] = '\0';
    
    memcpy(append_str, Data, copy_size);
    append_str[copy_size] = '\0';
    
    // 1. sqlite3_str_appendf
    sqlite3_str_appendf(str, "%s", format_str);
    
    // 2. sqlite3_str_append
    int append_len = (int)(copy_size > 100 ? 100 : copy_size);
    sqlite3_str_append(str, append_str, append_len);
    
    // 3. sqlite3_str_append (second call)
    sqlite3_str_append(str, append_str, append_len);
    
    // Get the built string for SQL preparation
    char *sql = sqlite3_str_value(str);
    
    // 4. sqlite3_prepare_v2
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    if (sql != NULL) {
        sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    }
    
    // 5. sqlite3_str_value (call again as specified)
    sqlite3_str_value(str);
    
    // 6. sqlite3_finalize
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    free(format_str);
    free(append_str);
    sqlite3_str_finish(str);
    sqlite3_close(db);
    
    return 0;
}
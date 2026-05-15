// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_realloc at sqlite3.c:17642:18 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
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
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int prepare_test_statement(sqlite3 *db, sqlite3_stmt **stmt) {
    const char *sql = "SELECT 1, 'test', 3.14;";
    return sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Initialize database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Prepare a statement
    if (prepare_test_statement(db, &stmt) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // 1. sqlite3_realloc
    // Use first byte for realloc size (mod 1024 to avoid huge allocations)
    int realloc_size = (Size > 0) ? (Data[0] % 1024) : 64;
    void *ptr = sqlite3_realloc(NULL, realloc_size);
    if (ptr && realloc_size > 0) {
        // Initialize with some data if we have fuzzer input
        size_t copy_size = (Size < (size_t)realloc_size) ? Size : (size_t)realloc_size;
        if (copy_size > 0) {
            memcpy(ptr, Data, copy_size);
        }
    }
    
    // 2. sqlite3_step
    int step_result = sqlite3_step(stmt);
    
    // 3. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    
    // 4. sqlite3_mprintf
    // Use fuzzer data for format string (ensure null termination)
    char *format_str = NULL;
    if (Size > 0) {
        size_t str_len = (Size < 256) ? Size : 256;
        char *temp_buf = (char *)malloc(str_len + 1);
        if (temp_buf) {
            memcpy(temp_buf, Data, str_len);
            temp_buf[str_len] = '\0';
            format_str = sqlite3_mprintf("%s", temp_buf);
            free(temp_buf);
        }
    } else {
        format_str = sqlite3_mprintf("default");
    }
    
    // 5. sqlite3_column_text
    const unsigned char *col_text = NULL;
    if (step_result == SQLITE_ROW && col_count > 0) {
        col_text = sqlite3_column_text(stmt, 0);
    }
    
    // 6. sqlite3_free
    if (format_str) {
        sqlite3_free(format_str);
    }
    if (ptr) {
        sqlite3_free(ptr);
    }
    
    // 7. sqlite3_reset
    int reset_result = sqlite3_reset(stmt);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    // Use results to avoid compiler optimizations
    (void)step_result;
    (void)col_count;
    (void)col_text;
    (void)reset_result;
    
    return 0;
}
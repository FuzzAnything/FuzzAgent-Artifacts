// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_realloc at sqlite3.c:17642:18 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
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

static int dummy_busy_handler(void *ptr, int count) {
    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql;
    void *realloc_ptr = NULL;
    char *mprintf_str = NULL;
    const unsigned char *column_text = NULL;
    
    // Initialize database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set a busy handler to avoid SQLITE_BUSY errors
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    // Create a simple table with some data
    rc = sqlite3_exec(db, 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT);"
        "INSERT INTO test (name) VALUES ('test1');"
        "INSERT INTO test (name) VALUES ('test2');"
        "INSERT INTO test (name) VALUES ('test3');", 
        NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement
    sql = "SELECT * FROM test WHERE name LIKE ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind a parameter if we have data
    if (Size > 0) {
        // Use first byte as parameter (modulo to avoid invalid UTF-8)
        char param[2] = {(char)(Data[0] % 128), 0};
        sqlite3_bind_text(stmt, 1, param, -1, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_text(stmt, 1, "%", -1, SQLITE_TRANSIENT);
    }
    
    // 1. sqlite3_realloc
    // Use fuzzer data to determine reallocation size (limit to reasonable size)
    size_t realloc_size = Size % 1024;  // Limit to 1KB
    realloc_ptr = sqlite3_realloc(NULL, (int)realloc_size);
    
    // 2. sqlite3_step
    rc = sqlite3_step(stmt);
    
    // 3. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    
    // 4. sqlite3_mprintf
    // Create a formatted string using fuzzer data
    if (Size > 0) {
        // Use up to 64 bytes from fuzzer input for formatting
        size_t fmt_len = Size > 64 ? 64 : Size;
        char fmt_buf[65];
        memcpy(fmt_buf, Data, fmt_len);
        fmt_buf[fmt_len] = 0;
        
        // Also create argument from fuzzer data
        char arg_buf[65];
        if (Size > fmt_len) {
            size_t arg_len = (Size - fmt_len) > 64 ? 64 : (Size - fmt_len);
            memcpy(arg_buf, Data + fmt_len, arg_len);
            arg_buf[arg_len] = 0;
        } else {
            strcpy(arg_buf, "fuzz");
        }
        
        mprintf_str = sqlite3_mprintf(fmt_buf, arg_buf);
    } else {
        mprintf_str = sqlite3_mprintf("Test %d", 123);
    }
    
    // 5. sqlite3_column_text
    if (rc == SQLITE_ROW && col_count > 0) {
        // Try to access columns with indices derived from fuzzer data
        for (int i = 0; i < col_count && i < 3; i++) {
            column_text = sqlite3_column_text(stmt, i);
            // Access the text to ensure it's read
            if (column_text) {
                volatile const unsigned char *dummy = column_text;
                (void)dummy;
            }
        }
    }
    
    // 6. sqlite3_free
    if (mprintf_str) {
        sqlite3_free(mprintf_str);
    }
    
    // Also free the reallocated memory if we allocated any
    if (realloc_ptr) {
        sqlite3_free(realloc_ptr);
    }
    
    // 7. sqlite3_reset
    rc = sqlite3_reset(stmt);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
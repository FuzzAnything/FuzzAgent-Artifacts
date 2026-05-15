// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:17642:18 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    char *err_msg = NULL;
    const char *tail;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple table
    rc = sqlite3_exec(db, "CREATE TABLE t1 (a TEXT, b INTEGER);", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Insert some data using the fuzzer input
    if (Size > 0) {
        char *insert_sql = sqlite3_mprintf("INSERT INTO t1 VALUES ('%.*s', %d);", 
                                          (int)(Size > 100 ? 100 : Size), Data, 
                                          (int)(Size % 100));
        if (insert_sql) {
            sqlite3_exec(db, insert_sql, NULL, NULL, &err_msg);
            sqlite3_free(insert_sql);
        }
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
    }
    
    // Prepare a statement
    const char *sql = "SELECT * FROM t1;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // 1. sqlite3_realloc
    void *mem_block = sqlite3_malloc(64);
    if (mem_block) {
        memset(mem_block, 0xAA, 64);
        void *realloced = sqlite3_realloc(mem_block, Size > 128 ? 128 : (Size ? (int)Size : 64));
        if (realloced) {
            mem_block = realloced;
        }
    }
    
    // 2. sqlite3_step
    rc = sqlite3_step(stmt);
    
    // 3. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    
    // 4. sqlite3_mprintf
    char *formatted = NULL;
    if (Size > 0) {
        formatted = sqlite3_mprintf("Data: %.*s, Cols: %d, Step: %d", 
                                   (int)(Size > 50 ? 50 : Size), Data, 
                                   col_count, rc);
    } else {
        formatted = sqlite3_mprintf("No data, Cols: %d, Step: %d", col_count, rc);
    }
    
    // 5. sqlite3_column_text
    const unsigned char *col_text = NULL;
    if (rc == SQLITE_ROW && col_count > 0) {
        col_text = sqlite3_column_text(stmt, 0);
        if (col_count > 1) {
            // Access second column if it exists
            (void)sqlite3_column_text(stmt, 1);
        }
    }
    
    // 6. sqlite3_free
    if (formatted) {
        sqlite3_free(formatted);
    }
    if (mem_block) {
        sqlite3_free(mem_block);
    }
    
    // 7. sqlite3_reset
    sqlite3_reset(stmt);
    
    // Additional step to explore more states
    if (rc == SQLITE_ROW) {
        // Step through remaining rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Do nothing, just step through
        }
        sqlite3_reset(stmt);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
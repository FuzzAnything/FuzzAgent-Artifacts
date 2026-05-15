// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static int dummy_callback(void *data, int argc, char **argv, char **col_names) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    char *errmsg = NULL;
    const char *error_str = NULL;
    int rc;
    
    // Limit input size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }
    
    // Open an in-memory database
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    // Test sqlite3_prepare_v2 with the fuzzed input as SQL
    if (Size > 0) {
        // Create null-terminated SQL string
        char *sql = (char *)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            // Test sqlite3_prepare_v2
            rc = sqlite3_prepare_v2(db, sql, (int)Size, &stmt, &tail);
            
            if (rc == SQLITE_OK && stmt) {
                // Test sqlite3_db_handle
                sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
                (void)db_from_stmt; // Use to avoid unused variable warning
                
                // Try to step through the statement if it's a SELECT
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
            
            // Test sqlite3_exec with the same SQL
            rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            
            free(sql);
        }
    }
    
    // Test sqlite3_errmsg
    error_str = sqlite3_errmsg(db);
    (void)error_str; // Use to avoid unused variable warning
    
    // Test sqlite3_interrupt
    sqlite3_interrupt(db);
    
    // Try to prepare another simple statement after interrupt
    rc = sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}
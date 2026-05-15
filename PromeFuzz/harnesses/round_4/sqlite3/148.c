// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
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

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    char *errmsg = NULL;
    int rc;
    
    // Use a fixed filename for the database
    const char *filename = "./dummy_file";
    
    // Open database with various flags
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    if (Size > 0) {
        // Use first byte to influence flags for diversity
        flags |= (Data[0] & 0x07);
    }
    
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        // Even if open fails, we can still test some functions
        const char *err = sqlite3_errmsg(db);
        (void)err;
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    // Test sqlite3_errmsg
    const char *err = sqlite3_errmsg(db);
    (void)err;
    
    // Prepare SQL statement using fuzzer input as SQL text
    // Ensure null termination
    char *sql = malloc(Size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';
    
    // Test sqlite3_prepare_v2 with various nByte values
    int nByte = (int)Size;
    if (Size > 4) {
        // Use some bytes to influence nByte parameter
        nByte = (Data[1] << 8) | Data[2];
        if (nByte < 0) nByte = (int)Size;
        if (nByte > (int)Size) nByte = (int)Size;
    }
    
    rc = sqlite3_prepare_v2(db, sql, nByte, &stmt, &tail);
    
    // Test sqlite3_db_handle
    sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
    (void)db_from_stmt;
    
    // Test sqlite3_errmsg again after prepare
    err = sqlite3_errmsg(db);
    (void)err;
    
    // Test sqlite3_exec with the same SQL
    rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Test sqlite3_interrupt
    sqlite3_interrupt(db);
    
    // Clean up prepared statement if created
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Test sqlite3_errmsg one more time
    err = sqlite3_errmsg(db);
    (void)err;
    
    // Close database
    sqlite3_close(db);
    
    free(sql);
    
    return 0;
}
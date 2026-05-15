// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *err_msg = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table to work with
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement for INSERT
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // First sqlite3_reset call
    rc = sqlite3_reset(stmt);
    
    // sqlite3_bind_text - use fuzzer input as text to bind
    // Ensure we have at least 1 byte for null-termination
    char *text_to_bind = NULL;
    if (Size > 0) {
        text_to_bind = (char *)malloc(Size + 1);
        if (text_to_bind) {
            memcpy(text_to_bind, Data, Size);
            text_to_bind[Size] = '\0';
            rc = sqlite3_bind_text(stmt, 1, text_to_bind, -1, SQLITE_TRANSIENT);
            free(text_to_bind);
        } else {
            // If allocation fails, bind empty string
            rc = sqlite3_bind_text(stmt, 1, "", 0, SQLITE_STATIC);
        }
    } else {
        // Empty input
        rc = sqlite3_bind_text(stmt, 1, "", 0, SQLITE_STATIC);
    }
    
    // sqlite3_step
    rc = sqlite3_step(stmt);
    
    // sqlite3_errmsg
    err_msg = sqlite3_errmsg(db);
    
    // Second sqlite3_reset call
    rc = sqlite3_reset(stmt);
    
    // sqlite3_changes
    int changes = sqlite3_changes(db);
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
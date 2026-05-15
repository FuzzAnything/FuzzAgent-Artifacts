// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
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
    int changes = 0;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table for testing
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement for INSERT
    const char *insert_sql = 
        "INSERT INTO test (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // First sqlite3_reset call
    rc = sqlite3_reset(stmt);
    
    // sqlite3_bind_text call
    // Use fuzzer input as the text to bind
    // If Size is 0, use empty string
    const char *bind_text = (Size > 0) ? (const char *)Data : "";
    int bind_size = (Size > 0) ? (int)Size : 0;
    // Use SQLITE_TRANSIENT so SQLite makes its own copy
    rc = sqlite3_bind_text(stmt, 1, bind_text, bind_size, SQLITE_TRANSIENT);
    
    // sqlite3_step call
    // Execute the prepared statement
    rc = sqlite3_step(stmt);
    
    // sqlite3_errmsg call
    // Get error message (if any)
    err_msg = sqlite3_errmsg(db);
    
    // Second sqlite3_reset call
    rc = sqlite3_reset(stmt);
    
    // sqlite3_changes call
    // Get number of rows changed by the last INSERT
    changes = sqlite3_changes(db);
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
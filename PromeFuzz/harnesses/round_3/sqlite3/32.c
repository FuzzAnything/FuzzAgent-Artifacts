// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
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

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Initialize database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table
    char *err_msg = NULL;
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);", 
                      NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement
    sqlite3_stmt *stmt = NULL;
    const char *sql = "INSERT INTO test (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // First sqlite3_reset
    rc = sqlite3_reset(stmt);
    
    // sqlite3_bind_text - use fuzzer input as text to bind
    // Ensure we don't overflow by limiting to Size
    const char *text_data = (const char*)Data;
    int text_len = (int)(Size > 0 ? Size : 0);
    if (text_len > 0) {
        rc = sqlite3_bind_text(stmt, 1, text_data, text_len, SQLITE_TRANSIENT);
    } else {
        // Bind empty string if no data
        rc = sqlite3_bind_text(stmt, 1, "", 0, SQLITE_STATIC);
    }
    
    // sqlite3_step
    rc = sqlite3_step(stmt);
    
    // sqlite3_errmsg
    const char *error_msg = sqlite3_errmsg(db);
    (void)error_msg; // Use variable to avoid unused warning
    
    // Second sqlite3_reset
    rc = sqlite3_reset(stmt);
    
    // sqlite3_changes
    int changes = sqlite3_changes(db);
    (void)changes; // Use variable to avoid unused warning
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
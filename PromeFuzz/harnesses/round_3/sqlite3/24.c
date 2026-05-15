// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:80267:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:80267:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *err_msg;
    
    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy table with BLOB column
    rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a SELECT statement
    rc = sqlite3_prepare_v2(db, "SELECT data FROM test;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // First step to get to SQLITE_ROW (though table is empty)
    rc = sqlite3_step(stmt);
    
    // Call functions in specified order
    // Note: Since table is empty, sqlite3_step returns SQLITE_DONE, not SQLITE_ROW
    // But we still call the functions to explore error paths
    (void)sqlite3_column_bytes(stmt, 0);
    (void)sqlite3_column_blob(stmt, 0);
    (void)sqlite3_column_bytes(stmt, 0);
    (void)sqlite3_column_blob(stmt, 0);
    (void)sqlite3_stmt_isexplain(stmt);
    (void)sqlite3_reset(stmt);
    (void)sqlite3_step(stmt);
    
    // Call sqlite3_errmsg twice
    err_msg = sqlite3_errmsg(db);
    (void)err_msg;
    err_msg = sqlite3_errmsg(db);
    (void)err_msg;
    
    // Finalize statement
    (void)sqlite3_finalize(stmt);
    
    // Now test with actual data
    // Insert some data using fuzzer input
    sqlite3_stmt *insert_stmt = NULL;
    rc = sqlite3_prepare_v2(db, "INSERT INTO test (id, data) VALUES (1, ?);", -1, &insert_stmt, NULL);
    if (rc == SQLITE_OK) {
        if (Size > 0) {
            sqlite3_bind_blob(insert_stmt, 1, Data, Size > 1024 ? 1024 : Size, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(insert_stmt, 1);
        }
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    // Prepare and execute SELECT again
    rc = sqlite3_prepare_v2(db, "SELECT data FROM test;", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // Call functions again with actual data
            (void)sqlite3_column_bytes(stmt, 0);
            (void)sqlite3_column_blob(stmt, 0);
            (void)sqlite3_column_bytes(stmt, 0);
            (void)sqlite3_column_blob(stmt, 0);
            (void)sqlite3_stmt_isexplain(stmt);
            (void)sqlite3_reset(stmt);
            (void)sqlite3_step(stmt);
            
            // Call sqlite3_errmsg twice more
            err_msg = sqlite3_errmsg(db);
            (void)err_msg;
            err_msg = sqlite3_errmsg(db);
            (void)err_msg;
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const char *errmsg = NULL;
    int rc;
    int iCol = 0;
    int bytes = 0;
    const void *blob = NULL;
    int explain_result = 0;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table
    rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER, data BLOB);", callback, 0, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some data using the fuzzer input
    char *insert_sql = "INSERT INTO test VALUES (1, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK) {
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1024 ? 1024 : Size), SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Prepare a SELECT statement
    const char *select_sql = "SELECT data FROM test WHERE id = 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step to get a row
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // First sqlite3_column_bytes
        bytes = sqlite3_column_bytes(stmt, iCol);
        
        // First sqlite3_column_blob
        blob = sqlite3_column_blob(stmt, iCol);
        
        // Second sqlite3_column_bytes
        bytes = sqlite3_column_bytes(stmt, iCol);
        
        // Second sqlite3_column_blob
        blob = sqlite3_column_blob(stmt, iCol);
        
        // sqlite3_stmt_isexplain
        explain_result = sqlite3_stmt_isexplain(stmt);
        
        // sqlite3_reset
        rc = sqlite3_reset(stmt);
        
        // sqlite3_step again
        rc = sqlite3_step(stmt);
        
        // First sqlite3_errmsg
        errmsg = sqlite3_errmsg(db);
        
        // Second sqlite3_errmsg
        errmsg = sqlite3_errmsg(db);
    }
    
    // sqlite3_finalize
    rc = sqlite3_finalize(stmt);
    
    // Clean up
    sqlite3_close(db);
    
    // Use the variables to avoid compiler warnings
    (void)bytes;
    (void)blob;
    (void)explain_result;
    (void)errmsg;
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static int prepare_test_statement(sqlite3 *db, sqlite3_stmt **stmt) {
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB); "
                      "INSERT OR IGNORE INTO test VALUES (1, x'01020304'); "
                      "SELECT data FROM test WHERE id = 1;";
    return sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const void *blob_ptr;
    int blob_size;
    const char *err_msg;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement that returns a BLOB column
    rc = prepare_test_statement(db, &stmt);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Execute the statement to get to a row
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // First sqlite3_column_bytes call
        blob_size = sqlite3_column_bytes(stmt, 0);
        
        // First sqlite3_column_blob call
        blob_ptr = sqlite3_column_blob(stmt, 0);
        
        // Second sqlite3_column_bytes call
        blob_size = sqlite3_column_bytes(stmt, 0);
        
        // Second sqlite3_column_blob call
        blob_ptr = sqlite3_column_blob(stmt, 0);
        
        // Use the fuzz data to potentially modify behavior
        if (Size > 0) {
            // Use first byte to decide column index (mod 2 for safety)
            int col_index = Data[0] % 2;
            sqlite3_column_bytes(stmt, col_index);
            sqlite3_column_blob(stmt, col_index);
        }
    }
    
    // sqlite3_stmt_isexplain
    int explain_type = sqlite3_stmt_isexplain(stmt);
    
    // sqlite3_reset
    rc = sqlite3_reset(stmt);
    
    // sqlite3_step again
    rc = sqlite3_step(stmt);
    
    // First sqlite3_errmsg call
    err_msg = sqlite3_errmsg(db);
    
    // Second sqlite3_errmsg call
    err_msg = sqlite3_errmsg(db);
    
    // sqlite3_finalize
    rc = sqlite3_finalize(stmt);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
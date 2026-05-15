// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:90967:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:90957:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:90950:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
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
#include <stdio.h>

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_result_int(ctx, 42);
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *errmsg;
    int rc;
    
    // Initialize database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table with a BLOB column
    rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert a row to get a valid rowid
    rc = sqlite3_exec(db, "INSERT INTO test (data) VALUES (zeroblob(1024))", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Open BLOB handle
    rc = sqlite3_blob_open(db, "main", "test", "data", sqlite3_last_insert_rowid(db), 1, &blob);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare buffer for blob operations
    size_t blob_size = sqlite3_blob_bytes(blob);
    uint8_t *buffer = (uint8_t*)malloc(blob_size);
    if (!buffer) {
        sqlite3_blob_close(blob);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer input for blob operations
    size_t write_size = Size > blob_size ? blob_size : Size;
    if (write_size > 0) {
        // sqlite3_blob_write
        rc = sqlite3_blob_write(blob, Data, write_size, 0);
        
        // sqlite3_errmsg
        errmsg = sqlite3_errmsg(db);
        
        // sqlite3_blob_read
        rc = sqlite3_blob_read(blob, buffer, write_size, 0);
        
        // sqlite3_errmsg
        errmsg = sqlite3_errmsg(db);
    }
    
    // sqlite3_create_function
    rc = sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8 | SQLITE_DETERMINISTIC, 
                                 NULL, dummy_func, NULL, NULL);
    
    // sqlite3_prepare_v2
    const char *sql = "SELECT dummy_func(5)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    free(buffer);
    sqlite3_blob_close(blob);
    sqlite3_close(db);
    
    return 0;
}
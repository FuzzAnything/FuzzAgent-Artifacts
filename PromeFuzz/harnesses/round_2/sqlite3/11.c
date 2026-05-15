// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:90957:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:90950:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
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
    // Dummy function for sqlite3_create_function
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Initialize database
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    const char *err_msg = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table with BLOB column for blob operations
    rc = sqlite3_exec(db, 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test (data) VALUES (zeroblob(1024));", 
        NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Open BLOB handle
    rc = sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob);
    if (rc != SQLITE_OK || blob == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare buffer for blob operations
    size_t blob_size = Size > 1024 ? 1024 : Size;
    void *buffer = malloc(blob_size);
    if (!buffer && blob_size > 0) {
        sqlite3_blob_close(blob);
        sqlite3_close(db);
        return 0;
    }
    
    if (blob_size > 0 && Data) {
        memcpy(buffer, Data, blob_size);
    }
    
    // 1. sqlite3_blob_write
    if (blob_size > 0) {
        rc = sqlite3_blob_write(blob, buffer, (int)blob_size, 0);
        // Ignore return value - we just want to exercise the API
    }
    
    // 2. sqlite3_errmsg
    err_msg = sqlite3_errmsg(db);
    
    // 3. sqlite3_blob_read
    if (blob_size > 0) {
        rc = sqlite3_blob_read(blob, buffer, (int)blob_size, 0);
        // Ignore return value
    }
    
    // 4. sqlite3_errmsg
    err_msg = sqlite3_errmsg(db);
    
    // 5. sqlite3_create_function
    // Use fuzzed data for function name if available
    const char *func_name = "fuzz_func";
    if (Size > 0 && Data) {
        // Copy up to 255 bytes for function name (sqlite limit)
        size_t name_len = Size > 255 ? 255 : Size;
        char *temp_name = malloc(name_len + 1);
        if (temp_name) {
            memcpy(temp_name, Data, name_len);
            temp_name[name_len] = '\0';
            func_name = temp_name;
        }
    }
    
    rc = sqlite3_create_function(db, func_name, 1, SQLITE_UTF8 | SQLITE_DETERMINISTIC, 
                                  NULL, dummy_func, NULL, NULL);
    
    if (func_name != "fuzz_func") {
        free((void*)func_name);
    }
    
    // 6. sqlite3_prepare_v2
    // Use fuzzed data for SQL statement if available
    const char *sql = "SELECT 1;";
    if (Size > 0 && Data) {
        // Copy SQL statement from fuzzed data
        char *temp_sql = malloc(Size + 1);
        if (temp_sql) {
            memcpy(temp_sql, Data, Size);
            temp_sql[Size] = '\0';
            sql = temp_sql;
        }
    }
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (sql != "SELECT 1;") {
        free((void*)sql);
    }
    
    // Cleanup
    if (stmt) sqlite3_finalize(stmt);
    if (blob) sqlite3_blob_close(blob);
    free(buffer);
    sqlite3_close(db);
    
    return 0;
}
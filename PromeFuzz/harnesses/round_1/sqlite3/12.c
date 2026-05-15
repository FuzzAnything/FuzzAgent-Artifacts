// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_result_int(ctx, 0);
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Do nothing
}

static void dummy_final(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Create in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create table with BLOB column
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    if (sqlite3_exec(db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert a row with BLOB data
    const char *insert_sql = "INSERT INTO test (data) VALUES (zeroblob(1024));";
    if (sqlite3_exec(db, insert_sql, NULL, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Open BLOB handle for writing
    if (sqlite3_blob_open(db, "main", "test", "data", 1, 1, &blob) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // 1. sqlite3_blob_write
    size_t write_size = Size > 1024 ? 1024 : Size;
    int offset = Data[0] % 512; // Use first byte for offset
    int n = Data[1] % 512; // Use second byte for write size
    if (n > 0 && offset + n <= 1024) {
        sqlite3_blob_write(blob, Data, n, offset);
    }
    
    // 2. sqlite3_errmsg
    const char *errmsg1 = sqlite3_errmsg(db);
    (void)errmsg1; // Use variable to avoid unused warning
    
    // 3. sqlite3_blob_read
    char read_buf[1024];
    offset = Data[2] % 512; // Use third byte for offset
    n = Data[3] % 512; // Use fourth byte for read size
    if (n > 0 && offset + n <= 1024) {
        sqlite3_blob_read(blob, read_buf, n, offset);
    }
    
    // 4. sqlite3_errmsg
    const char *errmsg2 = sqlite3_errmsg(db);
    (void)errmsg2; // Use variable to avoid unused warning
    
    // 5. sqlite3_create_function
    // Use part of input for function name
    size_t name_len = Size > 100 ? 100 : Size;
    char *func_name = malloc(name_len + 1);
    if (func_name) {
        memcpy(func_name, Data, name_len);
        func_name[name_len] = '\0';
        
        int nArg = (int)(Data[0] % 8) - 1; // -1 to 6 arguments
        int eTextRep = SQLITE_UTF8 | ((Data[1] & 1) ? SQLITE_DETERMINISTIC : 0);
        
        sqlite3_create_function(db, func_name, nArg, eTextRep, NULL,
                               dummy_func, dummy_step, dummy_final);
        
        free(func_name);
    }
    
    // 6. sqlite3_prepare_v2
    // Use remaining input for SQL statement
    size_t sql_len = Size > 200 ? 200 : Size;
    char *sql = malloc(sql_len + 1);
    if (sql) {
        memcpy(sql, Data, sql_len);
        sql[sql_len] = '\0';
        
        const char *tail = NULL;
        sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
        
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        free(sql);
    }
    
    // Cleanup
    if (blob) sqlite3_blob_close(blob);
    if (db) sqlite3_close(db);
    
    return 0;
}
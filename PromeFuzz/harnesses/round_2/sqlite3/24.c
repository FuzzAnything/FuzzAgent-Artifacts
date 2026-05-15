// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *errmsg = NULL;
    int rc;
    int col_bytes;
    const void *blob_data;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    // Create a simple table and insert some data
    const char *create_table = 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test (data) VALUES (x'01020304');"
        "INSERT INTO test (data) VALUES (x'05060708');";
    
    rc = sqlite3_exec(db, create_table, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a SELECT statement
    const char *select_sql = "SELECT data FROM test ORDER BY id";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step to first row
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // First call to sqlite3_column_bytes
        col_bytes = sqlite3_column_bytes(stmt, 0);
        
        // First call to sqlite3_column_blob
        blob_data = sqlite3_column_blob(stmt, 0);
        
        // Second call to sqlite3_column_bytes
        col_bytes = sqlite3_column_bytes(stmt, 0);
        
        // Second call to sqlite3_column_blob
        blob_data = sqlite3_column_blob(stmt, 0);
        
        // Call sqlite3_stmt_isexplain
        int explain_type = sqlite3_stmt_isexplain(stmt);
        
        // Reset the statement
        rc = sqlite3_reset(stmt);
        
        // Step again
        rc = sqlite3_step(stmt);
        
        // First call to sqlite3_errmsg
        errmsg = sqlite3_errmsg(db);
        
        // Second call to sqlite3_errmsg
        errmsg = sqlite3_errmsg(db);
    }
    
    // Finalize the statement
    sqlite3_finalize(stmt);
    
    // Try to explore more states by using fuzzer data
    // Create a dummy file and write fuzzer data to it
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Try to open the dummy file as a database
    sqlite3 *db2 = NULL;
    rc = sqlite3_open("./dummy_file", &db2);
    if (rc == SQLITE_OK) {
        // Prepare another statement using fuzzer data
        sqlite3_stmt *stmt2 = NULL;
        const char *sql = "SELECT 1";
        rc = sqlite3_prepare_v2(db2, sql, -1, &stmt2, NULL);
        if (rc == SQLITE_OK && stmt2) {
            // Call the sequence again on this statement
            rc = sqlite3_step(stmt2);
            if (rc == SQLITE_ROW) {
                sqlite3_column_bytes(stmt2, 0);
                sqlite3_column_blob(stmt2, 0);
                sqlite3_column_bytes(stmt2, 0);
                sqlite3_column_blob(stmt2, 0);
                sqlite3_stmt_isexplain(stmt2);
                sqlite3_reset(stmt2);
                sqlite3_step(stmt2);
                sqlite3_errmsg(db2);
                sqlite3_errmsg(db2);
            }
            sqlite3_finalize(stmt2);
        }
        sqlite3_close(db2);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
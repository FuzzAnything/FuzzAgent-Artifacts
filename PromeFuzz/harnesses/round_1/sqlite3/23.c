// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static void dummy_file_write(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *errmsg = NULL;
    int rc;
    
    // Create dummy file with fuzzer data
    dummy_file_write(Data, Size);
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table and insert some data
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement to insert data
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind the fuzzer data as a BLOB
    sqlite3_bind_blob(stmt, 1, Data, Size > 1024 ? 1024 : Size, SQLITE_STATIC);
    
    // Execute the insert
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Finalize the insert statement
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    // Prepare a SELECT statement
    const char *select_sql = "SELECT data FROM test WHERE id = 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step to get a row
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Call the functions in the specified order
        int col_bytes1 = sqlite3_column_bytes(stmt, 0);
        const void *blob1 = sqlite3_column_blob(stmt, 0);
        int col_bytes2 = sqlite3_column_bytes(stmt, 0);
        const void *blob2 = sqlite3_column_blob(stmt, 0);
        int explain = sqlite3_stmt_isexplain(stmt);
        int reset_rc = sqlite3_reset(stmt);
        int step_rc = sqlite3_step(stmt);
        const char *errmsg1 = sqlite3_errmsg(db);
        const char *errmsg2 = sqlite3_errmsg(db);
        int finalize_rc = sqlite3_finalize(stmt);
        
        // Use the results to avoid compiler warnings
        (void)col_bytes1;
        (void)blob1;
        (void)col_bytes2;
        (void)blob2;
        (void)explain;
        (void)reset_rc;
        (void)step_rc;
        (void)errmsg1;
        (void)errmsg2;
        (void)finalize_rc;
    } else {
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    return 0;
}
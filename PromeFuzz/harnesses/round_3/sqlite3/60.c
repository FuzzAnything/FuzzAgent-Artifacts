// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:78471:25 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

static void test_udf(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        // Test sqlite3_value_bytes on protected value
        int bytes = sqlite3_value_bytes(argv[0]);
        (void)bytes;
        
        // Test sqlite3_value_int on protected value
        int int_val = sqlite3_value_int(argv[0]);
        (void)int_val;
        
        // Test sqlite3_value_int64 on protected value
        sqlite3_int64 int64_val = sqlite3_value_int64(argv[0]);
        (void)int64_val;
        
        // Test sqlite3_value_blob on protected value
        const void *blob = sqlite3_value_blob(argv[0]);
        (void)blob;
        
        // Test sqlite3_result_int64
        sqlite3_result_int64(ctx, int64_val);
        
        // Test sqlite3_result_value
        sqlite3_result_value(ctx, argv[0]);
    } else {
        sqlite3_result_int64(ctx, 0);
    }
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table
    char *err_msg = NULL;
    rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER, data BLOB);", 
                      dummy_callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare an INSERT statement
    rc = sqlite3_prepare_v2(db, "INSERT INTO test (id, data) VALUES (?, ?);", -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind parameters
    sqlite3_bind_int(stmt, 1, 1);
    sqlite3_bind_blob(stmt, 2, Data, (int)(Size > 100 ? 100 : Size), SQLITE_STATIC);
    
    // Execute the INSERT
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a user-defined function to test with protected values
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                            test_udf, NULL, NULL);
    
    // Call the user-defined function to test all APIs with protected values
    rc = sqlite3_prepare_v2(db, "SELECT test_func(data) FROM test WHERE id = 1;", 
                           -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return 0;
}
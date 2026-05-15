// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:78471:25 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void dummy_udf(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called to test the APIs
    if (argc < 1) return;
    
    sqlite3_value *value = argv[0];
    
    // Test sqlite3_value_blob
    const void *blob = sqlite3_value_blob(value);
    
    // Test sqlite3_value_bytes
    int bytes = sqlite3_value_bytes(value);
    
    // Test sqlite3_value_int64
    sqlite3_int64 int64_val = sqlite3_value_int64(value);
    
    // Test sqlite3_value_int
    int int_val = sqlite3_value_int(value);
    
    // Test sqlite3_result_int64
    sqlite3_result_int64(context, int64_val);
    
    // Test sqlite3_result_value
    sqlite3_result_value(context, value);
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy table
    rc = sqlite3_exec(db, "CREATE TABLE t1(x)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Register our test UDF
    rc = sqlite3_create_function(db, "dummy_udf", 1, SQLITE_UTF8, NULL, dummy_udf, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement that uses the UDF
    rc = sqlite3_prepare_v2(db, "SELECT dummy_udf(?)", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind the fuzzer input as a blob
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_STATIC);
    } else {
        sqlite3_bind_null(stmt, 1);
    }
    
    // Execute the statement to trigger the UDF
    rc = sqlite3_step(stmt);
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
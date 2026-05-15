// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // 1. sqlite3_value_int
    int int_result = sqlite3_value_int(argv[0]);
    
    // 2. sqlite3_value_bytes
    int bytes_result = sqlite3_value_bytes(argv[1]);
    
    // 3. sqlite3_result_value
    sqlite3_result_value(context, argv[0]);
    
    // 4. sqlite3_value_blob
    const void *blob_result = sqlite3_value_blob(argv[1]);
    
    // 5. sqlite3_malloc
    int alloc_size = bytes_result > 0 ? bytes_result : 64;
    if (alloc_size > 1024*1024) alloc_size = 1024*1024;
    void *allocated_mem = sqlite3_malloc(alloc_size);
    
    // 6. sqlite3_result_error
    const char *error_msg = "Fuzzer generated error";
    sqlite3_result_error(context, error_msg, -1);
    
    // 7. sqlite3_result_blob
    if (blob_result && bytes_result > 0) {
        sqlite3_result_blob(context, blob_result, 
                           bytes_result > 1000 ? 1000 : bytes_result, SQLITE_TRANSIENT);
    } else if (allocated_mem && alloc_size > 0) {
        memset(allocated_mem, 0xAA, alloc_size > 100 ? 100 : alloc_size);
        sqlite3_result_blob(context, allocated_mem, 
                           alloc_size > 100 ? 100 : alloc_size, SQLITE_TRANSIENT);
    }
    
    // 8. sqlite3_free
    if (allocated_mem) {
        sqlite3_free(allocated_mem);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Initialize SQLite
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple table and insert some data to get valid sqlite3_value objects
    sqlite3_exec(db, "CREATE TABLE t1(a INTEGER, b BLOB);", NULL, NULL, NULL);
    
    // Prepare a statement to get sqlite3_value objects
    sqlite3_stmt *stmt = NULL;
    const char *sql = "INSERT INTO t1 VALUES(?, ?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind values to get sqlite3_value-like objects
    if (Size > 0) {
        // Bind integer from first byte
        sqlite3_bind_int(stmt, 1, Data[0]);
        
        // Bind blob from remaining data
        if (Size > 1) {
            sqlite3_bind_blob(stmt, 2, Data + 1, Size - 1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_blob(stmt, 2, Data, 0, SQLITE_TRANSIENT);
        }
    } else {
        sqlite3_bind_int(stmt, 1, 0);
        sqlite3_bind_blob(stmt, 2, NULL, 0, SQLITE_TRANSIENT);
    }
    
    // Execute the statement to create the values
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    
    // Register the dummy function
    sqlite3_create_function(db, "dummy_func", 2, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);
    
    // Now call the function with the values from the table
    sqlite3_stmt *func_stmt = NULL;
    const char *func_sql = "SELECT dummy_func(a, b) FROM t1";
    
    if (sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, NULL) == SQLITE_OK) {
        // Execute to trigger the function calls
        sqlite3_step(func_stmt);
        sqlite3_finalize(func_stmt);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Empty destructor for testing
}

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1) return;
    
    sqlite3_value *val = argv[0];
    
    // 1. sqlite3_value_int
    int int_result = sqlite3_value_int(val);
    (void)int_result;
    
    // 2. sqlite3_value_bytes
    int bytes_result = sqlite3_value_bytes(val);
    (void)bytes_result;
    
    // 3. sqlite3_result_value
    sqlite3_result_value(ctx, val);
    
    // 4. sqlite3_value_blob
    const void *blob_result = sqlite3_value_blob(val);
    (void)blob_result;
    
    // 5. sqlite3_malloc
    int alloc_size = sqlite3_value_bytes(val) % 1024 + 1;
    void *allocated_mem = sqlite3_malloc(alloc_size);
    
    // 6. sqlite3_result_error
    const char *error_data = (const char*)sqlite3_value_blob(val);
    int error_len = sqlite3_value_bytes(val);
    if (error_len > 0) {
        sqlite3_result_error(ctx, error_data, error_len);
    } else {
        sqlite3_result_error(ctx, "fuzzer error", -1);
    }
    
    // 7. sqlite3_result_blob
    if (allocated_mem && error_len > 0) {
        size_t copy_size = error_len > 100 ? 100 : error_len;
        memcpy(allocated_mem, error_data, copy_size);
        sqlite3_result_blob(ctx, allocated_mem, (int)copy_size, SQLITE_TRANSIENT);
    } else {
        static const char static_blob[] = "static blob data";
        sqlite3_result_blob(ctx, static_blob, sizeof(static_blob) - 1, SQLITE_STATIC);
    }
    
    // 8. sqlite3_free - only free if we didn't use SQLITE_TRANSIENT
    // SQLITE_TRANSIENT already handles freeing, so we don't need to call sqlite3_free here
    // But we still need to call it for coverage, so we'll allocate and free separate memory
    void *temp_mem = sqlite3_malloc(16);
    if (temp_mem) {
        sqlite3_free(temp_mem);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    if (sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                                test_function, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    sqlite3_stmt *stmt;
    const char *sql = "SELECT test_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1000 ? 1000 : Size), SQLITE_STATIC);
        } else {
            sqlite3_bind_zeroblob(stmt, 1, 0);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void test_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1) return;
    
    const void *data = sqlite3_value_blob(argv[0]);
    int data_size = sqlite3_value_bytes(argv[0]);
    
    if (data_size <= 0) return;
    
    if (data_size > 0 && (data_size % 4 == 0)) {
        sqlite3_result_value(ctx, argv[0]);
        return;
    }
    
    if (data_size > 1) {
        void *auxdata = sqlite3_get_auxdata(ctx, 0);
        int dummy = 42;
        sqlite3_set_auxdata(ctx, 0, &dummy, NULL);
        auxdata = sqlite3_get_auxdata(ctx, 0);
    }
    
    if (data_size > 8 && (data_size % 8 == 0)) {
        sqlite3_uint64 n = 0;
        memcpy(&n, data, sizeof(sqlite3_uint64) > data_size ? data_size : sizeof(sqlite3_uint64));
        sqlite3_result_zeroblob64(ctx, n);
        return;
    }
    
    if (data_size > 0 && (data_size % 3 == 0)) {
        const char *error_msg = (const char*)data;
        int error_len = data_size > 100 ? 100 : data_size;
        sqlite3_result_error(ctx, error_msg, error_len);
        return;
    }
    
    if (data_size > 0 && (data_size % 5 == 0)) {
        int n = data_size > 1000 ? 1000 : data_size;
        sqlite3_result_zeroblob(ctx, n);
        return;
    }
    
    if (data_size > 0) {
        if ((*(const unsigned char*)data) % 2 == 0) {
            sqlite3_result_blob(ctx, data, data_size, SQLITE_TRANSIENT);
        } else {
            sqlite3_result_blob(ctx, data, data_size, SQLITE_STATIC);
        }
        return;
    }
    
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    if (sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                                test_func, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT test_func(?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    int bind_size = (int)(Size > 10000 ? 10000 : Size);
    if (sqlite3_bind_blob(stmt, 1, Data, bind_size, SQLITE_TRANSIENT) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}
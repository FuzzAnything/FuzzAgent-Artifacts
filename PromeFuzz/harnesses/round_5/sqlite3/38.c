// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"

static void dummy_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    if (argc > 0) {
        int bytes = sqlite3_value_bytes(argv[0]);
        const void *blob = sqlite3_value_blob(argv[0]);
        
        (void)bytes;
        (void)blob;
        
        sqlite3_result_int(ctx, 12345);
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    static sqlite3_stmt *stmt = NULL;
    
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        if (sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, dummy_function, NULL, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        const char *create_table = "CREATE TABLE t1 (x BLOB);";
        if (sqlite3_exec(db, create_table, NULL, NULL, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        const char *insert_sql = "INSERT INTO t1 (x) VALUES (?);";
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        initialized = 1;
    }
    
    if (Size == 0) {
        return 0;
    }
    
    sqlite3_reset(stmt);
    sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        return 0;
    }
    
    const char *select_sql = "SELECT dummy_func(x) FROM t1;";
    sqlite3_stmt *select_stmt;
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    if (sqlite3_step(select_stmt) == SQLITE_ROW) {
        (void)sqlite3_column_int(select_stmt, 0);
    }
    
    sqlite3_finalize(select_stmt);
    
    const char *delete_sql = "DELETE FROM t1;";
    sqlite3_exec(db, delete_sql, NULL, NULL, NULL);
    
    return 0;
}
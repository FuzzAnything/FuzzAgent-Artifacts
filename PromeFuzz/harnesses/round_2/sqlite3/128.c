// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_value_double at sqlite3.c:78465:19 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static sqlite3_value* create_test_value(const uint8_t *Data, size_t Size, int value_type) {
    static sqlite3 *db = NULL;
    static sqlite3_stmt *stmt = NULL;
    static int initialized = 0;
    
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return NULL;
        }
        
        const char *create_sql = "CREATE TABLE t1 (col);";
        if (sqlite3_exec(db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return NULL;
        }
        
        const char *insert_sql = "INSERT INTO t1 VALUES (?);";
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return NULL;
        }
        
        initialized = 1;
    }
    
    if (!db || !stmt) return NULL;
    
    sqlite3_reset(stmt);
    
    switch (value_type) {
        case 0:  // BLOB
            sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 100 ? 100 : Size), SQLITE_STATIC);
            break;
        case 1:  // TEXT (UTF-8)
            sqlite3_bind_text(stmt, 1, (const char*)Data, (int)(Size > 100 ? 100 : Size), SQLITE_STATIC);
            break;
        case 2:  // TEXT (UTF-16)
            if (Size > 0) {
                sqlite3_bind_text16(stmt, 1, Data, (int)(Size > 100 ? 100 : Size), SQLITE_STATIC);
            } else {
                sqlite3_bind_null(stmt, 1);
            }
            break;
        case 3:  // INTEGER
            if (Size >= sizeof(int64_t)) {
                int64_t val = *(int64_t*)Data;
                sqlite3_bind_int64(stmt, 1, val);
            } else {
                sqlite3_bind_int(stmt, 1, 42);
            }
            break;
        case 4:  // REAL
            if (Size >= sizeof(double)) {
                double val = *(double*)Data;
                sqlite3_bind_double(stmt, 1, val);
            } else {
                sqlite3_bind_double(stmt, 1, 3.14159);
            }
            break;
        default:
            sqlite3_bind_null(stmt, 1);
            break;
    }
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        return NULL;
    }
    
    sqlite3_stmt *select_stmt = NULL;
    const char *select_sql = "SELECT col FROM t1 LIMIT 1;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) != SQLITE_OK) {
        return NULL;
    }
    
    if (sqlite3_step(select_stmt) != SQLITE_ROW) {
        sqlite3_finalize(select_stmt);
        return NULL;
    }
    
    sqlite3_value *unprotected_value = sqlite3_column_value(select_stmt, 0);
    sqlite3_value *protected_value = sqlite3_value_dup(unprotected_value);
    
    sqlite3_finalize(select_stmt);
    
    return protected_value;
}

static void test_context_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        sqlite3_result_value(ctx, argv[0]);
    } else {
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    for (int value_type = 0; value_type < 5; value_type++) {
        sqlite3_value *val = create_test_value(Data, Size, value_type);
        if (!val) continue;
        
        double d = sqlite3_value_double(val);
        (void)d;
        
        const void *text16 = sqlite3_value_text16(val);
        (void)text16;
        
        const void *blob = sqlite3_value_blob(val);
        (void)blob;
        
        const void *text16be = sqlite3_value_text16be(val);
        (void)text16be;
        
        const void *text16le = sqlite3_value_text16le(val);
        (void)text16le;
        
        sqlite3_value_free(val);
    }
    
    static sqlite3 *test_db = NULL;
    static int db_initialized = 0;
    
    if (!db_initialized) {
        if (sqlite3_open(":memory:", &test_db) != SQLITE_OK) {
            return 0;
        }
        
        sqlite3_create_function(test_db, "test_func", 1, SQLITE_UTF8, NULL, 
                                test_context_function, NULL, NULL);
        db_initialized = 1;
    }
    
    if (!test_db) return 0;
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT test_func(?);";
    
    if (sqlite3_prepare_v2(test_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 50 ? 50 : Size), SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_value *result = sqlite3_column_value(stmt, 0);
            (void)result;
        }
        
        sqlite3_finalize(stmt);
    }
    
    return 0;
}
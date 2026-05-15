// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

static void test_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc > 0 && argv[0]) {
        sqlite3_result_value(context, argv[0]);
    }
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    if (Size < 1) return 0;
    
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        initialized = 1;
    }
    
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER, data BLOB, text TEXT);", 
                 dummy_callback, 0, 0);
    
    char *sql = sqlite3_mprintf("INSERT INTO test VALUES(%d, ?, ?);", (int)Data[0] % 100);
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        if (Size > 1) {
            sqlite3_bind_blob(stmt, 1, Data + 1, Size - 1 < 100 ? Size - 1 : 100, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        
        if (Size > 50) {
            char text_buf[51];
            size_t text_len = Size > 100 ? 50 : Size / 2;
            memcpy(text_buf, Data + (Size/2), text_len);
            text_buf[text_len] = 0;
            sqlite3_bind_text(stmt, 2, text_buf, -1, SQLITE_TRANSIENT);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    sqlite3_free(sql);
    
    sqlite3_stmt *stmt2;
    if (sqlite3_prepare_v2(db, "SELECT id, data, text FROM test ORDER BY id DESC LIMIT 1;", 
                          -1, &stmt2, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt2) == SQLITE_ROW) {
            sqlite3_value *unprotected_val = sqlite3_column_value(stmt2, 0);
            sqlite3_value *unprotected_blob = sqlite3_column_value(stmt2, 1);
            sqlite3_value *unprotected_text = sqlite3_column_value(stmt2, 2);
            
            sqlite3_value *protected_val = sqlite3_value_dup(unprotected_val);
            sqlite3_value *protected_blob = sqlite3_value_dup(unprotected_blob);
            sqlite3_value *protected_text = sqlite3_value_dup(unprotected_text);
            
            if (protected_val) {
                int bytes16 = sqlite3_value_bytes16(protected_val);
                (void)bytes16;
                
                int val_type = sqlite3_value_type(protected_val);
                (void)val_type;
                
                int int_val = sqlite3_value_int(protected_val);
                (void)int_val;
                
                int num_type = sqlite3_value_numeric_type(protected_val);
                (void)num_type;
                
                sqlite3_value_free(protected_val);
            }
            
            if (protected_blob) {
                const void *blob_data = sqlite3_value_blob(protected_blob);
                (void)blob_data;
                
                int blob_bytes16 = sqlite3_value_bytes16(protected_blob);
                (void)blob_bytes16;
                
                sqlite3_value_free(protected_blob);
            }
            
            if (protected_text) {
                int text_bytes16 = sqlite3_value_bytes16(protected_text);
                (void)text_bytes16;
                
                const void *text_as_blob = sqlite3_value_blob(protected_text);
                (void)text_as_blob;
                
                sqlite3_value_free(protected_text);
            }
            
            sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                                   test_func, NULL, NULL);
            
            sqlite3_stmt *stmt3;
            if (sqlite3_prepare_v2(db, "SELECT test_func(id) FROM test;", -1, &stmt3, 0) == SQLITE_OK) {
                sqlite3_step(stmt3);
                sqlite3_finalize(stmt3);
            }
        }
        sqlite3_finalize(stmt2);
    }
    
    sqlite3_exec(db, "DELETE FROM test;", dummy_callback, 0, 0);
    
    return 0;
}
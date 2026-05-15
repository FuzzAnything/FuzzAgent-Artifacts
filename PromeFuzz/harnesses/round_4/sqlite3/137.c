// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_frombind at sqlite3.c:78603:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

static sqlite3_value* create_test_value(const uint8_t *data, size_t size, int value_type) {
    static sqlite3 *db = NULL;
    static sqlite3_stmt *stmt = NULL;
    sqlite3_value *result = NULL;
    
    if (db == NULL) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return NULL;
        }
        
        const char *create_sql = "CREATE TABLE test (id INTEGER, data BLOB, text TEXT);";
        if (sqlite3_exec(db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            db = NULL;
            return NULL;
        }
    }
    
    if (stmt == NULL) {
        const char *insert_sql = "INSERT INTO test VALUES (?, ?, ?);";
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
            return NULL;
        }
    }
    
    sqlite3_reset(stmt);
    
    int int_val = 0;
    const void *blob_data = data;
    int blob_size = size > 0 ? (int)size : 0;
    const char *text_data = (const char*)data;
    int text_size = size > 0 ? (int)size : 0;
    
    if (size > 0) {
        if (size >= sizeof(int)) {
            int_val = *(int*)data;
        }
        
        sqlite3_bind_int(stmt, 1, int_val);
        sqlite3_bind_blob(stmt, 2, blob_data, blob_size, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, text_data, text_size, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_stmt *select_stmt = NULL;
            const char *select_sql = "SELECT data, text FROM test LIMIT 1;";
            
            if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) == SQLITE_OK) {
                if (sqlite3_step(select_stmt) == SQLITE_ROW) {
                    if (value_type == 0) {
                        result = sqlite3_column_value(select_stmt, 0);
                    } else {
                        result = sqlite3_column_value(select_stmt, 1);
                    }
                    
                    if (result) {
                        result = sqlite3_value_dup(result);
                    }
                }
                sqlite3_finalize(select_stmt);
            }
        }
    }
    
    const char *delete_sql = "DELETE FROM test;";
    sqlite3_exec(db, delete_sql, NULL, NULL, NULL);
    
    return result;
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    create_dummy_file(Data, Size);
    
    sqlite3_value *test_value = create_test_value(Data, Size, Size % 2);
    
    if (test_value) {
        int frombind = sqlite3_value_frombind(test_value);
        (void)frombind;
        
        int bytes = sqlite3_value_bytes(test_value);
        (void)bytes;
        
        const void *blob = sqlite3_value_blob(test_value);
        (void)blob;
        
        int bytes16 = sqlite3_value_bytes16(test_value);
        (void)bytes16;
        
        int int_val = sqlite3_value_int(test_value);
        (void)int_val;
        
        int type = sqlite3_value_type(test_value);
        (void)type;
        
        sqlite3_value_free(test_value);
    }
    
    cleanup_dummy_file();
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        sqlite3_value *val = argv[0];
        
        const void *blob_data = sqlite3_value_blob(val);
        const void *text16le_data = sqlite3_value_text16le(val);
        const void *text16be_data = sqlite3_value_text16be(val);
        const void *text16_data = sqlite3_value_text16(val);
        int int_val = sqlite3_value_int(val);
        
        sqlite3_result_value(context, val);
        
        (void)blob_data;
        (void)text16le_data;
        (void)text16be_data;
        (void)text16_data;
        (void)int_val;
    } else {
        sqlite3_result_null(context);
    }
}

static int create_test_database(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return rc;
    }
    
    const char *create_table_sql = 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "text_data TEXT"
        ");";
    
    char *err_msg = NULL;
    rc = sqlite3_exec(*db, create_table_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
    }
    
    rc = sqlite3_create_function(*db, "test_func", 1, SQLITE_UTF8, NULL, 
                                 dummy_func, NULL, NULL);
    
    return rc;
}

static void insert_test_data(sqlite3 *db, const uint8_t *data, size_t size) {
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql = "INSERT INTO test_table (data, text_data) VALUES (?, ?);";
    
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (size > 0) {
            sqlite3_bind_blob(stmt, 1, data, (int)(size > 1024 ? 1024 : size), SQLITE_STATIC);
            
            if (size > 1) {
                char *text_buf = malloc((size > 1024 ? 1024 : size) + 1);
                if (text_buf) {
                    size_t text_len = size > 1024 ? 1024 : size;
                    memcpy(text_buf, data, text_len);
                    text_buf[text_len] = '\0';
                    sqlite3_bind_text(stmt, 2, text_buf, -1, SQLITE_TRANSIENT);
                    free(text_buf);
                } else {
                    sqlite3_bind_null(stmt, 2);
                }
            } else {
                sqlite3_bind_null(stmt, 2);
            }
        } else {
            sqlite3_bind_null(stmt, 1);
            sqlite3_bind_null(stmt, 2);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

static void test_protected_values(sqlite3 *db) {
    sqlite3_stmt *stmt = NULL;
    
    const char *select_sql = "SELECT test_func(data) FROM test_table;";
    
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
        }
        sqlite3_finalize(stmt);
    }
}

static void test_unprotected_values(sqlite3 *db, const uint8_t *data, size_t size) {
    sqlite3_stmt *stmt = NULL;
    const char *select_sql = "SELECT ?;";
    
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (size > 0) {
            sqlite3_bind_blob(stmt, 1, data, (int)(size > 1024 ? 1024 : size), SQLITE_STATIC);
            
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite3_value *unprotected_val = sqlite3_column_value(stmt, 0);
                (void)unprotected_val;
            }
            sqlite3_reset(stmt);
            
            if (size > 1) {
                char *text_buf = malloc((size > 1024 ? 1024 : size) + 1);
                if (text_buf) {
                    size_t text_len = size > 1024 ? 1024 : size;
                    memcpy(text_buf, data, text_len);
                    text_buf[text_len] = '\0';
                    sqlite3_bind_text(stmt, 1, text_buf, -1, SQLITE_TRANSIENT);
                    free(text_buf);
                    
                    if (sqlite3_step(stmt) == SQLITE_ROW) {
                        sqlite3_value *unprotected_val = sqlite3_column_value(stmt, 0);
                        (void)unprotected_val;
                    }
                    sqlite3_reset(stmt);
                }
            }
        }
        
        sqlite3_finalize(stmt);
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    
    if (create_test_database(&db) != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    insert_test_data(db, Data, Size);
    test_protected_values(db);
    test_unprotected_values(db, Data, Size);
    
    sqlite3_close(db);
    
    return 0;
}
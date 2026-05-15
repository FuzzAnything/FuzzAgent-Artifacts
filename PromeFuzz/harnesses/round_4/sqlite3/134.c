// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:78474:25 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc < 1) return;
    
    const void *text16le_result = sqlite3_value_text16le(argv[0]);
    (void)text16le_result;
    
    unsigned int subtype_result = sqlite3_value_subtype(argv[0]);
    (void)subtype_result;
    
    const void *text16_result = sqlite3_value_text16(argv[0]);
    (void)text16_result;
    
    sqlite3_result_value(context, argv[0]);
    
    const void *blob_result = sqlite3_value_blob(argv[0]);
    (void)blob_result;
    
    int int_result = sqlite3_value_int(argv[0]);
    (void)int_result;
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8 | SQLITE_SUBTYPE, 
                               NULL, test_function, NULL, NULL);
        initialized = 1;
    }
    
    if (!db) return 0;
    
    const char *create_table_sql = "CREATE TEMP TABLE IF NOT EXISTS test_data (data BLOB);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    sqlite3_stmt *insert_stmt;
    const char *insert_sql = "INSERT INTO test_data (data) VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_blob(insert_stmt, 1, Data, (int)(Size > 1024 ? 1024 : Size), SQLITE_STATIC);
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    sqlite3_stmt *test_stmt;
    const char *test_sql = "SELECT test_func(data) FROM test_data;";
    if (sqlite3_prepare_v2(db, test_sql, -1, &test_stmt, NULL) == SQLITE_OK) {
        sqlite3_step(test_stmt);
        sqlite3_finalize(test_stmt);
    }
    
    sqlite3_exec(db, "DELETE FROM test_data;", NULL, NULL, NULL);
    
    if (Size > 0) {
        sqlite3_stmt *int_stmt;
        const char *int_sql = "SELECT test_func(?);";
        if (sqlite3_prepare_v2(db, int_sql, -1, &int_stmt, NULL) == SQLITE_OK) {
            if (Size >= 4) {
                int32_t int_val;
                memcpy(&int_val, Data, sizeof(int32_t));
                sqlite3_bind_int(int_stmt, 1, int_val);
            } else {
                sqlite3_bind_int(int_stmt, 1, (int)Data[0]);
            }
            sqlite3_step(int_stmt);
            sqlite3_finalize(int_stmt);
        }
        
        sqlite3_stmt *text_stmt;
        const char *text_sql = "SELECT test_func(?);";
        if (sqlite3_prepare_v2(db, text_sql, -1, &text_stmt, NULL) == SQLITE_OK) {
            char *text_data = (char *)malloc(Size + 1);
            if (text_data) {
                memcpy(text_data, Data, Size);
                text_data[Size] = '\0';
                sqlite3_bind_text(text_stmt, 1, text_data, -1, SQLITE_TRANSIENT);
                free(text_data);
            } else {
                sqlite3_bind_text(text_stmt, 1, "test", -1, SQLITE_STATIC);
            }
            sqlite3_step(text_stmt);
            sqlite3_finalize(text_stmt);
        }
        
        sqlite3_stmt *null_stmt;
        const char *null_sql = "SELECT test_func(?);";
        if (sqlite3_prepare_v2(db, null_sql, -1, &null_stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_null(null_stmt, 1);
            sqlite3_step(null_stmt);
            sqlite3_finalize(null_stmt);
        }
    }
    
    return 0;
}
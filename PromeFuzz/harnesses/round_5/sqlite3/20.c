// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:110746:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int authorizer_callback(void *pUserData, int action_code,
                               const char *arg1, const char *arg2,
                               const char *arg3, const char *arg4) {
    (void)pUserData;
    (void)action_code;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    return SQLITE_OK;
}

static int exec_callback(void *data, int argc, char **argv, char **col_names) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int rc;
    
    if (Size < 1) {
        return 0;
    }
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    char *sql = NULL;
    if (Size > 0) {
        sql = (char *)malloc(Size + 1);
        if (sql == NULL) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql, Data, Size);
        sql[Size] = '\0';
    } else {
        sql = (char *)malloc(1);
        if (sql == NULL) {
            sqlite3_close(db);
            return 0;
        }
        sql[0] = '\0';
    }
    
    rc = sqlite3_exec(db, sql, exec_callback, NULL, &errmsg);
    if (errmsg != NULL) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    rc = sqlite3_set_authorizer(db, authorizer_callback, NULL);
    
    const char *db_name = NULL;
    const char *table_name = NULL;
    const char *column_name = NULL;
    const char *data_type = NULL;
    const char *coll_seq = NULL;
    int not_null = 0;
    int primary_key = 0;
    int autoinc = 0;
    
    if (Size > 3) {
        size_t part1 = Size / 3;
        size_t part2 = part1 * 2;
        
        char *db_name_buf = NULL;
        char *table_name_buf = NULL;
        char *column_name_buf = NULL;
        
        if (part1 > 0) {
            db_name_buf = (char *)malloc(part1 + 1);
            table_name_buf = (char *)malloc(part1 + 1);
        }
        
        if (db_name_buf && table_name_buf) {
            memcpy(db_name_buf, Data, part1);
            db_name_buf[part1] = '\0';
            db_name = db_name_buf;
            
            memcpy(table_name_buf, Data + part1, part1);
            table_name_buf[part1] = '\0';
            table_name = table_name_buf;
        }
        
        size_t col_len = Size - part2;
        if (col_len > 0) {
            column_name_buf = (char *)malloc(col_len + 1);
            if (column_name_buf) {
                memcpy(column_name_buf, Data + part2, col_len);
                column_name_buf[col_len] = '\0';
                column_name = column_name_buf;
            }
        }
        
        rc = sqlite3_table_column_metadata(db, db_name, table_name, column_name,
                                          &data_type, &coll_seq, &not_null,
                                          &primary_key, &autoinc);
        
        free(db_name_buf);
        free(table_name_buf);
        free(column_name_buf);
    }
    
    int test_op = 0;
    if (Size > 0) {
        test_op = (int)Data[0] % 100;
    }
    
    rc = sqlite3_test_control(test_op, 0, 0, 0, 0, 0, 0, 0);
    
    int alloc_size = 0;
    if (Size > 0) {
        alloc_size = (int)(Data[0] % 1024);
    }
    void *mem = sqlite3_malloc(alloc_size);
    if (mem != NULL) {
        sqlite3_free(mem);
    }
    
    free(sql);
    sqlite3_close(db);
    
    return 0;
}
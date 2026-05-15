// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    char *err_msg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    int rc;
    int current, highwater;
    
    // Use fuzzer input to determine some parameters
    int config_option = Size > 0 ? Data[0] % 10 : 0;
    int db_status_op = Size > 1 ? Data[1] % 10 : 0;
    int status_op = Size > 2 ? Data[2] % 10 : 0;
    int reset_flag = Size > 3 ? Data[3] % 2 : 0;
    
    // 1. sqlite3_config
    // Use SQLITE_CONFIG_URI (an anytime option) to avoid SQLITE_MISUSE
    sqlite3_config(SQLITE_CONFIG_URI, 1);
    
    // 2. sqlite3_open
    rc = sqlite3_open(":memory:", &db);
    
    // 3. sqlite3_errmsg
    const char *err_str = sqlite3_errmsg(db);
    (void)err_str;
    
    // 4. sqlite3_exec
    char *sql = NULL;
    if (Size > 0) {
        sql = (char *)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            sqlite3_exec(db, sql, dummy_callback, NULL, &err_msg);
            free(sql);
        }
    }
    
    // 5. sqlite3_free
    sqlite3_free(err_msg);
    err_msg = NULL;
    
    // 6-15. sqlite3_db_status (10 calls with different op codes)
    // Use various SQLITE_DBSTATUS options
    int db_status_ops[] = {
        SQLITE_DBSTATUS_LOOKASIDE_USED,
        SQLITE_DBSTATUS_LOOKASIDE_HIT,
        SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
        SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL,
        SQLITE_DBSTATUS_CACHE_USED,
        SQLITE_DBSTATUS_SCHEMA_USED,
        SQLITE_DBSTATUS_STMT_USED,
        SQLITE_DBSTATUS_CACHE_HIT,
        SQLITE_DBSTATUS_CACHE_MISS,
        SQLITE_DBSTATUS_CACHE_WRITE
    };
    
    for (int i = 0; i < 10; i++) {
        int op = (i < sizeof(db_status_ops)/sizeof(db_status_ops[0])) ? 
                 db_status_ops[i] : SQLITE_DBSTATUS_LOOKASIDE_USED;
        sqlite3_db_status(db, op, &current, &highwater, reset_flag);
    }
    
    // 16. sqlite3_close
    sqlite3_close(db);
    db = NULL;
    
    // 17-21. sqlite3_status (5 calls with different op codes)
    int status_ops[] = {
        SQLITE_STATUS_MEMORY_USED,
        SQLITE_STATUS_PAGECACHE_USED,
        SQLITE_STATUS_PAGECACHE_OVERFLOW,
        SQLITE_STATUS_SCRATCH_USED,
        SQLITE_STATUS_SCRATCH_OVERFLOW
    };
    
    for (int i = 0; i < 5; i++) {
        int op = (i < sizeof(status_ops)/sizeof(status_ops[0])) ? 
                 status_ops[i] : SQLITE_STATUS_MEMORY_USED;
        sqlite3_status(op, &current, &highwater, reset_flag);
    }
    
    // 22. sqlite3_mprintf
    str1 = sqlite3_mprintf("Test string %d", (int)Size);
    
    // 23. sqlite3_free
    sqlite3_free(str1);
    str1 = NULL;
    
    // 24. sqlite3_mprintf with fuzzer data
    if (Size > 0) {
        char *temp = (char *)malloc(Size + 1);
        if (temp) {
            memcpy(temp, Data, Size);
            temp[Size] = '\0';
            str2 = sqlite3_mprintf("Data: %s", temp);
            free(temp);
        }
    }
    
    // 25. sqlite3_free
    sqlite3_free(str2);
    str2 = NULL;
    
    // 26. sqlite3_open (second database)
    rc = sqlite3_open("./dummy_file", &db2);
    
    // 27. sqlite3_errmsg
    err_str = sqlite3_errmsg(db2);
    (void)err_str;
    
    // 28-30. sqlite3_exec (three calls)
    sqlite3_exec(db2, "CREATE TABLE IF NOT EXISTS t1(a,b);", dummy_callback, NULL, &err_msg);
    sqlite3_free(err_msg);
    err_msg = NULL;
    
    if (Size > 0) {
        char *insert_sql = (char *)malloc(100 + Size);
        if (insert_sql) {
            snprintf(insert_sql, 100, "INSERT INTO t1 VALUES(%d,", (int)Size);
            if (Size < 50) {
                strncat(insert_sql, (const char*)Data, Size);
            } else {
                strncat(insert_sql, (const char*)Data, 50);
            }
            strcat(insert_sql, ");");
            sqlite3_exec(db2, insert_sql, dummy_callback, NULL, &err_msg);
            free(insert_sql);
        }
    }
    
    sqlite3_free(err_msg);
    err_msg = NULL;
    
    sqlite3_exec(db2, "SELECT * FROM t1;", dummy_callback, NULL, &err_msg);
    
    // 31. sqlite3_free
    sqlite3_free(err_msg);
    err_msg = NULL;
    
    // 32. sqlite3_close
    sqlite3_close(db2);
    
    return 0;
}
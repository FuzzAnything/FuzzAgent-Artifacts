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
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    char *err_msg = NULL;
    char *mprintf_str1 = NULL;
    char *mprintf_str2 = NULL;
    int current, highwater;
    int i;
    
    // Use fuzzer input to influence behavior
    // We'll use the first byte to determine some options
    uint8_t options = Size > 0 ? Data[0] : 0;
    
    // 1. sqlite3_config
    // Use SQLITE_CONFIG_URI which is an "anytime" option
    int config_uri = (options & 0x01) ? 1 : 0;
    sqlite3_config(SQLITE_CONFIG_URI, config_uri);
    
    // 2. sqlite3_open
    const char *filename = "./dummy_file";
    int rc = sqlite3_open(filename, &db);
    
    // 3. sqlite3_errmsg
    const char *err = sqlite3_errmsg(db);
    (void)err;
    
    // 4. sqlite3_exec
    // Create SQL from fuzzer data (limited to safe size)
    char sql[256];
    size_t sql_len = Size < 255 ? Size : 255;
    if (sql_len > 0) {
        memcpy(sql, Data, sql_len);
        sql[sql_len] = '\0';
    } else {
        strcpy(sql, "CREATE TABLE t1(a,b);");
    }
    
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
    // 5. sqlite3_free
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // 6-15. sqlite3_db_status (10 times with different op codes)
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
    
    for (i = 0; i < 10 && i < (int)(sizeof(db_status_ops)/sizeof(db_status_ops[0])); i++) {
        int reset = (options >> (i % 8)) & 0x01;
        sqlite3_db_status(db, db_status_ops[i], &current, &highwater, reset);
    }
    
    // 16. sqlite3_close (but we need to close later, so save for now)
    // Actually we'll close at the end
    
    // 17-21. sqlite3_status (5 times with different op codes)
    int status_ops[] = {
        SQLITE_STATUS_MEMORY_USED,
        SQLITE_STATUS_PAGECACHE_USED,
        SQLITE_STATUS_PAGECACHE_OVERFLOW,
        SQLITE_STATUS_SCRATCH_USED,
        SQLITE_STATUS_SCRATCH_OVERFLOW
    };
    
    for (i = 0; i < 5 && i < (int)(sizeof(status_ops)/sizeof(status_ops[0])); i++) {
        int reset = (options >> (i % 8)) & 0x01;
        sqlite3_status(status_ops[i], &current, &highwater, reset);
    }
    
    // 22. sqlite3_mprintf
    // Use fuzzer data for format string (limited)
    char fmt[64];
    size_t fmt_len = Size < 63 ? Size : 63;
    if (fmt_len > 0) {
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
    } else {
        strcpy(fmt, "test %d");
    }
    
    mprintf_str1 = sqlite3_mprintf(fmt, 123);
    
    // 23. sqlite3_free
    if (mprintf_str1) {
        sqlite3_free(mprintf_str1);
        mprintf_str1 = NULL;
    }
    
    // 24. sqlite3_mprintf
    mprintf_str2 = sqlite3_mprintf("fuzz: %.*s", (int)(Size > 100 ? 100 : Size), Data);
    
    // 25. sqlite3_free
    if (mprintf_str2) {
        sqlite3_free(mprintf_str2);
        mprintf_str2 = NULL;
    }
    
    // 26. sqlite3_open (second database)
    rc = sqlite3_open(":memory:", &db2);
    
    // 27. sqlite3_errmsg
    err = sqlite3_errmsg(db2);
    (void)err;
    
    // 28-30. sqlite3_exec (3 times)
    // First exec: create table
    rc = sqlite3_exec(db2, "CREATE TABLE IF NOT EXISTS t2(x,y,z);", callback, 0, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Second exec: insert (using fuzzer data for values)
    char insert_sql[256];
    int val1 = Size > 0 ? Data[0] : 0;
    int val2 = Size > 1 ? Data[1] : 0;
    snprintf(insert_sql, sizeof(insert_sql), "INSERT INTO t2 VALUES(%d,%d,%d);", val1, val2, (int)Size);
    rc = sqlite3_exec(db2, insert_sql, callback, 0, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Third exec: select
    rc = sqlite3_exec(db2, "SELECT * FROM t2;", callback, 0, &err_msg);
    
    // 31. sqlite3_free
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // 32. sqlite3_close (both databases)
    if (db) {
        sqlite3_close(db);
    }
    if (db2) {
        sqlite3_close(db2);
    }
    
    return 0;
}
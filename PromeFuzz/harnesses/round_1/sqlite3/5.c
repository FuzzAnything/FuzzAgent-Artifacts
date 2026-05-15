// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int rc;
    int current, highwater;
    
    // Use fuzzer data to create a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        }
        fclose(fp);
    }
    
    // 1. sqlite3_config
    // Use an "anytime" configuration option to avoid SQLITE_MISUSE
    sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
    
    // 2. sqlite3_open
    rc = sqlite3_open("./dummy_file", &db);
    
    // 3. sqlite3_errmsg
    if (db) {
        const char *err = sqlite3_errmsg(db);
        (void)err;
    }
    
    // 4. sqlite3_exec
    if (db) {
        char *sql;
        if (Size > 0) {
            sql = (char*)malloc(Size + 1);
            if (sql) {
                memcpy(sql, Data, Size);
                sql[Size] = '\0';
                sqlite3_exec(db, sql, callback, 0, &err_msg);
                free(sql);
            }
        } else {
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a,b);", callback, 0, &err_msg);
        }
    }
    
    // 5. sqlite3_free
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // 6-15. sqlite3_db_status (10 calls with different op codes)
    if (db) {
        sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_SCHEMA_USED, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_STMT_USED, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_HIT, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_HIT, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_MISS, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_WRITE, &current, &highwater, 0);
    }
    
    // 16. sqlite3_close
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
    
    // 17-21. sqlite3_status (5 calls with different op codes)
    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current, &highwater, 0);
    sqlite3_status(SQLITE_STATUS_PAGECACHE_USED, &current, &highwater, 0);
    sqlite3_status(SQLITE_STATUS_PAGECACHE_OVERFLOW, &current, &highwater, 0);
    sqlite3_status(SQLITE_STATUS_SCRATCH_USED, &current, &highwater, 0);
    sqlite3_status(SQLITE_STATUS_SCRATCH_OVERFLOW, &current, &highwater, 0);
    
    // 22. sqlite3_mprintf
    char *str1 = sqlite3_mprintf("Test %d", (int)(Size & 0xFF));
    
    // 23. sqlite3_free
    if (str1) {
        sqlite3_free(str1);
    }
    
    // 24. sqlite3_mprintf
    char *str2 = sqlite3_mprintf("%.*s", (int)(Size > 100 ? 100 : Size), Data);
    
    // 25. sqlite3_free
    if (str2) {
        sqlite3_free(str2);
    }
    
    // 26. sqlite3_open (second database)
    sqlite3 *db2 = NULL;
    rc = sqlite3_open(":memory:", &db2);
    
    // 27. sqlite3_errmsg
    if (db2) {
        const char *err2 = sqlite3_errmsg(db2);
        (void)err2;
    }
    
    // 28-30. sqlite3_exec (3 calls)
    if (db2) {
        sqlite3_exec(db2, "CREATE TABLE t2(x,y,z);", callback, 0, &err_msg);
        
        if (Size > 0) {
            char *sql2 = (char*)malloc(Size + 1);
            if (sql2) {
                memcpy(sql2, Data, Size);
                sql2[Size] = '\0';
                sqlite3_exec(db2, sql2, callback, 0, &err_msg);
                free(sql2);
            }
        }
        
        sqlite3_exec(db2, "SELECT * FROM t2;", callback, 0, &err_msg);
    }
    
    // 31. sqlite3_free
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // 32. sqlite3_close
    if (db2) {
        sqlite3_close(db2);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}
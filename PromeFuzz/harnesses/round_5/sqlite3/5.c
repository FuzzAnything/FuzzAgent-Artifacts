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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    char *errmsg = NULL;
    char *mprintf_str1 = NULL;
    char *mprintf_str2 = NULL;
    int current = 0;
    int highwater = 0;
    int rc;
    
    // Use fuzz data to determine configuration
    int config_op = Data[0] % 10;
    rc = sqlite3_config(config_op);
    (void)rc;
    
    // Open first database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK && db) {
        const char *err = sqlite3_errmsg(db);
        (void)err;
    }
    
    if (db) {
        // Execute SQL using fuzz data
        size_t sql_len = Size > 100 ? 100 : Size;
        char *sql = (char*)malloc(sql_len + 1);
        if (sql) {
            memcpy(sql, Data, sql_len);
            sql[sql_len] = '\0';
            rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql);
        }
        
        // Call sqlite3_db_status multiple times with different op codes
        int db_status_ops[] = {
            SQLITE_DBSTATUS_LOOKASIDE_USED,
            SQLITE_DBSTATUS_CACHE_USED,
            SQLITE_DBSTATUS_SCHEMA_USED,
            SQLITE_DBSTATUS_STMT_USED,
            SQLITE_DBSTATUS_LOOKASIDE_HIT,
            SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
            SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL,
            SQLITE_DBSTATUS_CACHE_HIT,
            SQLITE_DBSTATUS_CACHE_MISS,
            SQLITE_DBSTATUS_CACHE_WRITE
        };
        
        for (int i = 0; i < 10 && i < (int)(Size); i++) {
            int op = db_status_ops[i % 10];
            rc = sqlite3_db_status(db, op, &current, &highwater, Data[i] & 1);
            (void)rc;
        }
        
        // Close first database
        sqlite3_close(db);
    }
    
    // Call sqlite3_status multiple times
    int status_ops[] = {
        SQLITE_STATUS_MEMORY_USED,
        SQLITE_STATUS_PAGECACHE_USED,
        SQLITE_STATUS_PAGECACHE_OVERFLOW,
        SQLITE_STATUS_SCRATCH_USED,
        SQLITE_STATUS_SCRATCH_OVERFLOW
    };
    
    for (int i = 0; i < 5 && i < (int)(Size); i++) {
        int op = status_ops[i % 5];
        rc = sqlite3_status(op, &current, &highwater, Data[i] & 1);
        (void)rc;
    }
    
    // Test sqlite3_mprintf and sqlite3_free
    if (Size > 10) {
        mprintf_str1 = sqlite3_mprintf("Fuzz: %.*s", (int)(Size > 50 ? 50 : Size), Data);
        if (mprintf_str1) {
            sqlite3_free(mprintf_str1);
        }
        
        mprintf_str2 = sqlite3_mprintf("Data: %d bytes", (int)Size);
        if (mprintf_str2) {
            sqlite3_free(mprintf_str2);
        }
    }
    
    // Open second database
    rc = sqlite3_open(":memory:", &db2);
    if (rc != SQLITE_OK && db2) {
        const char *err = sqlite3_errmsg(db2);
        (void)err;
    }
    
    if (db2) {
        // Execute multiple SQL statements
        for (int i = 0; i < 3 && i < (int)(Size); i++) {
            size_t offset = i * 10;
            if (offset + 10 <= Size) {
                char sql_buf[11];
                memcpy(sql_buf, Data + offset, 10);
                sql_buf[10] = '\0';
                
                rc = sqlite3_exec(db2, sql_buf, callback, 0, &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = NULL;
                }
            } else {
                rc = sqlite3_exec(db2, "SELECT 1", callback, 0, &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = NULL;
                }
            }
        }
        
        // Close second database
        sqlite3_close(db2);
    }
    
    return 0;
}
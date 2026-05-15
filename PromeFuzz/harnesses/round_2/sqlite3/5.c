// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
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
    // Use a fixed-size buffer for file operations
    static const char* filename = "./dummy_file";
    
    // Write data to file for sqlite3_open
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Initialize variables
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    char *errmsg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    int current = 0;
    int highwater = 0;
    int rc;
    
    // 1. sqlite3_config
    // Use SQLITE_CONFIG_LOG which is an "anytime" option
    rc = sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
    (void)rc;
    
    // 2. sqlite3_open
    rc = sqlite3_open(filename, &db);
    
    // 3. sqlite3_errmsg
    if (db) {
        const char *err = sqlite3_errmsg(db);
        (void)err;
    }
    
    // 4. sqlite3_exec
    if (db) {
        // Use a simple SQL command
        rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a,b);", callback, 0, &errmsg);
    }
    
    // 5. sqlite3_free
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // 6-15. sqlite3_db_status (10 times)
    if (db) {
        // Try different op codes
        int ops[] = {
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
        
        for (int i = 0; i < 10 && i < (int)(Size/sizeof(int)); i++) {
            int op = ops[i % 10];
            int resetFlag = (Data[i] & 1) ? 1 : 0;
            sqlite3_db_status(db, op, &current, &highwater, resetFlag);
        }
    }
    
    // 16. sqlite3_close (first database)
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
    
    // 17-21. sqlite3_status (5 times)
    {
        int ops[] = {
            SQLITE_STATUS_MEMORY_USED,
            SQLITE_STATUS_PAGECACHE_USED,
            SQLITE_STATUS_PAGECACHE_OVERFLOW,
            SQLITE_STATUS_SCRATCH_USED,
            SQLITE_STATUS_SCRATCH_OVERFLOW
        };
        
        for (int i = 0; i < 5 && i < (int)(Size/sizeof(int)); i++) {
            int op = ops[i % 5];
            int resetFlag = (Data[i] & 1) ? 1 : 0;
            sqlite3_status(op, &current, &highwater, resetFlag);
        }
    }
    
    // 22. sqlite3_mprintf
    if (Size > 0) {
        str1 = sqlite3_mprintf("%.*s", (int)Size, (const char*)Data);
    } else {
        str1 = sqlite3_mprintf("test");
    }
    
    // 23. sqlite3_free
    if (str1) {
        sqlite3_free(str1);
        str1 = NULL;
    }
    
    // 24. sqlite3_mprintf
    if (Size > 0) {
        str2 = sqlite3_mprintf("Data: %.*s", (int)(Size > 100 ? 100 : Size), (const char*)Data);
    } else {
        str2 = sqlite3_mprintf("test2");
    }
    
    // 25. sqlite3_free
    if (str2) {
        sqlite3_free(str2);
        str2 = NULL;
    }
    
    // 26. sqlite3_open (second database)
    rc = sqlite3_open(":memory:", &db2);
    
    // 27. sqlite3_errmsg
    if (db2) {
        const char *err = sqlite3_errmsg(db2);
        (void)err;
    }
    
    // 28-30. sqlite3_exec (3 times)
    if (db2) {
        // First exec
        rc = sqlite3_exec(db2, "CREATE TABLE t2(x,y,z);", callback, 0, &errmsg);
        
        // Free error message if any
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        
        // Second exec - try to insert data based on fuzzer input
        if (Size > 0) {
            char sql[256];
            int len = Size > 100 ? 100 : (int)Size;
            snprintf(sql, sizeof(sql), "INSERT INTO t2 VALUES('%.*s',%d,%d);", 
                    len, (const char*)Data, (int)Data[0], (int)Size);
            rc = sqlite3_exec(db2, sql, callback, 0, &errmsg);
        } else {
            rc = sqlite3_exec(db2, "INSERT INTO t2 VALUES('a',1,2);", callback, 0, &errmsg);
        }
        
        // Free error message if any
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        
        // Third exec
        rc = sqlite3_exec(db2, "SELECT * FROM t2;", callback, 0, &errmsg);
    }
    
    // 31. sqlite3_free
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // 32. sqlite3_close (second database)
    if (db2) {
        sqlite3_close(db2);
        db2 = NULL;
    }
    
    // Clean up file
    remove(filename);
    
    return 0;
}
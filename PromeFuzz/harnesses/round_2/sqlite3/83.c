// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

static int dummy_busy_handler(void *data, int count) {
    return 0;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    sqlite3 *db1 = NULL;
    sqlite3 *db2 = NULL;
    sqlite3_backup *backup = NULL;
    int rc;
    
    rc = sqlite3_open(":memory:", &db1);
    if (rc != SQLITE_OK) {
        if (db1) sqlite3_close(db1);
        return 0;
    }
    
    rc = sqlite3_open(":memory:", &db2);
    if (rc != SQLITE_OK) {
        sqlite3_close(db1);
        if (db2) sqlite3_close(db2);
        return 0;
    }
    
    sqlite3_busy_handler(db1, dummy_busy_handler, NULL);
    sqlite3_busy_handler(db2, dummy_busy_handler, NULL);
    
    char *err_msg = NULL;
    sqlite3_exec(db1, "CREATE TABLE t1(a INTEGER, b TEXT);", NULL, NULL, &err_msg);
    if (err_msg) sqlite3_free(err_msg);
    err_msg = NULL;
    
    sqlite3_exec(db1, "INSERT INTO t1 VALUES(1, 'test');", NULL, NULL, &err_msg);
    if (err_msg) sqlite3_free(err_msg);
    err_msg = NULL;
    
    backup = sqlite3_backup_init(db2, "main", db1, "main");
    
    if (backup) {
        int nPage = 0;
        if (Size > 0) {
            nPage = Data[0];
            if (nPage > 127) nPage = -1;
        }
        
        rc = sqlite3_backup_step(backup, nPage);
        
        int pagecount = sqlite3_backup_pagecount(backup);
        int remaining = sqlite3_backup_remaining(backup);
        
        if (rc == SQLITE_OK) {
            int nPage2 = 5;
            if (Size > 1) {
                nPage2 = Data[1] % 20;
            }
            sqlite3_backup_step(backup, nPage2);
            
            pagecount = sqlite3_backup_pagecount(backup);
            remaining = sqlite3_backup_remaining(backup);
        }
        
        rc = sqlite3_backup_finish(backup);
    }
    
    if (Size >= 512) {
        sqlite3 *db3 = NULL;
        rc = sqlite3_open(":memory:", &db3);
        if (rc == SQLITE_OK) {
            size_t usable_size = (Size / 512) * 512;
            if (usable_size > 0) {
                unsigned char *buffer = (unsigned char*)sqlite3_malloc(usable_size);
                if (buffer) {
                    memcpy(buffer, Data, usable_size > Size ? Size : usable_size);
                    
                    rc = sqlite3_deserialize(db3, "main", buffer, usable_size, usable_size, 0);
                    
                    if (rc != SQLITE_OK) {
                        buffer = (unsigned char*)sqlite3_malloc(usable_size);
                        if (buffer) {
                            memcpy(buffer, Data, usable_size > Size ? Size : usable_size);
                            rc = sqlite3_deserialize(db3, "main", buffer, usable_size, usable_size, 
                                                   SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE);
                        }
                    }
                }
            }
            sqlite3_close(db3);
        }
    }
    
    sqlite3_close(db1);
    sqlite3_close(db2);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
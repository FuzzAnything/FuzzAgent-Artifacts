// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    sqlite3 *db1 = NULL;
    sqlite3 *db2 = NULL;
    sqlite3_backup *backup = NULL;
    int rc;
    
    rc = sqlite3_open(":memory:", &db1);
    if (rc != SQLITE_OK) return 0;
    
    rc = sqlite3_open(":memory:", &db2);
    if (rc != SQLITE_OK) {
        sqlite3_close(db1);
        return 0;
    }
    
    sqlite3_busy_handler(db1, dummy_busy_handler, NULL);
    sqlite3_busy_handler(db2, dummy_busy_handler, NULL);
    
    if (Size > 0) {
        char *str_arg = "";
        char temp_buf[1025];
        
        if (Size > 1) {
            size_t copy_len = Size - 1;
            if (copy_len > sizeof(temp_buf) - 1) {
                copy_len = sizeof(temp_buf) - 1;
            }
            memcpy(temp_buf, &Data[1], copy_len);
            temp_buf[copy_len] = '\0';
            str_arg = temp_buf;
        }
        
        char *sql = sqlite3_mprintf(
            "CREATE TABLE IF NOT EXISTS t1(a, b); "
            "INSERT INTO t1 VALUES(%d, %Q); "
            "CREATE TABLE IF NOT EXISTS t2(x, y);",
            (int)(Data[0] % 256),
            str_arg
        );
        if (sql) {
            sqlite3_exec(db1, sql, NULL, NULL, NULL);
            sqlite3_free(sql);
        }
    }
    
    const char *dest_names[] = {"main", "temp", NULL};
    const char *src_names[] = {"main", NULL};
    
    for (int i = 0; dest_names[i]; i++) {
        for (int j = 0; src_names[j]; j++) {
            backup = sqlite3_backup_init(db2, dest_names[i], db1, src_names[j]);
            if (backup) {
                int pagecount = sqlite3_backup_pagecount(backup);
                int remaining = sqlite3_backup_remaining(backup);
                
                if (Size > 0) {
                    int nPage = (int)(Data[0] % 256);
                    if (nPage == 0) nPage = -1;
                    
                    do {
                        rc = sqlite3_backup_step(backup, nPage);
                        if (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
                            pagecount = sqlite3_backup_pagecount(backup);
                            remaining = sqlite3_backup_remaining(backup);
                        } else if (rc == SQLITE_DONE) {
                            break;
                        } else {
                            break;
                        }
                    } while (remaining > 0);
                }
                
                rc = sqlite3_backup_finish(backup);
                backup = NULL;
            }
        }
    }
    
    if (Size >= 100) {
        unsigned char *data_copy = (unsigned char*)malloc(Size);
        if (data_copy) {
            memcpy(data_copy, Data, Size);
            
            unsigned int flags = 0;
            if (Size > 0) {
                flags = Data[0] % 4;
            }
            
            rc = sqlite3_deserialize(db1, "main", data_copy, (sqlite3_int64)Size, 
                                     (sqlite3_int64)Size, flags);
            
            if (rc != SQLITE_OK && !(flags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                free(data_copy);
            }
        }
    }
    
    if (backup) {
        sqlite3_backup_finish(backup);
    }
    sqlite3_close(db1);
    sqlite3_close(db2);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
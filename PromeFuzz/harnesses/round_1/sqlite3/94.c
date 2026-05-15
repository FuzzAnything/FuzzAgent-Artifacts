// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_busy_handler(void *data, int count) {
    return 0;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    sqlite3 *db1 = NULL;
    sqlite3 *db2 = NULL;
    sqlite3_backup *backup = NULL;
    int rc;
    
    rc = sqlite3_open_v2("./dummy_file", &db1, 
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                        NULL);
    if (rc != SQLITE_OK || db1 == NULL) {
        rc = sqlite3_open_v2(":memory:", &db1, SQLITE_OPEN_READWRITE, NULL);
        if (rc != SQLITE_OK || db1 == NULL) {
            return 0;
        }
    }
    
    sqlite3_busy_handler(db1, dummy_busy_handler, NULL);
    
    if (Size > 0) {
        uint8_t db_name_selector = Data[0] % 4;
        const char *db_names[] = {"main", "temp", NULL, "invalid"};
        const char *test_name = db_names[db_name_selector];
        
        if (test_name != NULL) {
            int readonly = sqlite3_db_readonly(db1, test_name);
            (void)readonly;
        }
    }
    
    if (Size > 1) {
        int n = (int)(Data[1] % 10);
        const char *db_name = sqlite3_db_name(db1, n);
        if (db_name != NULL) {
            (void)db_name;
        }
    }
    
    if (Size >= 100) {
        unsigned char *data_copy = malloc(Size);
        if (data_copy != NULL) {
            memcpy(data_copy, Data, Size);
            
            rc = sqlite3_deserialize(db1, "main", data_copy, (sqlite3_int64)Size, 
                                     (sqlite3_int64)Size, SQLITE_DESERIALIZE_RESIZEABLE);
            
            if (rc == SQLITE_BUSY) {
                rc = sqlite3_deserialize(db1, "temp", data_copy, (sqlite3_int64)Size,
                                         (sqlite3_int64)Size, SQLITE_DESERIALIZE_RESIZEABLE);
            }
            
            free(data_copy);
        }
    }
    
    rc = sqlite3_open_v2(":memory:", &db2, SQLITE_OPEN_READWRITE, NULL);
    if (rc == SQLITE_OK && db2 != NULL) {
        backup = sqlite3_backup_init(db2, "main", db1, "main");
        if (backup != NULL) {
            for (int i = 0; i < 3 && rc == SQLITE_OK; i++) {
                rc = sqlite3_backup_step(backup, 5);
                if (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
                    if (rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
                        rc = SQLITE_OK;
                    }
                    
                    int remaining = sqlite3_backup_remaining(backup);
                    int pagecount = sqlite3_backup_pagecount(backup);
                    (void)remaining;
                    (void)pagecount;
                }
            }
            
            sqlite3_backup_finish(backup);
        }
        
        int readonly2 = sqlite3_db_readonly(db2, "main");
        (void)readonly2;
        
        sqlite3_close(db2);
    }
    
    rc = sqlite3_close(db1);
    if (rc == SQLITE_BUSY) {
        sqlite3_close(db1);
    }
    
    sqlite3_close(NULL);
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Create database connections */
    sqlite3 *db1 = NULL;
    sqlite3 *db2 = NULL;
    int rc = sqlite3_open(":memory:", &db1);
    if (rc != SQLITE_OK) return 0;
    
    rc = sqlite3_open(":memory:", &db2);
    if (rc != SQLITE_OK) {
        sqlite3_close(db1);
        return 0;
    }

    /* Create some tables and data in source database */
    char *err_msg = NULL;
    sqlite3_exec(db1, "CREATE TABLE t1(a INTEGER, b TEXT);", NULL, NULL, &err_msg);
    sqlite3_exec(db1, "INSERT INTO t1 VALUES(1, 'test');", NULL, NULL, &err_msg);
    sqlite3_exec(db1, "CREATE TABLE t2(x REAL, y BLOB);", NULL, NULL, &err_msg);
    
    /* Initialize backup object */
    sqlite3_backup *backup = sqlite3_backup_init(db2, "main", db1, "main");
    
    if (backup) {
        /* Use fuzzer input to determine backup parameters */
        int nPage = (int)(Data[0] % 256);
        if (nPage == 0) nPage = -1; /* Negative means all remaining pages */
        
        /* Call backup_step multiple times with different parameters */
        for (int i = 0; i < 3 && Size > i; i++) {
            int step_nPage = (int)(Data[i % Size] % 256);
            if (step_nPage == 0) step_nPage = -1;
            
            rc = sqlite3_backup_step(backup, step_nPage);
            if (rc == SQLITE_DONE || rc == SQLITE_OK) {
                /* Query progress information */
                int remaining = sqlite3_backup_remaining(backup);
                int pagecount = sqlite3_backup_pagecount(backup);
                (void)remaining; /* Use variables to avoid unused warnings */
                (void)pagecount;
            }
            
            /* Try different page counts based on fuzzer input */
            if (Size > 10) {
                int alt_nPage = (int)(Data[(i+5) % Size] % 512) - 256;
                rc = sqlite3_backup_step(backup, alt_nPage);
            }
        }
        
        /* Finalize backup */
        sqlite3_backup_finish(backup);
    }
    
    /* Test deserialize function */
    if (Size > 100) {
        /* Allocate buffer using sqlite3_malloc for FREEONCLOSE flag */
        unsigned char *pData = (unsigned char *)sqlite3_malloc(Size);
        if (pData) {
            memcpy(pData, Data, Size);
            
            /* Try different schema names */
            const char *schemas[] = {"main", "temp", NULL};
            for (int i = 0; i < 2; i++) {
                /* Calculate sizes - ensure szBuf >= szDb */
                sqlite3_int64 szDb = (sqlite3_int64)(Size / 512) * 512;
                if (szDb < 512) szDb = 512;
                if (szDb > Size) szDb = Size;
                
                sqlite3_int64 szBuf = Size;
                if (szBuf < szDb) szBuf = szDb;
                
                /* Try different flags */
                unsigned int flags[] = {0, SQLITE_DESERIALIZE_READONLY, 
                                       SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE};
                
                for (int j = 0; j < 3; j++) {
                    /* Create a new database connection for deserialize test */
                    sqlite3 *db3 = NULL;
                    if (sqlite3_open(":memory:", &db3) == SQLITE_OK) {
                        /* Modify database header to potentially force rollback mode */
                        if (Size > 19) {
                            pData[18] = 0x01;
                            pData[19] = 0x01;
                        }
                        
                        rc = sqlite3_deserialize(db3, schemas[i], pData, szDb, szBuf, flags[j]);
                        
                        /* If using FREEONCLOSE, sqlite3 will free pData, so we need to reallocate */
                        if (flags[j] & SQLITE_DESERIALIZE_FREEONCLOSE) {
                            pData = (unsigned char *)sqlite3_malloc(Size);
                            if (pData) {
                                memcpy(pData, Data, Size);
                            } else {
                                break;
                            }
                        }
                        
                        sqlite3_close(db3);
                    }
                }
            }
            
            /* Free if not already freed by sqlite3 */
            if (pData) sqlite3_free(pData);
        }
    }
    
    /* Clean up */
    sqlite3_close(db1);
    sqlite3_close(db2);
    
    return 0;
}
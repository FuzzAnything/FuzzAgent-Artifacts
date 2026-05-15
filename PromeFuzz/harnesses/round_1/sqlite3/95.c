// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:173158:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:17178:17 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:17081:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_trace(unsigned int T, void* C, void* P, void* X) {
    (void)T;
    (void)C;
    (void)P;
    (void)X;
    return 0;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Use first byte to determine fuzzing mode */
    uint8_t mode = Data[0];
    const uint8_t *params = Data + 1;
    size_t params_size = Size - 1;

    /* Initialize sqlite3 if needed */
    sqlite3_initialize();

    /* Test sqlite3_soft_heap_limit */
    {
        int limit = 0;
        if (params_size >= sizeof(int)) {
            memcpy(&limit, params, sizeof(int));
        } else if (params_size > 0) {
            limit = params[0] * 1000;
        }
        sqlite3_soft_heap_limit(limit);
    }

    /* Test sqlite3_hard_heap_limit64 */
    {
        sqlite3_int64 hard_limit = 0;
        if (params_size >= sizeof(sqlite3_int64)) {
            memcpy(&hard_limit, params, sizeof(sqlite3_int64));
        } else if (params_size > 0) {
            hard_limit = params[0] * 1024 * 1024;
        }
        sqlite3_int64 prev = sqlite3_hard_heap_limit64(hard_limit);
        (void)prev; /* Use result to avoid compiler warning */
    }

    /* Test sqlite3_release_memory */
    {
        int bytes_to_free = 0;
        if (params_size >= sizeof(int)) {
            memcpy(&bytes_to_free, params, sizeof(int));
        } else if (params_size > 0) {
            bytes_to_free = params[0] * 1024;
        }
        int freed = sqlite3_release_memory(bytes_to_free);
        (void)freed;
    }

    /* Test sqlite3_status */
    {
        int op_codes[] = {
            SQLITE_STATUS_MEMORY_USED,
            SQLITE_STATUS_MALLOC_SIZE,
            SQLITE_STATUS_PAGECACHE_USED,
            SQLITE_STATUS_PAGECACHE_OVERFLOW,
            SQLITE_STATUS_SCRATCH_USED,
            SQLITE_STATUS_SCRATCH_OVERFLOW,
            SQLITE_STATUS_MALLOC_COUNT
        };
        int num_ops = sizeof(op_codes) / sizeof(op_codes[0]);
        
        for (int i = 0; i < num_ops && i < 3; i++) { /* Test first 3 ops */
            int current = 0;
            int highwater = 0;
            int resetFlag = (mode >> i) & 1; /* Use mode bits for reset flags */
            
            int rc = sqlite3_status(op_codes[i], &current, &highwater, resetFlag);
            (void)rc;
        }
    }

    /* Test sqlite3_memory_highwater */
    {
        int resetFlag = mode & 1;
        sqlite3_int64 highwater = sqlite3_memory_highwater(resetFlag);
        (void)highwater;
    }

    /* Test sqlite3_db_release_memory */
    {
        sqlite3 *db = NULL;
        int rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK && db != NULL) {
            /* Create a table and insert some data to have something to release */
            char *errmsg = NULL;
            sqlite3_exec(db, "CREATE TABLE t1(a, b);", NULL, NULL, &errmsg);
            sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2);", NULL, NULL, &errmsg);
            sqlite3_exec(db, "INSERT INTO t1 VALUES(3, 4);", NULL, NULL, &errmsg);
            
            /* Enable tracing to generate some cache activity */
            sqlite3_trace_v2(db, SQLITE_TRACE_STMT, dummy_trace, NULL);
            
            /* Execute a query to populate pager cache */
            sqlite3_exec(db, "SELECT * FROM t1;", NULL, NULL, &errmsg);
            
            /* Now release memory */
            rc = sqlite3_db_release_memory(db);
            (void)rc;
            
            if (errmsg) {
                sqlite3_free(errmsg);
            }
            sqlite3_close(db);
        }
    }

    /* Cleanup */
    sqlite3_shutdown();
    
    return 0;
}
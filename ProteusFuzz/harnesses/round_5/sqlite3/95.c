// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:31088:16 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:31185:17 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* 
 * Fuzz driver for sqlite3 memory management and status APIs.
 * Target APIs:
 * 1. sqlite3_release_memory
 * 2. sqlite3_db_release_memory
 * 3. sqlite3_malloc
 * 4. sqlite3_db_status
 * 5. sqlite3_soft_heap_limit
 * 6. sqlite3_memory_highwater
 */

static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        /* Write some dummy data to ensure the file exists and has content */
        const char *dummy = "SQLite fuzzing dummy file content";
        fwrite(dummy, 1, strlen(dummy), f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Create dummy file if it doesn't exist to allow sqlite3_open to work */
    create_dummy_file();

    /* Initialize variables from input data */
    int release_bytes = (int)(Data[0] * 256 + Data[1]);
    int db_status_op = (Data[2] % 10); /* Limit to reasonable range for SQLITE_DBSTATUS options */
    int reset_flag = (Data[3] % 2);
    int heap_limit = (int)(Data[4] * 256 + Data[5]);
    
    /* Ensure heap_limit isn't negative for the 32-bit API unless testing edge cases */
    if (heap_limit < 0 && Data[4] & 0x80) {
        /* Intentionally pass negative to test defaulting behavior */
    }

    sqlite3 *db = NULL;
    int rc;
    int pCur = 0, pHiwtr = 0;
    sqlite3_int64 hw = 0;
    void *ptr = NULL;

    /* 1. Test sqlite3_release_memory (Global) */
    /* Can be called without a database connection */
    int freed = sqlite3_release_memory(release_bytes);
    (void)freed; /* Suppress unused variable warning */

    /* 2. Test sqlite3_soft_heap_limit (Global) */
    sqlite3_soft_heap_limit(heap_limit);

    /* 3. Test sqlite3_memory_highwater (Global) */
    hw = sqlite3_memory_highwater(0);
    (void)hw;
    hw = sqlite3_memory_highwater(1); /* Reset */
    (void)hw;

    /* 4. Open a database connection to test connection-specific APIs */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK && rc != SQLITE_IOERR) {
        /* If open fails, we can still test global APIs, but skip db-specific ones */
        /* Try to close anyway if db was partially initialized */
        if (db) sqlite3_close(db);
        return 0;
    }

    if (db) {
        /* 5. Test sqlite3_db_release_memory */
        rc = sqlite3_db_release_memory(db);
        (void)rc;

        /* 6. Test sqlite3_db_status */
        /* We use a subset of valid SQLITE_DBSTATUS options to avoid undefined behavior */
        /* SQLITE_DBSTATUS_LOOKASIDE_USED (1), SQLITE_DBSTATUS_CACHE_USED (2), etc. */
        int op = (int)(Data[2] % 5); /* Limit to first few known constants */
        rc = sqlite3_db_status(db, op, &pCur, &pHiwtr, reset_flag);
        (void)rc;
        (void)pCur;
        (void)pHiwtr;

        /* 7. Test sqlite3_malloc with various sizes derived from input */
        if (Size > 6) {
            int alloc_size = (int)(Data[6] * 256 + Data[7]);
            /* Clamp size to prevent OOM or huge allocations in fuzzing */
            if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024;
            
            ptr = sqlite3_malloc(alloc_size);
            if (ptr) {
                /* Write some data to the allocated memory to trigger potential issues */
                memset(ptr, 0xAB, alloc_size);
                /* Free it immediately */
                sqlite3_free(ptr);
                ptr = NULL;
            }

            /* Test realloc with NULL (acts like malloc) */
            ptr = sqlite3_realloc(NULL, alloc_size);
            if (ptr) {
                memset(ptr, 0xCD, alloc_size);
                sqlite3_free(ptr);
                ptr = NULL;
            }

            /* Test realloc with 0 (acts like free) */
            ptr = sqlite3_malloc(alloc_size);
            if (ptr) {
                sqlite3_realloc(ptr, 0); /* Should free */
                ptr = NULL;
            }
        }

        /* Close the database connection */
        sqlite3_close(db);
        db = NULL;
    }

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Callback for sqlite3_busy_handler.
 * Returns 0 to stop retrying, non-zero to continue.
 */
static int dummy_busy_handler(void *arg, int count) {
    /* Fuzzing logic: sometimes retry, sometimes stop based on arg */
    if (arg) {
        unsigned char *p = (unsigned char *)arg;
        return (p[0] % 2) != 0;
    }
    return 0;
}

/*
 * Callback for sqlite3_autovacuum_pages.
 * Returns number of free pages to remove.
 */
static unsigned int dummy_autovacuum_callback(
    void *pClientData,
    const char *zSchema,
    unsigned int nDbPage,
    unsigned int nFreePage,
    unsigned int nBytePerPage) {
    /* Fuzzing logic: return a value based on inputs to explore states */
    if (pClientData) {
        unsigned char *p = (unsigned char *)pClientData;
        unsigned int val = (p[0] * nFreePage) / 256;
        if (val > nFreePage) val = nFreePage;
        return val;
    }
    return nFreePage; /* Default behavior */
}

/*
 * Destructor for autovacuum callback client data.
 */
static void dummy_autovacuum_destructor(void *pClientData) {
    /* No-op for fuzzing, but ensures the function exists */
    if (pClientData) {
        /* Simulate cleanup if needed, but avoid free on fuzzing data */
    }
}

/*
 * Rollback hook callback.
 */
static void dummy_rollback_hook(void *arg) {
    /* No-op */
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Ensure minimum size to avoid trivial crashes */
    if (Size < 2) {
        return 0;
    }

    /* Use a dummy file for database operations */
    const char *db_name = "./dummy_file";

    /* Initialize the database connection */
    rc = sqlite3_open_v2(db_name, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        /* If open fails, we cannot test the target functions */
        return 0;
    }

    /*
     * Fuzzing sqlite3_busy_handler
     * Data[0] controls the behavior of the busy handler
     */
    void *busy_arg = (void *)(uintptr_t)(Data[0]);
    rc = sqlite3_busy_handler(db, dummy_busy_handler, busy_arg);
    (void)rc; /* Ignore return value for fuzzing, just trigger the code path */

    /*
     * Fuzzing sqlite3_wal_autocheckpoint
     * Data[1] controls the threshold N.
     * We map the byte to a range to test different states (positive, zero, negative).
     */
    int wal_threshold = (int)Data[1];
    /* Shift to include negative values */
    if (wal_threshold > 127) {
        wal_threshold -= 256;
    }
    rc = sqlite3_wal_autocheckpoint(db, wal_threshold);
    (void)rc;

    /*
     * Fuzzing sqlite3_rollback_hook
     * Register a dummy hook.
     * Check size before accessing Data[2]
     */
    if (Size > 2) {
        void *old_rollback_arg = sqlite3_rollback_hook(db, dummy_rollback_hook, (void *)(uintptr_t)Data[2]);
        (void)old_rollback_arg;
    }

    /*
     * Fuzzing sqlite3_autovacuum_pages
     * Use Data[3] as client data pointer (casted)
     * Check size before accessing Data[3]
     */
    if (Size > 3) {
        void *autovac_arg = (void *)(uintptr_t)Data[3];
        rc = sqlite3_autovacuum_pages(db, dummy_autovacuum_callback, autovac_arg, dummy_autovacuum_destructor);
        (void)rc;
    }

    /*
     * Fuzzing sqlite3_db_cacheflush
     * This requires a write transaction. We start one.
     */
    sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, NULL, NULL);

    /* Call the target function */
    rc = sqlite3_db_cacheflush(db);
    (void)rc;

    /* Commit the transaction to clean up */
    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

    /*
     * Step 2: Cleanup
     * Close the database connection safely using sqlite3_close_v2
     */
    rc = sqlite3_close_v2(db);
    (void)rc;

    /*
     * Step 3: Explore more states
     * We can try to create a table and insert data to ensure the pager has dirty pages
     * if the previous transaction didn't trigger enough activity.
     */
    if (Size > 8) {
        rc = sqlite3_open_v2(db_name, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (rc == SQLITE_OK && db) {
            /* Create a table if it doesn't exist */
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
            
            /* Insert some data to ensure dirty pages */
            char *sql = sqlite3_mprintf("INSERT INTO fuzz_test (data) VALUES (X'%02X%02X%02X%02X');",
                                        Data[4], Data[5], Data[6], Data[7]);
            if (sql) {
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                sqlite3_free(sql);
            }

            /* Try to flush again */
            sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, NULL, NULL);
            rc = sqlite3_db_cacheflush(db);
            (void)rc;
            sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

            /* Try to configure autocheckpoint with a different value */
            int wal_threshold2 = (int)Data[8];
            if (wal_threshold2 > 127) wal_threshold2 -= 256;
            rc = sqlite3_wal_autocheckpoint(db, wal_threshold2);
            (void)rc;

            /* Close */
            sqlite3_close_v2(db);
        }
    }

    return 0;
}
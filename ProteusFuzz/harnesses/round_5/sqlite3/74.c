// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Static helper function to create a valid database connection.
 * Uses a dummy file to ensure the connection is valid for testing.
 */
static sqlite3* create_test_db(void) {
    sqlite3 *db = NULL;
    int rc;

    /* Remove dummy file if it exists to start fresh */
    remove("./dummy_file");

    /* Open the database */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return NULL;
    }

    /* Ensure WAL mode is enabled for sqlite3_wal_autocheckpoint testing */
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);

    return db;
}

/*
 * Static callback for sqlite3_autovacuum_pages
 */
static unsigned int dummy_autovac_callback(
    void *pClientData,
    const char *zSchema,
    unsigned int nDbPage,
    unsigned int nFreePage,
    unsigned int nBytePerPage
) {
    (void)pClientData;
    (void)zSchema;
    (void)nDbPage;
    (void)nBytePerPage;
    return nFreePage;
}

/*
 * Static destructor for autovacuum pages client data
 */
static void dummy_autovac_destructor(void *p) {
    (void)p;
    if (p) {
        free(p);
    }
}

/*
 * Static callback for sqlite3_rollback_hook
 */
static void dummy_rollback_callback(void *p) {
    (void)p;
    /* Do nothing */
}

/*
 * Static callback for sqlite3_busy_handler
 */
static int dummy_busy_handler(void *p, int count) {
    (void)p;
    (void)count;
    return 0;
}

/*
 * Fuzzer entry point
 */
int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Guard against empty input to avoid trivial loops */
    if (Size == 0) {
        return 0;
    }

    /* Initialize database connection */
    db = create_test_db();
    if (db == NULL) {
        return 0;
    }

    /*
     * Fuzz sqlite3_wal_autocheckpoint
     * Uses input data to determine the N parameter.
     * N > 0 enables, N <= 0 disables.
     */
    if (Size >= 1) {
        int n_val = (int)(Data[0] * 100); /* Scale up to test various thresholds */
        /* Also test negative values */
        if (Size >= 2 && (Data[1] & 1)) {
            n_val = -n_val;
        }
        rc = sqlite3_wal_autocheckpoint(db, n_val);
        (void)rc; /* Ignore return for fuzzing stability, though we could check */
    }

    /*
     * Fuzz sqlite3_autovacuum_pages
     * Registers a callback, then potentially unregisters it.
     */
    void *client_data = NULL;
    if (Size >= 3) {
        /* Allocate dummy client data based on input size */
        size_t alloc_size = (Size % 1024) + 1;
        client_data = malloc(alloc_size);
        if (client_data) {
            memset(client_data, Data[2], alloc_size);
            rc = sqlite3_autovacuum_pages(db, dummy_autovac_callback, client_data, dummy_autovac_destructor);
            (void)rc;
        }
    }

    /*
     * Fuzz sqlite3_rollback_hook
     * Registers and unregisters the hook.
     */
    void *prev_arg = NULL;
    if (Size >= 4) {
        /* Register hook */
        prev_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, (void*)(size_t)(Data[3]));
        (void)prev_arg;

        /* Unregister hook */
        prev_arg = sqlite3_rollback_hook(db, NULL, NULL);
        (void)prev_arg;
    }

    /*
     * Fuzz sqlite3_db_cacheflush
     * Requires a write transaction to be effective.
     */
    if (Size >= 5) {
        /* Start a transaction */
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);

        /* Create a table and insert data to ensure dirty pages */
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS fuzz_table (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
        sqlite3_exec(db, "INSERT INTO fuzz_table VALUES (1, 'test');", NULL, NULL, NULL);

        /* Flush cache */
        rc = sqlite3_db_cacheflush(db);
        (void)rc;

        /* Commit transaction */
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }

    /*
     * Fuzz sqlite3_busy_handler
     * Registers and unregisters the handler.
     */
    if (Size >= 6) {
        int (*prev_handler)(void*, int) = NULL;
        void *prev_arg = NULL;
        
        /* Register busy handler */
        rc = sqlite3_busy_handler(db, dummy_busy_handler, (void*)(size_t)(Data[6]));
        (void)rc;

        /* Unregister busy handler */
        rc = sqlite3_busy_handler(db, NULL, NULL);
        (void)rc;
    }

    /*
     * Fuzz sqlite3_close_v2
     * Safely close the connection.
     */
    rc = sqlite3_close_v2(db);
    (void)rc;

    return 0;
}
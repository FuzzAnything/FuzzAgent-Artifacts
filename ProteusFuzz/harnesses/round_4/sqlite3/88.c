// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 APIs:
 * - sqlite3_db_cacheflush
 * - sqlite3_wal_autocheckpoint
 * - sqlite3_busy_timeout
 * - sqlite3_close_v2
 * - sqlite3_autovacuum_pages
 * - sqlite3_busy_handler
 */

static int dummy_busy_handler(void *arg, int count) {
    (void)arg;
    (void)count;
    return 1; /* Retry */
}

static unsigned int dummy_autovacuum_callback(void *pClientData,
                                              const char *zSchema,
                                              unsigned int nDbPage,
                                              unsigned int nFreePage,
                                              unsigned int nBytePerPage) {
    (void)pClientData;
    (void)zSchema;
    (void)nDbPage;
    (void)nBytePerPage;
    /* Return a value based on free pages to trigger different behaviors */
    return nFreePage;
}

static void dummy_destructor(void *p) {
    (void)p;
    /* No-op destructor */
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *db_name = "./dummy_file";

    /* Initialize SQLite if needed */
    sqlite3_initialize();

    /* Open database */
    rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    /* Helper to safely read input data */
    #define GET_BYTE(idx) ((idx) < (Size) ? (int)Data[(idx)] : 0)

    /* sqlite3_busy_timeout: ms from input (0 to 10000) */
    int ms = GET_BYTE(0) % 10001;
    sqlite3_busy_timeout(db, ms);

    /* sqlite3_wal_autocheckpoint: N from input */
    int n_wal = (GET_BYTE(1) % 2001) - 1000; /* Range: -1000 to 1000 */
    sqlite3_wal_autocheckpoint(db, n_wal);

    /* sqlite3_busy_handler: set with dummy handler or NULL */
    if (GET_BYTE(2) % 2 == 0) {
        sqlite3_busy_handler(db, dummy_busy_handler, (void *)Data);
    } else {
        sqlite3_busy_handler(db, NULL, NULL);
    }

    /* sqlite3_autovacuum_pages: register callback or NULL */
    if (GET_BYTE(3) % 2 == 0) {
        sqlite3_autovacuum_pages(db, dummy_autovacuum_callback, (void *)Data, dummy_destructor);
    } else {
        sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
    }

    /* Prepare a simple statement to potentially create a write transaction */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, val TEXT);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Begin a transaction to test sqlite3_db_cacheflush */
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        /* Insert some data to make pages dirty */
        rc = sqlite3_exec(db, "INSERT OR IGNORE INTO test VALUES (1, 'fuzz_data');", NULL, NULL, NULL);
        
        /* Flush caches mid-transaction */
        sqlite3_db_cacheflush(db);
        
        /* Commit the transaction */
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }

    /* Close the database connection safely */
    sqlite3_close_v2(db);

    /* Cleanup */
    sqlite3_shutdown();

    return 0;
}
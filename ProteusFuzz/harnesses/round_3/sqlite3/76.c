// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to initialize the database connection.
 * Uses an in-memory database to avoid file system dependencies.
 */
static sqlite3 *init_db(void) {
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    
    /* Create a simple table to ensure the database schema exists */
    const char *sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NULL;
    }
    
    return db;
}

/*
 * Static autovacuum callback for fuzzing.
 * Returns a value based on input to trigger different code paths.
 */
static unsigned int fuzz_autovac_callback(void *pClientData, const char *zSchema,
                                          unsigned int nDbPage, unsigned int nFreePage,
                                          unsigned int nBytePerPage) {
    /* 
     * Use client data as a seed for decision making if available.
     * Otherwise, return nFreePage to trigger full autovacuum or 0 to skip.
     */
    if (pClientData) {
        unsigned int seed = *(unsigned int *)pClientData;
        if (seed % 2 == 0) {
            return nFreePage; /* Full autovacuum */
        } else {
            return 0; /* No autovacuum */
        }
    }
    return nFreePage;
}

/*
 * Static destructor for autovacuum client data.
 */
static void fuzz_autovac_destructor(void *pClientData) {
    if (pClientData) {
        free(pClientData);
    }
}

/*
 * Static rollback hook callback.
 */
static void fuzz_rollback_callback(void *pArg) {
    /* Do nothing, just a placeholder to register the hook */
    (void)pArg;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = init_db();
    if (!db) {
        return 0;
    }

    /*
     * Use input data to drive fuzzing logic.
     * We interpret parts of the input to make decisions.
     */
    unsigned int seed = 0;
    if (Size >= sizeof(unsigned int)) {
        memcpy(&seed, Data, sizeof(unsigned int));
    }

    /*
     * 1. Test sqlite3_autovacuum_pages
     * Register a callback with varying parameters.
     */
    void *client_data = NULL;
    /* Ensure we have enough data before allocating and copying */
    if (Size > sizeof(unsigned int)) {
        client_data = malloc(sizeof(unsigned int));
        if (client_data) {
            memcpy(client_data, Data + sizeof(unsigned int), sizeof(unsigned int));
        }
    }

    int ret_autovac = sqlite3_autovacuum_pages(
        db,
        fuzz_autovac_callback,
        client_data,
        fuzz_autovac_destructor
    );
    (void)ret_autovac; /* Suppress unused warning */

    /*
     * 2. Test sqlite3_rollback_hook
     * Register and unregister the hook.
     */
    void *prev_rollback_arg = sqlite3_rollback_hook(db, fuzz_rollback_callback, (void *)(uintptr_t)seed);
    (void)prev_rollback_arg;
    
    /* Unregister the hook */
    sqlite3_rollback_hook(db, NULL, NULL);

    /*
     * 3. Test sqlite3_wal_autocheckpoint
     * Use input to determine checkpoint threshold.
     */
    int checkpoint_n = 1000;
    /* Ensure we have enough data before reading checkpoint_n */
    if (Size > sizeof(unsigned int) + sizeof(int)) {
        memcpy(&checkpoint_n, Data + sizeof(unsigned int) + sizeof(int), sizeof(int));
    }
    int ret_wal = sqlite3_wal_autocheckpoint(db, checkpoint_n);
    (void)ret_wal;

    /*
     * 4. Test sqlite3_txn_state
     * Check transaction state for main and temp schemas.
     */
    int state_main = sqlite3_txn_state(db, "main");
    int state_temp = sqlite3_txn_state(db, "temp");
    int state_all = sqlite3_txn_state(db, NULL);
    int state_invalid = sqlite3_txn_state(db, "nonexistent_schema");
    (void)state_main;
    (void)state_temp;
    (void)state_all;
    (void)state_invalid;

    /*
     * 5. Test sqlite3_db_cacheflush
     * This requires a write transaction.
     */
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql = "INSERT INTO test_table (data) VALUES ('fuzz_data');";
    
    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, NULL, NULL) == SQLITE_OK) {
        /* Execute some writes */
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }

        /* Now flush the cache */
        int ret_flush = sqlite3_db_cacheflush(db);
        (void)ret_flush;

        /* Commit or rollback to end transaction */
        if (seed % 2 == 0) {
            sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        } else {
            sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        }
    }

    /*
     * 6. Test sqlite3_close_v2
     * Gracefully close the connection.
     */
    sqlite3_close_v2(db);

    /* Cleanup client data if allocated */
    if (client_data) {
        free(client_data);
    }

    return 0;
}
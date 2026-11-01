// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sqlite3.h>

/*
 * Static helper to create a temporary database file for fuzzing.
 * Returns 0 on success, -1 on failure.
 */
static int create_dummy_db(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    /* Write some dummy data to ensure file exists and is writable */
    const char *dummy = "SQLite fuzzing dummy file content";
    size_t written = fwrite(dummy, 1, strlen(dummy), f);
    fclose(f);
    return (written > 0) ? 0 : -1;
}

/*
 * Static callback for rollback hook.
 */
static void dummy_rollback_callback(void *arg) {
    /* Do nothing */
    (void)arg;
}

/*
 * Static callback for autovacuum pages.
 */
static unsigned int dummy_autovacuum_callback(
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
    /* Return number of free pages to vacuum (0 to disable, nFreePage to full) */
    return nFreePage;
}

/*
 * Static destructor for autovacuum client data.
 */
static void dummy_autovacuum_destructor(void *arg) {
    (void)arg;
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Create a dummy file for the database */
    const char *db_path = "./dummy_file";
    if (create_dummy_db(db_path) != 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc = sqlite3_open_v2(db_path, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * Extract parameters from the fuzzing input.
     * We use the first few bytes to determine behavior.
     */
    int checkpoint_threshold = (int)Data[0];
    int schema_index = Data[1];
    
    /* Prepare a schema name string based on input */
    const char *schema_name = NULL;
    if (schema_index == 0) {
        schema_name = "main";
    } else if (schema_index == 1) {
        schema_name = "temp";
    } else if (schema_index == 2) {
        schema_name = "invalid_schema_name";
    } else {
        /* Use a pointer into the fuzz data if it's a valid string */
        if (Size > 2 && Data[2] != 0) {
            schema_name = (const char *)&Data[2];
        }
    }

    /*
     * 1. Test sqlite3_wal_autocheckpoint
     * Pass various values for N (positive, zero, negative)
     */
    int wal_rc = sqlite3_wal_autocheckpoint(db, checkpoint_threshold);
    (void)wal_rc;

    /*
     * 2. Test sqlite3_txn_state
     * Check transaction state for different schemas
     */
    int txn_state = sqlite3_txn_state(db, schema_name);
    (void)txn_state;

    /*
     * 3. Test sqlite3_db_cacheflush
     * This requires a write transaction to be effective, but we call it anyway
     * to test error handling and state checks.
     */
    int flush_rc = sqlite3_db_cacheflush(db);
    (void)flush_rc;

    /*
     * 4. Test sqlite3_rollback_hook
     * Register a callback, then unregister it.
     */
    void *prev_rollback_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, (void*)Data);
    (void)prev_rollback_arg;
    
    /* Unregister the hook */
    sqlite3_rollback_hook(db, NULL, NULL);

    /*
     * 5. Test sqlite3_autovacuum_pages
     * Register a callback, then unregister it.
     */
    int autovac_rc = sqlite3_autovacuum_pages(
        db, 
        dummy_autovacuum_callback, 
        (void*)Data, 
        dummy_autovacuum_destructor
    );
    (void)autovac_rc;

    /* Unregister the autovacuum callback */
    sqlite3_autovacuum_pages(db, NULL, NULL, NULL);

    /*
     * 6. Test sqlite3_close_v2
     * This should safely close the connection.
     */
    sqlite3_close_v2(db);
    db = NULL;

    /* Clean up the dummy file */
    unlink(db_path);

    return 0;
}
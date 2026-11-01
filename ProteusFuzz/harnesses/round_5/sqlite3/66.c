// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:189478:18 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper functions to avoid name conflicts.
 */

/*
 * A simple destructor for client data used in fuzzing.
 * It just frees the memory if it was allocated.
 */
static void client_data_destructor(void *pData) {
    if (pData) {
        free(pData);
    }
}

/*
 * A simple autovacuum pages callback.
 * It returns a value based on the inputs to exercise different code paths.
 * Note: The callback must not call other SQLite APIs.
 */
static unsigned int autovacuum_callback(
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

    /*
     * Return 0 to disable autovacuum, or nFreePage to vacuum all.
     * We can also return a value between 0 and nFreePage.
     * To explore states, we can use the nFreePage value directly or 0.
     */
    return nFreePage;
}

/*
 * A destructor for the autovacuum callback client data.
 * IMPORTANT: Do NOT free the data here if we plan to free it manually later.
 * SQLite will call this destructor when the callback is overridden or the DB closes.
 * If we free it here, we must NOT free it again in the fuzzer code.
 * To avoid double-free, we will use a flag or a special allocator, or simply
 * let SQLite manage the memory completely.
 * For this fix, we will let SQLite manage the memory completely.
 * We will allocate memory, pass it to SQLite, and let SQLite's destructor free it.
 * We will NOT free it manually in the fuzzer code after passing to SQLite.
 */
static void autovacuum_destructor(void *pData) {
    if (pData) {
        free(pData);
    }
}

/*
 * Main fuzzing entry point.
 */
int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * Create a dummy file for the database.
     * We write some data to it to ensure it exists and has content.
     */
    const char *db_filename = "./dummy_file";
    FILE *f = fopen(db_filename, "wb");
    if (f) {
        /* Write up to Size bytes or a fixed amount to create a file */
        size_t write_size = (Size < 1024) ? Size : 1024;
        fwrite(Data, 1, write_size, f);
        fclose(f);
    }

    sqlite3 *db = NULL;
    int rc;

    /*
     * Open the database.
     * We use SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE to allow creation/modification.
     */
    rc = sqlite3_open_v2(db_filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        /* If opening fails, we can't test much, but we should clean up the file if created. */
        if (db) {
            sqlite3_close_v2(db);
        }
        remove(db_filename);
        return 0;
    }

    /*
     * Test sqlite3_db_config.
     * We try different operations. SQLITE_DBCONFIG_LOOKASIDE is a common one.
     */
    if (Size >= 10) {
        /* Try to configure lookaside memory */
        void *lookaside_buf = malloc(Size);
        if (lookaside_buf) {
            memcpy(lookaside_buf, Data, Size);
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, lookaside_buf, Size, Size);
            free(lookaside_buf);
        }
    }

    /*
     * Test sqlite3_set_clientdata and sqlite3_get_clientdata.
     * We use a fixed key "fuzz_key" and store some data.
     */
    if (Size > 0) {
        void *client_data = malloc(Size);
        if (client_data) {
            memcpy(client_data, Data, Size);
            rc = sqlite3_set_clientdata(db, "fuzz_key", client_data, client_data_destructor);
            if (rc == SQLITE_OK) {
                void *retrieved = sqlite3_get_clientdata(db, "fuzz_key");
                (void)retrieved; /* Use the retrieved data if needed, or just check it's not NULL */
                
                /* Try to set it again to trigger the destructor */
                void *new_client_data = malloc(Size);
                if (new_client_data) {
                    memcpy(new_client_data, Data, Size);
                    rc = sqlite3_set_clientdata(db, "fuzz_key", new_client_data, client_data_destructor);
                    /* Do NOT free new_client_data here. SQLite's destructor will free it. */
                }
            }
        }
    }

    /*
     * Test sqlite3_autovacuum_pages.
     * We register a callback and then unregister it.
     * IMPORTANT: We must NOT free the data passed to sqlite3_autovacuum_pages manually
     * if we provide a destructor that frees it. SQLite will call the destructor
     * when the callback is overridden or the DB closes.
     */
    if (Size > 0) {
        void *autovac_data = malloc(Size);
        if (autovac_data) {
            memcpy(autovac_data, Data, Size);
            /* Register the callback. SQLite takes ownership of autovac_data if destructor is provided. */
            rc = sqlite3_autovacuum_pages(db, autovacuum_callback, autovac_data, autovacuum_destructor);
            /* Do NOT free(autovac_data) here. It will be freed by autovacuum_destructor when overridden or DB closes. */
        }
        
        /* Try to unregister the callback by passing NULL. This triggers the destructor for the previous callback. */
        rc = sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
    }

    /*
     * Try to execute a simple SQL statement to trigger autovacuum if possible.
     */
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /*
     * Clean up: close the database connection.
     * This should trigger any remaining destructors.
     */
    rc = sqlite3_close_v2(db);

    /*
     * Clean up the dummy file.
     */
    remove(db_filename);

    return 0;
}
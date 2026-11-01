// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:189478:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to safely close a database connection.
 */
static void safe_close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close_v2(db);
    }
}

/*
 * Callback function for sqlite3_autovacuum_pages.
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
    return nFreePage;
}

/*
 * Destructor for client data.
 * Note: We do not free memory here to avoid double-free issues if the caller
 * also attempts to free the pointer after retrieval.
 */
static void dummy_clientdata_destructor(void *p) {
    (void)p;
    /* No-op destructor to prevent double-free logic errors in fuzzing */
}

/*
 * Destructor for autovacuum pages callback data.
 */
static void dummy_autovacuum_destructor(void *p) {
    (void)p;
    /* No-op destructor */
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";

    /* Step 1: Prepare environment */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        flags = (Data[0] & 0x07) | SQLITE_OPEN_CREATE; 
        if (flags == 0) flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }

    rc = sqlite3_open_v2(filename, &db, flags, NULL);

    if (rc != SQLITE_OK && db != NULL) {
        safe_close_db(db);
        db = NULL;
        return 0;
    }

    if (db == NULL) {
        return 0;
    }

    /* Step 3: Explore program states */

    /* Test sqlite3_set_clientdata and sqlite3_get_clientdata */
    if (Size >= 1) {
        char key[32];
        size_t key_len = (Size > 31) ? 31 : Size;
        memcpy(key, Data, key_len);
        key[key_len] = '\0';

        void *test_data = malloc(64);
        if (test_data) {
            memset(test_data, 0x42, 64);
            /* Register data with a destructor that does NOT free memory */
            rc = sqlite3_set_clientdata(db, key, test_data, dummy_clientdata_destructor);
            (void)rc;

            void *retrieved = sqlite3_get_clientdata(db, key);
            if (retrieved) {
                /* 
                 * We retrieved the pointer. Since the destructor does not free,
                 * we are responsible for freeing it here if we want to clean up.
                 * However, if we call set_clientdata again with NULL, the destructor
                 * will be called. To avoid double-free, we simply do not free here
                 * and rely on the destructor or the final cleanup.
                 * 
                 * Strategy: Do NOT free 'retrieved' here. Let the destructor handle it
                 * if we overwrite, or let the final close handle it.
                 * But wait, our destructor is a no-op. So we MUST free it here to avoid leak,
                 * OR we must ensure we don't free it twice.
                 * 
                 * Correct approach for fuzzing:
                 * 1. Set data.
                 * 2. Get data.
                 * 3. Do NOT free 'retrieved' immediately if we plan to overwrite/remove later
                 *    and the destructor is active.
                 * 4. Since our destructor is a no-op, we can safely free 'retrieved' here
                 *    ONLY IF we don't free it again later.
                 * 
                 * Let's change strategy: The destructor is a no-op. We free the memory
                 * immediately after retrieval to avoid leaks, and then we remove the
                 * client data entry (which calls the no-op destructor).
                 */
                free(retrieved);
            }

            /* Remove client data. This calls the destructor (no-op). */
            sqlite3_set_clientdata(db, key, NULL, NULL);
            
            /* Free the original allocation since we didn't free it via retrieved 
               (if retrieved was NULL) or we freed it above.
               Wait, if we freed 'retrieved' above, 'test_data' is now a dangling pointer
               if we try to free it again.
               
               Logic correction:
               'test_data' is the pointer we passed. 'retrieved' is the same pointer.
               If we free 'retrieved', we have freed the memory.
               We should NOT free 'test_data' again.
            */
            /* We already freed 'retrieved' (which is 'test_data') above. */
        }
    }

    /* Test sqlite3_db_config with SQLITE_DBCONFIG_LOOKASIDE */
    if (Size >= 3) {
        int sz = Data[0] + 1; 
        int cnt = Data[1];
        int op = Data[2];

        if (op == SQLITE_DBCONFIG_LOOKASIDE) {
            if (sz < sizeof(void*)) sz = sizeof(void*);
            sz = (sz / sizeof(void*)) * sizeof(void*);
            if (cnt < 0) cnt = 0;

            void *lookaside_buf = malloc(sz * cnt);
            if (lookaside_buf) {
                memset(lookaside_buf, 0, sz * cnt);
                rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, lookaside_buf, sz, cnt);
                (void)rc;
                free(lookaside_buf);
            }
        }
    }

    /* Test sqlite3_autovacuum_pages */
    if (Size >= 1) {
        void *client_data = malloc(16);
        if (client_data) {
            memset(client_data, 0, 16);
            rc = sqlite3_autovacuum_pages(db, dummy_autovacuum_callback, client_data, dummy_autovacuum_destructor);
            (void)rc;
            
            rc = sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
            (void)rc;
            
            /* The destructor (dummy_autovacuum_destructor) is a no-op.
             * We must free the memory here to prevent leaks.
             */
            free(client_data);
        }
    }

    /* Step 4: Cleanup */
    safe_close_db(db);

    return 0;
}
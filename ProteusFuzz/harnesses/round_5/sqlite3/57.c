// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
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
 * Static helper to create a dummy database file for testing.
 * Uses "./dummy_file" as the filename.
 */
static int create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (!f) {
        return 0;
    }
    /* Write some dummy data to ensure the file exists and is not empty */
    const char *dummy = "SQLite dummy data for fuzzing";
    fwrite(dummy, 1, strlen(dummy), f);
    fclose(f);
    return 1;
}

/*
 * Static callback for sqlite3_autovacuum_pages.
 * Returns the number of free pages to vacuum.
 * We use simple arithmetic based on input to avoid reentrancy issues.
 */
static unsigned int dummy_autovac_callback(
    void *pClientData,
    const char *zSchema,
    unsigned int nDbPage,
    unsigned int nFreePage,
    unsigned int nBytePerPage
) {
    /* Simple logic: if we have data, return half the free pages, else 0 */
    if (nFreePage > 0) {
        return nFreePage / 2;
    }
    return 0;
}

/*
 * Static destructor for autovacuum callback client data.
 */
static void dummy_autovac_destructor(void *pClientData) {
    if (pClientData) {
        free(pClientData);
    }
}

/*
 * Static callback for sqlite3_busy_handler.
 * Returns 1 to retry once, 0 to stop.
 */
static int dummy_busy_handler(void *pArg, int count) {
    /* Retry only once to avoid infinite loops in fuzzing */
    if (count < 1) {
        return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Ensure we have enough data to extract parameters */
    if (Size < 4) {
        return 0;
    }

    /* Create a dummy file to open */
    if (!create_dummy_file()) {
        return 0;
    }

    /* Open the database */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Extract parameters from fuzz input */
    int N = (int)(Data[0]);
    int busy_retry = (int)(Data[1]);
    int enable_autovac = (int)(Data[2]);
    int flush_cache = (int)(Data[3]);

    /* 1. Test sqlite3_wal_autocheckpoint */
    /* N > 0 enables, N <= 0 disables */
    rc = sqlite3_wal_autocheckpoint(db, N);
    (void)rc; /* Ignore return for fuzzing, just exercise the path */

    /* 2. Test sqlite3_busy_handler */
    /* If busy_retry is non-zero, register the handler */
    if (busy_retry != 0) {
        rc = sqlite3_busy_handler(db, dummy_busy_handler, (void*)(size_t)Data[0]);
    } else {
        /* Disable busy handler */
        rc = sqlite3_busy_handler(db, NULL, NULL);
    }
    (void)rc;

    /* 3. Test sqlite3_autovacuum_pages */
    void *client_data = NULL;
    if (enable_autovac != 0) {
        /* Allocate some dummy client data */
        client_data = malloc(64);
        if (client_data) {
            memset(client_data, 0, 64);
            rc = sqlite3_autovacuum_pages(
                db,
                dummy_autovac_callback,
                client_data,
                dummy_autovac_destructor
            );
        }
    } else {
        /* Disable autovacuum callback */
        rc = sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
    }
    (void)rc;

    /* 4. Test sqlite3_db_cacheflush */
    /* This requires a write transaction to have an effect, but we can still call it */
    if (flush_cache != 0) {
        rc = sqlite3_db_cacheflush(db);
        (void)rc;
    }

    /* 5. Test sqlite3_errcode */
    /* Retrieve the error code, which might be set by previous operations */
    int err = sqlite3_errcode(db);
    (void)err;

    /* 6. Close the database connection safely */
    rc = sqlite3_close_v2(db);
    if (rc != SQLITE_OK && rc != SQLITE_BUSY) {
        /* Unexpected error during close, but we continue */
    }

    /* Cleanup: remove the dummy file */
    remove("./dummy_file");

    return 0;
}
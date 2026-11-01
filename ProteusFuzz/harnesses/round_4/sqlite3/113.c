// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
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
 * Static helper function to safely close a database connection.
 */
static void safe_close(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Static helper function to create a dummy file for testing.
 */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        const char *data = "SQLite fuzzing dummy data for testing purposes.";
        fwrite(data, 1, strlen(data), f);
        fclose(f);
    }
}

/*
 * Progress callback for sqlite3_progress_handler.
 * Returns 0 to continue, non-zero to abort.
 */
static int progress_callback(void *arg) {
    /* 
     * Fuzzing strategy: Randomly decide whether to abort or continue.
     * We cast arg to an integer to make use of fuzz input if passed,
     * but here we just return a deterministic value or 0.
     * To explore states, we could return 1 occasionally.
     */
    if (arg) {
        int *val = (int *)arg;
        if (*val > 100) return 1; /* Abort */
    }
    return 0; /* Continue */
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int ms;
    int flags;
    int progress_n;
    int sleep_ms;
    int setlk_flags;

    /* Ensure we have enough data to extract parameters */
    if (Size < 4) {
        return 0;
    }

    /* Extract parameters from fuzz input */
    ms = (int)(Data[0] << 8) | Data[1];
    flags = (int)(Data[2] << 8) | Data[3];
    
    /* Normalize values to reasonable ranges for fuzzing */
    if (ms < 0) ms = 0;
    if (ms > 10000) ms = 10000; /* Cap timeout to avoid hanging fuzzer */
    
    progress_n = (Size > 4) ? (int)(Data[4]) : 100;
    sleep_ms = (Size > 5) ? (int)(Data[5]) : 10;
    setlk_flags = (Size > 6) ? (int)(Data[6]) : 0;

    /* Create dummy file if needed for file-based tests */
    create_dummy_file();

    /* 
     * Step 1: Open database connection
     * Use flags from input to vary open modes (READONLY, READWRITE, CREATE, etc.)
     * We mask flags to ensure they are valid SQLite flags.
     */
    int open_flags = flags & (SQLITE_OPEN_READWRITE | SQLITE_OPEN_READONLY | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI);
    
    rc = sqlite3_open_v2("./dummy_file", &db, open_flags, NULL);
    
    if (rc != SQLITE_OK && rc != SQLITE_BUSY) {
        /* If we can't open, we can't test the other APIs that require a connection.
         * However, we can still test sqlite3_sleep and sqlite3_errmsg (with NULL).
         */
        const char *err = sqlite3_errmsg(NULL);
        (void)err; /* Suppress unused warning */
        
        /* Test sqlite3_sleep with fuzzed input */
        if (sleep_ms > 0) {
            sqlite3_sleep(sleep_ms);
        } else {
            sqlite3_sleep(0);
        }
        return 0;
    }

    /* Step 2: Test sqlite3_progress_handler */
    /* Enable progress handler with fuzzed N */
    if (progress_n > 0) {
        sqlite3_progress_handler(db, progress_n, progress_callback, (void *)&progress_n);
    } else {
        /* Disable progress handler */
        sqlite3_progress_handler(db, -1, NULL, NULL);
    }

    /* Step 3: Test sqlite3_busy_timeout */
    /* Set busy timeout. This replaces any existing busy handler. */
    rc = sqlite3_busy_timeout(db, ms);
    (void)rc;

    /* Step 4: Test sqlite3_setlk_timeout */
    /* Set lock timeout for WAL mode. */
    /* Note: This is a no-op if SQLITE_ENABLE_SETLK_TIMEOUT is not defined, 
     * but calling it is safe. */
    rc = sqlite3_setlk_timeout(db, ms, setlk_flags);
    (void)rc;

    /* Step 5: Test sqlite3_sleep */
    /* Sleep for a short duration to exercise the VFS sleep implementation */
    if (sleep_ms > 0) {
        sqlite3_sleep(sleep_ms);
    }

    /* Step 6: Test sqlite3_errmsg */
    /* Retrieve error message. If no error, returns NULL or empty string depending on state. */
    const char *errmsg = sqlite3_errmsg(db);
    (void)errmsg;

    /* Step 7: Explore more states by preparing a simple statement */
    /* This might trigger the progress handler if N is small enough */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 1;";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        (void)rc;
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    safe_close(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Dummy extension entry point for sqlite3_auto_extension testing.
 * It does nothing and returns SQLITE_OK.
 */
static int dummy_extension_entry(sqlite3 *db, const char **pzErrMsg, const struct sqlite3_api_routines *pThunk) {
    (void)db;
    (void)pzErrMsg;
    (void)pThunk;
    return SQLITE_OK;
}

/*
 * Wrapper for the dummy extension to match the void (*)(void) signature required by sqlite3_auto_extension.
 */
static void dummy_extension_wrapper(void) {
    /* No-op, the actual logic is in dummy_extension_entry which is called by SQLite internally */
}

/*
 * Dummy log callback for SQLITE_CONFIG_LOG
 */
static void dummy_log_callback(void *pCtx, int iErrCode, const char *zMsg) {
    (void)pCtx;
    (void)iErrCode;
    (void)zMsg;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    int rc;
    int threadsafe;
    
    /* Ensure we have some data to work with */
    if (Size == 0) {
        return 0;
    }

    /* 
     * Strategy:
     * 1. Initialize SQLite.
     * 2. Try various configurations (sqlite3_config).
     * 3. Register and unregister auto-extensions.
     * 4. Check thread safety.
     * 5. Shutdown and re-initialize to test state transitions.
     */

    /* 
     * Use the first byte to determine an operation mode to diversify state exploration.
     * 0: Basic init/shutdown cycle
     * 1: Config before init (should be safe if not initialized yet, but we already initialized above)
     * 2: Config after shutdown
     * 3: Extension registration
     * 4: Extension cancellation
     * 5: Mixed operations
     */
    int mode = Data[0] % 6;

    /* 
     * Attempt to initialize SQLite.
     * Note: sqlite3_initialize is idempotent.
     */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        /* Initialization failed, try to recover or exit */
        return 0;
    }

    /* 
     * Check thread safety status.
     * This is a read-only operation and safe to call anytime.
     */
    threadsafe = sqlite3_threadsafe();
    (void)threadsafe; /* Suppress unused warning */

    /* 
     * Perform operations based on the selected mode.
     */
    switch (mode) {
        case 0:
            /* Basic cycle: Init -> Shutdown */
            rc = sqlite3_shutdown();
            if (rc != SQLITE_OK) {
                /* Shutdown failed, but we don't crash, just return */
            }
            break;

        case 1:
            /* Config before init (should be safe if not initialized yet, but we already initialized above)
             * Since we initialized above, this might trigger SQLITE_MISUSE for non-anytime options.
             * We use SQLITE_CONFIG_SINGLETHREAD as a test case.
             */
            rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            (void)rc;
            break;

        case 2:
            /* Shutdown then Config */
            rc = sqlite3_shutdown();
            if (rc == SQLITE_OK) {
                /* Try to config after shutdown */
                rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                (void)rc;
            }
            break;

        case 3:
            /* Register auto-extension */
            rc = sqlite3_auto_extension(dummy_extension_wrapper);
            (void)rc;
            break;

        case 4:
            /* Cancel auto-extension */
            rc = sqlite3_cancel_auto_extension(dummy_extension_wrapper);
            (void)rc;
            break;

        case 5:
            /* Mixed operations */
            rc = sqlite3_auto_extension(dummy_extension_wrapper);
            (void)rc;
            
            /* Try to cancel immediately */
            rc = sqlite3_cancel_auto_extension(dummy_extension_wrapper);
            (void)rc;

            /* Shutdown */
            rc = sqlite3_shutdown();
            if (rc == SQLITE_OK) {
                /* Re-init */
                rc = sqlite3_initialize();
                (void)rc;
            }
            break;
    }

    /* 
     * Additional fuzzing: Use remaining bytes to generate random config options if available.
     * We only try a few safe or common options to avoid undefined behavior.
     * Note: sqlite3_config is not threadsafe and requires specific timing for most options.
     * We will only attempt SQLITE_CONFIG_LOG which is an "anytime" option.
     * SQLITE_CONFIG_GETLOG was removed as it is not a standard macro in all versions.
     */
    if (Size > 1) {
        int log_op = Data[1] % 2;
        if (log_op == 0) {
            /* SQLITE_CONFIG_LOG */
            rc = sqlite3_config(SQLITE_CONFIG_LOG, dummy_log_callback, NULL);
            (void)rc;
        } else {
            /* SQLITE_CONFIG_LOG with NULL callback to reset logging */
            rc = sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
            (void)rc;
        }
    }

    /* 
     * Final cleanup: Ensure we shutdown if we are in a state where it makes sense.
     * However, since the fuzzer runs many iterations, we might want to leave it initialized
     * or shutdown depending on the mode. 
     * To be safe and avoid resource leaks across iterations, we attempt a shutdown.
     * If it fails (e.g., due to misuse), we ignore it.
     */
    rc = sqlite3_shutdown();
    (void)rc;

    return 0;
}
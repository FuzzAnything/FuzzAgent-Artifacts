// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Dummy extension entry point for testing auto-extension APIs */
static void dummy_extension_entry_point(void) {
    /* Intentionally empty for fuzzing purposes */
}

/* Another dummy extension to test multiple registrations */
static void dummy_extension_entry_point_2(void) {
    /* Intentionally empty for fuzzing purposes */
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    int rc;
    int threadsafe_mode;
    int cancel_result;
    int config_result;
    int init_result;
    int shutdown_result;

    /* Ensure we have some data to work with, though these APIs don't strictly need it */
    if (Size == 0) {
        return 0;
    }

    /* 
     * Strategy: 
     * 1. Call sqlite3_threadsafe() anytime to check compile-time settings.
     * 2. Call sqlite3_config() before initialization (safe zone).
     * 3. Call sqlite3_initialize() to start the library.
     * 4. Call sqlite3_auto_extension() (safe anytime).
     * 5. Call sqlite3_cancel_auto_extension() (safe anytime).
     * 6. Call sqlite3_shutdown() to clean up.
     * 7. Repeat cycles to test idempotency and state transitions.
     */

    /* Step 1: Check threadsafe status (safe anytime) */
    threadsafe_mode = sqlite3_threadsafe();
    (void)threadsafe_mode; /* Suppress unused variable warning */

    /* Step 2: Try to configure before init (safe zone) */
    /* We use a valid config option that is "anytime" safe or pre-init safe */
    /* SQLITE_CONFIG_SINGLETHREAD is valid pre-init */
    config_result = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    (void)config_result;

    /* Step 3: Initialize the library */
    init_result = sqlite3_initialize();
    if (init_result != SQLITE_OK) {
        /* If init fails, we can't do much else, but we can still test shutdown */
        goto cleanup;
    }

    /* Step 4: Register auto-extensions (safe anytime) */
    rc = sqlite3_auto_extension(dummy_extension_entry_point);
    (void)rc;
    
    rc = sqlite3_auto_extension(dummy_extension_entry_point_2);
    (void)rc;

    /* Register the same one again to test idempotency */
    rc = sqlite3_auto_extension(dummy_extension_entry_point);
    (void)rc;

    /* Step 5: Cancel auto-extensions */
    cancel_result = sqlite3_cancel_auto_extension(dummy_extension_entry_point);
    (void)cancel_result;

    cancel_result = sqlite3_cancel_auto_extension(dummy_extension_entry_point_2);
    (void)cancel_result;

    /* Try to cancel a non-existent extension */
    cancel_result = sqlite3_cancel_auto_extension(dummy_extension_entry_point);
    (void)cancel_result;

    /* Step 6: Shutdown the library */
    shutdown_result = sqlite3_shutdown();
    (void)shutdown_result;

cleanup:
    /* Step 7: Test idempotency of shutdown (calling again should be safe) */
    shutdown_result = sqlite3_shutdown();
    (void)shutdown_result;

    /* Step 8: Re-initialize to test state recovery */
    init_result = sqlite3_initialize();
    if (init_result == SQLITE_OK) {
        /* Try config again after re-init (might fail if not anytime option) */
        config_result = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
        (void)config_result;

        /* Final shutdown */
        shutdown_result = sqlite3_shutdown();
        (void)shutdown_result;
    }

    return 0;
}
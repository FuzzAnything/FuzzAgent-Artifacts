// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
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

/* Dummy extension entry point for testing sqlite3_auto_extension */
static void dummy_extension_entry_point(void) {
    /* Do nothing */
}

/* Another dummy extension entry point */
static void dummy_extension_entry_point_2(void) {
    /* Do nothing */
}

/* Fuzzing entry point */
int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    int rc;
    int threadsafe;

    /* 
     * Strategy:
     * 1. Attempt to configure SQLite before initialization (valid).
     * 2. Initialize SQLite.
     * 3. Attempt to configure SQLite after initialization (should return SQLITE_MISUSE for most options).
     * 4. Register auto-extensions.
     * 5. Check threadsafe status.
     * 6. Shutdown SQLite.
     * 7. Attempt to configure again (valid).
     * 8. Re-initialize and repeat cycles to explore state transitions.
     */

    /* Phase 1: Pre-initialization configuration */
    /* We use the data to select a config option if Size is large enough, otherwise default */
    int config_opt = SQLITE_CONFIG_SINGLETHREAD;
    if (Size > 0) {
        config_opt = Data[0] % 10; /* Map to a small range of config options */
        if (config_opt == 0) config_opt = SQLITE_CONFIG_SINGLETHREAD;
        else if (config_opt == 1) config_opt = SQLITE_CONFIG_MULTITHREAD;
        else if (config_opt == 2) config_opt = SQLITE_CONFIG_SERIALIZED;
        else if (config_opt == 3) config_opt = SQLITE_CONFIG_LOG;
        else if (config_opt == 4) config_opt = SQLITE_CONFIG_URI;
        else if (config_opt == 5) config_opt = SQLITE_CONFIG_SQLLOG;
        else if (config_opt == 6) config_opt = SQLITE_CONFIG_MMAP_SIZE;
        else if (config_opt == 7) config_opt = SQLITE_CONFIG_WIN32_HEAPSIZE;
        else if (config_opt == 8) config_opt = SQLITE_CONFIG_PCACHE;
        else if (config_opt == 9) config_opt = SQLITE_CONFIG_GETPCACHE;
    }

    rc = sqlite3_config(config_opt);
    (void)rc;

    /* Phase 2: Initialize */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        /* Initialization failed, try to continue with shutdown to clean up if possible */
        sqlite3_shutdown();
        return 0;
    }

    /* Phase 3: Post-initialization configuration (should fail for most options) */
    rc = sqlite3_config(config_opt);
    (void)rc;

    /* Phase 4: Register auto-extensions */
    rc = sqlite3_auto_extension(dummy_extension_entry_point);
    (void)rc;
    
    rc = sqlite3_auto_extension(dummy_extension_entry_point_2);
    (void)rc;

    /* Try to cancel an extension */
    rc = sqlite3_cancel_auto_extension(dummy_extension_entry_point);
    (void)rc;

    /* Phase 5: Check threadsafe */
    threadsafe = sqlite3_threadsafe();
    (void)threadsafe;

    /* Phase 6: Shutdown */
    rc = sqlite3_shutdown();
    (void)rc;

    /* Phase 7: Post-shutdown configuration */
    rc = sqlite3_config(config_opt);
    (void)rc;

    /* Phase 8: Re-initialize and try again to explore state */
    rc = sqlite3_initialize();
    if (rc == SQLITE_OK) {
        /* Try registering the same extension again (should be no-op) */
        rc = sqlite3_auto_extension(dummy_extension_entry_point);
        (void)rc;

        /* Try canceling the second one */
        rc = sqlite3_cancel_auto_extension(dummy_extension_entry_point_2);
        (void)rc;

        /* Check threadsafe again */
        threadsafe = sqlite3_threadsafe();
        (void)threadsafe;

        /* Shutdown again */
        rc = sqlite3_shutdown();
        (void)rc;
    }

    return 0;
}
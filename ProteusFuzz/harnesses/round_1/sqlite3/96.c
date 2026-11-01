// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Dummy extension entry point for sqlite3_auto_extension.
 */
static void dummy_extension_entry_point(void) {
    /* Intentionally empty */
}

/*
 * Another dummy extension to test multiple registrations.
 */
static void dummy_extension_entry_point_2(void) {
    /* Intentionally empty */
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    int rc;
    int op;

    /* Ensure we have at least one byte to determine an operation */
    if (Size == 0) {
        return 0;
    }

    /* Use the first byte to select an operation */
    op = Data[0] % 7;

    /* 
     * Ensure SQLite is initialized before performing operations that require it.
     * The crash occurred because sqlite3_cancel_auto_extension tries to access
     * mutexes that are not initialized if sqlite3_initialize() hasn't been called.
     * We call initialize at the start of every input to ensure a valid state.
     * Note: sqlite3_initialize is idempotent.
     */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        /* If initialization fails, we cannot safely proceed with most ops */
        return 0;
    }

    switch (op) {
        case 0:
            /* Test sqlite3_threadsafe */
            (void)sqlite3_threadsafe();
            break;

        case 1:
            /* Test sqlite3_initialize (idempotent, safe to call again) */
            rc = sqlite3_initialize();
            break;

        case 2:
            /* Test sqlite3_shutdown */
            /* Note: After shutdown, subsequent calls to other APIs may fail or crash
               if they expect initialized state. We handle this by re-initializing
               at the start of the next iteration, but we must ensure we don't
               call other APIs immediately after shutdown in the same input if
               they require init. However, the switch logic is atomic per input.
               If we shutdown here, we just return.
            */
            rc = sqlite3_shutdown();
            /* After shutdown, do not call other APIs in this input */
            break;

        case 3:
            /* Test sqlite3_config with various options */
            if (Size > 1) {
                int option = Data[1] % 10;
                switch (option) {
                    case 0: rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD); break;
                    case 1: rc = sqlite3_config(SQLITE_CONFIG_MULTITHREAD); break;
                    case 2: rc = sqlite3_config(SQLITE_CONFIG_SERIALIZED); break;
                    case 3: rc = sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL); break;
                    case 4: rc = sqlite3_config(SQLITE_CONFIG_URI, 0); break;
                    case 5: rc = sqlite3_config(SQLITE_CONFIG_URI, 1); break;
                    case 6: rc = sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 0); break;
                    case 7: rc = sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 1); break;
                    case 8: rc = sqlite3_config(SQLITE_CONFIG_MALLOC, NULL); break;
                    case 9: rc = sqlite3_config(0); break;
                    default: rc = sqlite3_config(0); break;
                }
            } else {
                rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            }
            break;

        case 4:
            /* Test sqlite3_auto_extension */
            rc = sqlite3_auto_extension(dummy_extension_entry_point);
            break;

        case 5:
            /* Test sqlite3_auto_extension with a second function */
            rc = sqlite3_auto_extension(dummy_extension_entry_point_2);
            break;

        case 6:
            /* Test sqlite3_cancel_auto_extension */
            /* This requires the library to be initialized (mutexes ready).
               Since we called sqlite3_initialize() at the top, this is safe. */
            rc = sqlite3_cancel_auto_extension(dummy_extension_entry_point);
            break;

        default:
            /* Fallback */
            (void)sqlite3_threadsafe();
            break;
    }

    /* 
     * Optional: Clean up auto-extensions registered in this input to prevent
     * state accumulation across inputs if desired, but usually fuzzers
     * prefer state accumulation or reset. 
     * Since sqlite3_shutdown is an option, we don't force it here.
     * If op was 2 (shutdown), the library is now shut down. 
     * The next input will call sqlite3_initialize() again.
     */

    return 0;
}
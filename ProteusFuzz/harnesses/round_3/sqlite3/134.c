// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:189543:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:189619:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int g_initialized = 0;

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    // Fuzz sqlite3_initialize
    // It is idempotent, so calling it multiple times is safe.
    int rc = sqlite3_initialize();
    if (rc == SQLITE_OK) {
        g_initialized = 1;
    }

    // Fuzz sqlite3_threadsafe
    // This is a query function, safe to call anytime.
    int threadsafe = sqlite3_threadsafe();
    (void)threadsafe;

    // Fuzz sqlite3_global_recover
    // This is a stub, safe to call anytime.
    rc = sqlite3_global_recover();
    (void)rc;

    // Fuzz sqlite3_thread_cleanup
    // This is a no-op stub, safe to call anytime.
    sqlite3_thread_cleanup();

    // Fuzz sqlite3_shutdown and sqlite3_os_end
    // These are destructive. We only call them if we successfully initialized.
    // We do this to explore the state after shutdown.
    if (g_initialized) {
        rc = sqlite3_shutdown();
        if (rc == SQLITE_OK) {
            g_initialized = 0;
            // sqlite3_os_end is typically called by sqlite3_shutdown internally.
            // Calling it explicitly here tests the cleanup path.
            rc = sqlite3_os_end();
            (void)rc;
        }
    }

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:189543:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:189619:17 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:189619:17 in sqlite3.h
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
 * Fuzz driver for SQLite global initialization, shutdown, and thread safety APIs.
 * Target functions:
 * - sqlite3_global_recover
 * - sqlite3_initialize
 * - sqlite3_shutdown
 * - sqlite3_threadsafe
 * - sqlite3_os_end
 * - sqlite3_thread_cleanup
 *
 * Note: sqlite3_shutdown and sqlite3_os_end are not thread-safe and must be called
 * carefully. This fuzzer runs single-threaded and ensures proper sequencing.
 */

static int invoke_sequence(const uint8_t *Data, size_t Size) {
    int ret = SQLITE_OK;
    int init_count = 0;
    int shutdown_count = 0;
    int recover_count = 0;
    int os_end_count = 0;
    int cleanup_count = 0;
    int threadsafe_check = 0;

    /* Use input data to drive diversity in call patterns */
    if (Size > 0) {
        init_count = Data[0] % 5;
        shutdown_count = (Size > 1 ? Data[1] : 0) % 5;
        recover_count = (Size > 2 ? Data[2] : 0) % 5;
        os_end_count = (Size > 3 ? Data[3] : 0) % 5;
        cleanup_count = (Size > 4 ? Data[4] : 0) % 5;
        threadsafe_check = (Size > 5 ? Data[5] : 0) % 3;
    }

    /* 1. Check threadsafe status (safe to call anytime) */
    if (threadsafe_check > 0) {
        (void)sqlite3_threadsafe();
    }

    /* 2. Call sqlite3_global_recover multiple times (idempotent, no side effects) */
    for (int i = 0; i < recover_count; i++) {
        (void)sqlite3_global_recover();
    }

    /* 3. Initialize SQLite */
    if (init_count > 0) {
        ret = sqlite3_initialize();
        if (ret != SQLITE_OK) {
            /* Initialization failed; avoid further calls that assume initialized state */
            return ret;
        }
    }

    /* 4. Call sqlite3_thread_cleanup multiple times (no-op, safe) */
    for (int i = 0; i < cleanup_count; i++) {
        sqlite3_thread_cleanup();
    }

    /* 5. Optionally call sqlite3_os_end (only if initialized and no active connections) */
    /* Note: sqlite3_os_end is typically called by sqlite3_shutdown. 
     * Calling it directly after sqlite3_initialize but before sqlite3_shutdown
     * may be unsafe in some builds, but per docs it's a cleanup routine.
     * We call it only if we initialized and haven't shut down yet.
     */
    if (init_count > 0 && os_end_count > 0 && shutdown_count == 0) {
        (void)sqlite3_os_end();
    }

    /* 6. Shutdown SQLite */
    if (shutdown_count > 0) {
        ret = sqlite3_shutdown();
        /* sqlite3_shutdown is idempotent; subsequent calls are no-ops */
        for (int i = 1; i < shutdown_count; i++) {
            (void)sqlite3_shutdown();
        }
    }

    /* 7. Post-shutdown: calling sqlite3_os_end again is safe (idempotent) */
    if (shutdown_count > 0 && os_end_count > 0) {
        (void)sqlite3_os_end();
    }

    /* 8. Post-shutdown: calling sqlite3_thread_cleanup is safe */
    for (int i = 0; i < cleanup_count; i++) {
        sqlite3_thread_cleanup();
    }

    return ret;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    /*
     * This fuzzer explores various sequences of SQLite global management calls.
     * Since the target APIs are global and have no direct input parameters,
     * we use the fuzzer input to vary the number and order of calls.
     */
    int result = invoke_sequence(Data, Size);
    (void)result; /* Suppress unused variable warning */
    return 0;
}
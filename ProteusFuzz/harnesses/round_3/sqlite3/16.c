// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
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
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:72372:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API sequence:
 * sqlite3_config -> sqlite3_config -> sqlite3_config ->
 * sqlite3_threadsafe -> sqlite3_initialize ->
 * sqlite3_enable_shared_cache -> sqlite3_open ->
 * sqlite3_close -> sqlite3_free
 */

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /*
     * Step 1: Prepare environment and invoke sqlite3_config three times.
     * We use the input data to select configuration options and arguments.
     * sqlite3_config must be called before sqlite3_initialize or after shutdown.
     * Since we are in a fuzzing loop, we assume the library is in a clean state
     * or we handle SQLITE_MISUSE gracefully.
     */

    /* First sqlite3_config call */
    if (Size > 0) {
        int opt1 = Data[0] % 5;
        if (opt1 == 0) {
            rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
        } else if (opt1 == 1) {
            rc = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
        } else if (opt1 == 2) {
            rc = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
        } else if (opt1 == 3) {
            rc = sqlite3_config(SQLITE_CONFIG_URI, 1);
        } else {
            rc = sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
        }
        (void)rc;
    }

    /* Second sqlite3_config call */
    if (Size > 1) {
        int opt2 = Data[1] % 5;
        if (opt2 == 0) {
            rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
        } else if (opt2 == 1) {
            rc = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
        } else if (opt2 == 2) {
            rc = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
        } else if (opt2 == 3) {
            rc = sqlite3_config(SQLITE_CONFIG_URI, 0);
        } else {
            rc = sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
        }
        (void)rc;
    }

    /* Third sqlite3_config call */
    if (Size > 2) {
        int opt3 = Data[2] % 5;
        if (opt3 == 0) {
            rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
        } else if (opt3 == 1) {
            rc = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
        } else if (opt3 == 2) {
            rc = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
        } else if (opt3 == 3) {
            rc = sqlite3_config(SQLITE_CONFIG_URI, 1);
        } else {
            rc = sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
        }
        (void)rc;
    }

    /* Step 2: Invoke sqlite3_threadsafe */
    int isThreadSafe = sqlite3_threadsafe();
    (void)isThreadSafe;

    /* Step 3: Invoke sqlite3_initialize */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Step 4: Invoke sqlite3_enable_shared_cache */
    int enableCache = (Size > 3) ? (Data[3] % 2) : 0;
    rc = sqlite3_enable_shared_cache(enableCache);
    (void)rc;

    /* Step 5: Invoke sqlite3_open */
    const char *filename = "./dummy_file";

    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK && db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }

    /* Step 6: Invoke sqlite3_close */
    if (db != NULL) {
        rc = sqlite3_close(db);
        db = NULL;
        (void)rc;
    } else {
        rc = sqlite3_close(NULL);
        (void)rc;
    }

    /* Step 7: Invoke sqlite3_free */
    sqlite3_free(NULL);

    /* Optional: Try to allocate and free to exercise the allocator more */
    if (Size > 4) {
        size_t allocSize = Data[4];
        if (allocSize > 1024) allocSize = 1024;
        void *ptr = sqlite3_malloc(allocSize);
        if (ptr != NULL) {
            memset(ptr, 0, allocSize);
            sqlite3_free(ptr);
        }
    }

    return 0;
}
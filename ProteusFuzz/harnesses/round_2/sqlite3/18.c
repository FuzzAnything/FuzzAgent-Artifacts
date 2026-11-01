// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:72372:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:72372:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3 API sequence:
 * sqlite3_config -> sqlite3_config -> sqlite3_config -> sqlite3_threadsafe ->
 * sqlite3_initialize -> sqlite3_enable_shared_cache -> sqlite3_open ->
 * sqlite3_close -> sqlite3_free
 *
 * Fixed to only use safe "anytime" configuration options or options that 
 * do not require complex pointer arguments to avoid crashes.
 */

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int config_opt = 0;

    /* 
     * Step 1: sqlite3_config (3 calls)
     * We restrict options to safe ones that don't require pointer arguments 
     * that could be NULL or invalid, or we only pass valid arguments.
     * Safe options include:
     * SQLITE_CONFIG_SINGLETHREAD (0)
     * SQLITE_CONFIG_MULTITHREAD (1)
     * SQLITE_CONFIG_SERIALIZED (2)
     * SQLITE_CONFIG_LOG (3) - requires callback and arg, skip for simplicity or use NULL
     * SQLITE_CONFIG_URI (9)
     * SQLITE_CONFIG_MMAP_SIZE (10)
     * SQLITE_CONFIG_WIN32_HEAPSIZE (11)
     * SQLITE_CONFIG_PCACHE (12) - requires pointer
     * SQLITE_CONFIG_GETPCACHE (13)
     * SQLITE_CONFIG_LOG (3) - requires callback
     * 
     * To avoid the crash seen (dereferencing NULL in va_arg), we only use 
     * options that take simple integer arguments or no arguments.
     * We'll use SQLITE_CONFIG_SINGLETHREAD, SQLITE_CONFIG_MULTITHREAD, 
     * SQLITE_CONFIG_SERIALIZED, SQLITE_CONFIG_URI, SQLITE_CONFIG_MMAP_SIZE,
     * SQLITE_CONFIG_WIN32_HEAPSIZE, SQLITE_CONFIG_PCACHE_HDRSZ.
     */
    
    /* Call 1 */
    if (Size > 0) {
        int val = Data[0];
        /* Map input to safe options */
        switch (val % 6) {
            case 0: config_opt = SQLITE_CONFIG_SINGLETHREAD; break;
            case 1: config_opt = SQLITE_CONFIG_MULTITHREAD; break;
            case 2: config_opt = SQLITE_CONFIG_SERIALIZED; break;
            case 3: config_opt = SQLITE_CONFIG_URI; break;
            case 4: config_opt = SQLITE_CONFIG_MMAP_SIZE; break;
            case 5: config_opt = SQLITE_CONFIG_WIN32_HEAPSIZE; break;
        }
        rc = sqlite3_config(config_opt);
        (void)rc;
    }

    /* Call 2 */
    if (Size > 1) {
        int val = Data[1];
        switch (val % 6) {
            case 0: config_opt = SQLITE_CONFIG_SINGLETHREAD; break;
            case 1: config_opt = SQLITE_CONFIG_MULTITHREAD; break;
            case 2: config_opt = SQLITE_CONFIG_SERIALIZED; break;
            case 3: config_opt = SQLITE_CONFIG_URI; break;
            case 4: config_opt = SQLITE_CONFIG_MMAP_SIZE; break;
            case 5: config_opt = SQLITE_CONFIG_WIN32_HEAPSIZE; break;
        }
        rc = sqlite3_config(config_opt);
        (void)rc;
    }

    /* Call 3 */
    if (Size > 2) {
        int val = Data[2];
        switch (val % 6) {
            case 0: config_opt = SQLITE_CONFIG_SINGLETHREAD; break;
            case 1: config_opt = SQLITE_CONFIG_MULTITHREAD; break;
            case 2: config_opt = SQLITE_CONFIG_SERIALIZED; break;
            case 3: config_opt = SQLITE_CONFIG_URI; break;
            case 4: config_opt = SQLITE_CONFIG_MMAP_SIZE; break;
            case 5: config_opt = SQLITE_CONFIG_WIN32_HEAPSIZE; break;
        }
        rc = sqlite3_config(config_opt);
        (void)rc;
    }

    /* 
     * Step 2: sqlite3_threadsafe
     * This is a simple query function, no state change.
     */
    rc = sqlite3_threadsafe();
    (void)rc;

    /* 
     * Step 3: sqlite3_initialize
     * Explicitly initialize the library.
     */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        /* If initialization fails, we can't proceed with open/close, but we continue to test free */
        sqlite3_free(NULL);
        return 0;
    }

    /* 
     * Step 4: sqlite3_enable_shared_cache
     * Enable or disable shared cache based on input data.
     * This affects future connections.
     */
    if (Size > 3) {
        int enable = (Data[3] % 2); /* 0 or 1 */
        rc = sqlite3_enable_shared_cache(enable);
        (void)rc;
    } else {
        rc = sqlite3_enable_shared_cache(0);
        (void)rc;
    }

    /* 
     * Step 5: sqlite3_open
     * Open a database. We use a dummy file name as per instructions.
     * We can also try in-memory databases or other special names based on input.
     */
    const char *filename = "./dummy_file";
    
    /* Optionally fuzz the filename if Size is large enough */
    if (Size > 4) {
        /* Create a temporary filename based on input to explore different states */
        static char fuzzed_filename[256];
        size_t len = Size < 250 ? Size : 250;
        memcpy(fuzzed_filename, Data, len);
        fuzzed_filename[len] = '\0';
        
        /* Ensure it's a valid path string by replacing nulls if any */
        for (size_t i = 0; i < len; i++) {
            if (fuzzed_filename[i] == '\0') {
                fuzzed_filename[i] = 'x';
            }
        }
        filename = fuzzed_filename;
    }

    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK && db != NULL) {
        /* Even on error, db might be non-NULL, so we must close it */
        sqlite3_close(db);
        db = NULL;
    }

    /* 
     * Step 6: sqlite3_close
     * Close the database connection if it was opened successfully.
     * Passing NULL is safe.
     */
    if (db != NULL) {
        rc = sqlite3_close(db);
        (void)rc;
        db = NULL;
    } else {
        /* Try closing NULL explicitly to test that path */
        rc = sqlite3_close(NULL);
        (void)rc;
    }

    /* 
     * Step 7: sqlite3_free
     * Free any memory allocated by SQLite.
     * We pass NULL to test the safe path.
     */
    sqlite3_free(NULL);

    /* 
     * Cleanup: Shutdown the library if we initialized it.
     * This is good practice to ensure clean state for next fuzz iteration.
     */
    sqlite3_shutdown();

    return 0;
}
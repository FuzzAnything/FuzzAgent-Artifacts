// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:33787:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:58365:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:33920:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    /* Use the input data to influence fuzzing decisions */
    unsigned int seed = 0;
    if (Size > 0) {
        /* Simple hash to derive a seed from input */
        for (size_t i = 0; i < Size; ++i) {
            seed = (seed * 31) + Data[i];
        }
    }

    /* Use seed to make fuzzing decisions */
    int decision = seed % 10;

    /* Start with shutdown to ensure clean state (idempotent) */
    sqlite3_shutdown();

    /* Call sqlite3_os_end (always returns OK) */
    sqlite3_os_end();

    /* Try sqlite3_os_init (initializes OS interface) */
    sqlite3_os_init();

    /* Initialize the library */
    sqlite3_initialize();

    /* Toggle shared cache based on input */
    int enable_cache = (seed & 1) ? 1 : 0;
    sqlite3_enable_shared_cache(enable_cache);

    /* Try different sqlite3_config options based on decision */
    switch (decision) {
        case 0:
            /* Single-threaded mode */
            sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            break;
        case 1:
            /* Multi-threaded mode */
            sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
            break;
        case 2:
            /* Serialized mode */
            sqlite3_config(SQLITE_CONFIG_SERIALIZED);
            break;
        case 3:
            /* Set page cache size */
            sqlite3_config(SQLITE_CONFIG_PAGECACHE, NULL, 0, 0);
            break;
        case 4:
            /* Set heap limit */
            sqlite3_config(SQLITE_CONFIG_HEAP, NULL, 0, 0);
            break;
        case 5:
            /* Set memory status reporting */
            sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 0);
            break;
        case 6:
            /* Set URI handling */
            sqlite3_config(SQLITE_CONFIG_URI, 1);
            break;
        case 7:
            /* Set CODEC */
            sqlite3_config(SQLITE_CONFIG_COVERING_INDEX_SCAN, 1);
            break;
        default:
            /* No additional config */
            break;
    }

    /* Shutdown again to test cleanup */
    sqlite3_shutdown();

    /* Final os_end */
    sqlite3_os_end();

    return 0;
}

#ifdef __cplusplus
}
#endif
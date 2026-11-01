// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:47794:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:47794:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:47794:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
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
 * Fuzz driver for SQLite3 initialization, OS layer, sleep, randomness, and shutdown APIs.
 * Entry point: LLVMFuzzerTestOneInput
 */

static int g_initialized = 0;
static int g_shutdown = 0;

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    /*
     * We use the input data to drive different code paths:
     * - First byte decides which API to test.
     * - Remaining bytes may be used as parameters (e.g., sleep duration, randomness size).
     */

    if (Size == 0) {
        return 0;
    }

    int op = Data[0] & 0x07; /* 0..7 */
    int ms = 0;
    int rand_bytes = 0;
    uint8_t *rand_buf = NULL;

    if (Size >= 2) {
        /* Use next two bytes to form a small integer for ms and rand_bytes */
        ms = (Data[1] & 0xFF);
        if (Size >= 3) {
            rand_bytes = (Data[2] & 0xFF);
        }
    }

    /* Clamp values to reasonable ranges to avoid hanging or huge allocations */
    if (ms > 1000) ms = 1000;
    if (rand_bytes > 1024) rand_bytes = 1024;

    if (rand_bytes > 0) {
        rand_buf = (uint8_t *)malloc((size_t)rand_bytes);
        if (!rand_buf) {
            return 0;
        }
    }

    switch (op) {
        case 0: {
            /* Test sqlite3_initialize */
            int rc = sqlite3_initialize();
            if (rc == SQLITE_OK) {
                g_initialized = 1;
                g_shutdown = 0;
            }
            break;
        }
        case 1: {
            /* Test sqlite3_os_init */
            int rc = sqlite3_os_init();
            (void)rc; /* ignore return, just exercise */
            break;
        }
        case 2: {
            /* Test sqlite3_sleep with various durations (including negative) */
            int sleep_ms = ms;
            /* Occasionally test negative value */
            if (Size >= 4 && (Data[3] & 0x01)) {
                sleep_ms = -sleep_ms;
            }
            int actual = sqlite3_sleep(sleep_ms);
            (void)actual;
            break;
        }
        case 3: {
            /* Test sqlite3_os_end */
            int rc = sqlite3_os_end();
            (void)rc;
            g_initialized = 0;
            g_shutdown = 1;
            break;
        }
        case 4: {
            /* Test sqlite3_randomness with various sizes and NULL */
            int n = rand_bytes;
            if (Size >= 4 && (Data[3] & 0x01)) {
                /* Test seeding path: pass NULL or n <= 0 */
                n = 0;
            }
            if (n <= 0) {
                sqlite3_randomness(n, NULL);
            } else {
                sqlite3_randomness(n, rand_buf);
            }
            break;
        }
        case 5: {
            /* Test sqlite3_shutdown */
            int rc = sqlite3_shutdown();
            (void)rc;
            g_initialized = 0;
            g_shutdown = 1;
            break;
        }
        case 6: {
            /* Mixed sequence: init -> os_init -> sleep -> randomness -> os_end -> shutdown */
            if (!g_initialized || g_shutdown) {
                sqlite3_initialize();
                g_initialized = 1;
                g_shutdown = 0;
            }
            sqlite3_os_init();
            sqlite3_sleep(ms);
            if (rand_bytes > 0) {
                sqlite3_randomness(rand_bytes, rand_buf);
            } else {
                sqlite3_randomness(0, NULL);
            }
            sqlite3_os_end();
            sqlite3_shutdown();
            g_initialized = 0;
            g_shutdown = 1;
            break;
        }
        case 7: {
            /* Stress: multiple calls to init/shutdown and os_init/os_end */
            int rc;
            rc = sqlite3_initialize();
            (void)rc;
            rc = sqlite3_os_init();
            (void)rc;
            rc = sqlite3_os_end();
            (void)rc;
            rc = sqlite3_initialize();
            (void)rc;
            rc = sqlite3_shutdown();
            (void)rc;
            rc = sqlite3_shutdown();
            (void)rc;
            g_initialized = 0;
            g_shutdown = 1;
            break;
        }
        default:
            break;
    }

    if (rand_buf) {
        free(rand_buf);
    }

    return 0;
}
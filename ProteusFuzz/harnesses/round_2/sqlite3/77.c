// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:189543:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:189543:16 in sqlite3.h
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
 * Fuzz driver for SQLite3 API functions:
 * - sqlite3_sleep
 * - sqlite3_shutdown
 * - sqlite3_global_recover
 * - sqlite3_randomness
 * - sqlite3_initialize
 * - sqlite3_os_end
 *
 * This driver explores various states by invoking these functions with
 * different inputs derived from the fuzzed data.
 */

static int g_initialized = 0;

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * Use the input data to decide which functions to call and with what parameters.
     * We will use the first few bytes to make decisions.
     */

    int idx = 0;
    int choice = 0;

    /* Read a choice from the input if available */
    if (Size > idx) {
        choice = Data[idx++];
    }

    /*
     * Strategy:
     * 1. Always ensure initialization before calling most functions.
     * 2. Call sqlite3_initialize if not already initialized.
     * 3. Based on 'choice', call different functions.
     * 4. Always clean up with sqlite3_shutdown or sqlite3_os_end at the end.
     */

    /* Ensure initialization */
    if (!g_initialized) {
        if (sqlite3_initialize() == SQLITE_OK) {
            g_initialized = 1;
        }
    }

    switch (choice % 6) {
        case 0: {
            /* Fuzz sqlite3_sleep */
            int ms = 0;
            if (Size > idx + 3) {
                ms = (Data[idx] << 24) | (Data[idx+1] << 16) | (Data[idx+2] << 8) | Data[idx+3];
                idx += 4;
            }
            /* sqlite3_sleep handles negative values as 0 in 3.42+ */
            sqlite3_sleep(ms);
            break;
        }
        case 1: {
            /* Fuzz sqlite3_global_recover */
            sqlite3_global_recover();
            break;
        }
        case 2: {
            /* Fuzz sqlite3_randomness */
            int n = 0;
            if (Size > idx + 3) {
                n = (Data[idx] << 24) | (Data[idx+1] << 16) | (Data[idx+2] << 8) | Data[idx+3];
                idx += 4;
            }
            if (n < 0) n = 0;
            if (n > 1024) n = 1024; /* Limit buffer size for safety */

            if (n > 0) {
                uint8_t *buf = (uint8_t *)malloc(n);
                if (buf) {
                    sqlite3_randomness(n, buf);
                    free(buf);
                }
            } else {
                /* Seed the PRNG */
                sqlite3_randomness(n, NULL);
            }
            break;
        }
        case 3: {
            /* Fuzz sqlite3_shutdown */
            int ret = sqlite3_shutdown();
            if (ret == SQLITE_OK) {
                g_initialized = 0;
            }
            break;
        }
        case 4: {
            /* Fuzz sqlite3_os_end */
            sqlite3_os_end();
            g_initialized = 0;
            break;
        }
        case 5: {
            /* Re-initialize after potential shutdown */
            if (!g_initialized) {
                if (sqlite3_initialize() == SQLITE_OK) {
                    g_initialized = 1;
                }
            }
            break;
        }
    }

    /*
     * Additional exploration: call functions multiple times with varied inputs
     * to explore more states.
     */
    if (Size > idx) {
        int extra = Data[idx++];
        if (extra & 1) {
            /* Call sqlite3_sleep with a small value */
            sqlite3_sleep(1);
        }
        if (extra & 2) {
            /* Call sqlite3_global_recover again */
            sqlite3_global_recover();
        }
        if (extra & 4) {
            /* Call sqlite3_randomness with a small buffer */
            uint8_t buf[16];
            sqlite3_randomness(16, buf);
        }
        if (extra & 8) {
            /* Call sqlite3_initialize again (idempotent) */
            sqlite3_initialize();
        }
    }

    /*
     * Cleanup: ensure we don't leave the library in a partially initialized state
     * for the next fuzz iteration. However, since sqlite3_shutdown is not thread-safe
     * and we are in a single-threaded fuzzer, we can call it safely.
     * But note: calling sqlite3_shutdown may prevent further use in the same process.
     * Since each fuzz iteration is independent, we can choose to shutdown or not.
     * For safety, we will NOT shutdown here to allow the next iteration to reuse the library.
     * Instead, we rely on the fuzzer's process model (each input is a new process or reset).
     * However, if we did call sqlite3_shutdown in the switch, we set g_initialized=0.
     */

    return 0;
}
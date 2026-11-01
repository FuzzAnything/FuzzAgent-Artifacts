// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_os_init at sqlite3.c:47794:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite3 low-level initialization and utility functions.
 * Target APIs:
 * - sqlite3_os_init
 * - sqlite3_os_end
 * - sqlite3_initialize
 * - sqlite3_shutdown
 * - sqlite3_sleep
 * - sqlite3_randomness
 */

static int g_initialized = 0;

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    int cmd = Data[0] % 6;

    switch (cmd) {
        case 0: {
            // Test sqlite3_os_init
            int rc = sqlite3_os_init();
            (void)rc;
            break;
        }
        case 1: {
            // Test sqlite3_os_end
            int rc = sqlite3_os_end();
            (void)rc;
            break;
        }
        case 2: {
            // Test sqlite3_initialize
            int rc = sqlite3_initialize();
            if (rc == SQLITE_OK) {
                g_initialized = 1;
            }
            (void)rc;
            break;
        }
        case 3: {
            // Test sqlite3_shutdown
            int rc = sqlite3_shutdown();
            if (rc == SQLITE_OK) {
                g_initialized = 0;
            }
            (void)rc;
            break;
        }
        case 4: {
            // Test sqlite3_sleep
            int ms = 0;
            if (Size >= 3) {
                ms = (Data[1] << 8) | Data[2];
            }
            // Cap sleep time to 100ms to prevent timeout issues
            if (ms > 100) ms = 100;
            // Also test negative values (which should be treated as 0 in 3.42+)
            if (Size >= 4 && (Data[3] & 1)) {
                ms = -ms;
            }
            int actual = sqlite3_sleep(ms);
            (void)actual;
            break;
        }
        case 5: {
            // Test sqlite3_randomness
            int n = 0;
            if (Size >= 3) {
                n = (Data[1] << 8) | Data[2];
            }
            // Cap N to avoid huge allocations
            if (n > 1024) n = 1024;
            
            // Allocate buffer if n > 0
            void *buf = NULL;
            if (n > 0) {
                buf = malloc((size_t)n);
                if (!buf) {
                    // If malloc fails, call with NULL to trigger seeding path
                    sqlite3_randomness(n, NULL);
                } else {
                    sqlite3_randomness(n, buf);
                    free(buf);
                }
            } else {
                // Call with n <= 0 or NULL to trigger seeding path
                sqlite3_randomness(n, NULL);
            }
            break;
        }
        default:
            break;
    }

    return 0;
}
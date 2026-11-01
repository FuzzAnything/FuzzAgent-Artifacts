// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:189619:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Fuzz sqlite3_threadsafe (no args, constant return)
    int ts = sqlite3_threadsafe();
    (void)ts;

    // Fuzz sqlite3_thread_cleanup (no args, no-op)
    sqlite3_thread_cleanup();

    // Fuzz sqlite3_randomness
    // Use first byte to determine N (limit to reasonable size to avoid OOM)
    int n = Data[0] % 1024;
    if (n < 0) n = 0;
    
    // Allocate buffer for randomness if N > 0
    void *buf = NULL;
    if (n > 0) {
        buf = malloc(n);
        if (!buf) return 0;
    }
    
    // Call sqlite3_randomness
    // If n <= 0 or buf is NULL, it seeds from OS
    // If n > 0 and buf is valid, it fills buf
    sqlite3_randomness(n, buf);
    
    if (buf) {
        free(buf);
    }

    // Fuzz sqlite3_sleep
    // Use second byte for sleep duration (ms)
    // Ensure we don't sleep too long in fuzzing context
    int sleep_ms = 0;
    if (Size > 1) {
        sleep_ms = Data[1] % 100; // 0 to 99 ms
    }
    
    // Also try negative value if Size allows
    if (Size > 2) {
        if (Data[2] & 1) {
            // Try negative input (SQLite 3.42+ treats as 0)
            sqlite3_sleep(-100);
        }
    }
    
    sqlite3_sleep(sleep_ms);

    // Fuzz sqlite3_initialize and sqlite3_shutdown
    // Use third byte to decide initialization strategy
    if (Size > 2) {
        if (Data[2] & 2) {
            // Try to initialize
            int rc = sqlite3_initialize();
            if (rc == SQLITE_OK) {
                g_initialized = 1;
            }
        } else if (Data[2] & 4 && g_initialized) {
            // Try to shutdown if we think we initialized
            int rc = sqlite3_shutdown();
            (void)rc;
            g_initialized = 0;
        }
    }

    // Additional state exploration: initialize/shutdown cycles
    if (Size > 3) {
        if (Data[3] & 1) {
            sqlite3_initialize();
            g_initialized = 1;
        }
        if (Data[3] & 2) {
            sqlite3_shutdown();
            g_initialized = 0;
        }
    }

    // Ensure cleanup if we initialized during this run
    if (g_initialized) {
        sqlite3_shutdown();
        g_initialized = 0;
    }

    return 0;
}
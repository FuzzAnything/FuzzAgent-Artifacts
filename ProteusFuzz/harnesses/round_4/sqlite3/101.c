// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:47794:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void cleanup_mutex(sqlite3_mutex *pMutex) {
    if (pMutex != NULL) {
        sqlite3_mutex_free(pMutex);
    }
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Initialize SQLite library
    int initResult = sqlite3_initialize();
    if (initResult != SQLITE_OK) {
        // Initialization failed, try to continue or exit
        // For fuzzing, we might want to continue to test error paths
    }

    // Test sqlite3_os_init (idempotent, safe to call multiple times)
    int osInitResult = sqlite3_os_init();
    (void)osInitResult; // Suppress unused variable warning

    // Use input data to determine mutex type and operations
    int mutexType = Data[0] % 2; // 0 for FAST, 1 for RECURSIVE
    int opType = Data[1] % 3;    // 0: alloc only, 1: alloc + try, 2: alloc + enter + leave

    sqlite3_mutex *pMutex = NULL;

    // Allocate mutex based on input
    if (mutexType == 0) {
        pMutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
    } else {
        pMutex = sqlite3_mutex_alloc(SQLITE_MUTEX_RECURSIVE);
    }

    if (pMutex == NULL) {
        // Allocation failed, test sqlite3_shutdown and return
        sqlite3_shutdown();
        return 0;
    }

    // Perform operations based on opType
    if (opType == 0) {
        // Just allocate and free
        cleanup_mutex(pMutex);
    } else if (opType == 1) {
        // Try to lock without blocking
        int tryResult = sqlite3_mutex_try(pMutex);
        (void)tryResult;
        if (tryResult == SQLITE_OK) {
            // If locked, release it
            sqlite3_mutex_leave(pMutex);
        }
        cleanup_mutex(pMutex);
    } else {
        // Enter, leave, and free
        sqlite3_mutex_enter(pMutex);
        sqlite3_mutex_leave(pMutex);
        cleanup_mutex(pMutex);
    }

    // Test sqlite3_randomness with input data if available
    if (Size > 2) {
        int nBytes = Data[2] % 256;
        if (nBytes > 0) {
            unsigned char *randomBuffer = (unsigned char *)malloc(nBytes);
            if (randomBuffer != NULL) {
                // Seed PRNG if needed (pass NULL or N<=0)
                sqlite3_randomness(0, NULL);
                // Generate random bytes
                sqlite3_randomness(nBytes, randomBuffer);
                // Use the buffer to prevent unused variable warning
                volatile unsigned char *check = randomBuffer;
                (void)check;
                free(randomBuffer);
            }
        } else {
            // Seed PRNG with NULL
            sqlite3_randomness(0, NULL);
        }
    }

    // Shutdown SQLite library
    sqlite3_shutdown();

    return 0;
}
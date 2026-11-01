// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_compileoption_get at sqlite3.c:190798:24 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:266758:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static void fuzz_helper(const uint8_t *Data, size_t Size) {
    // 1. sqlite3_sourceid
    const char *sourceid = sqlite3_sourceid();
    (void)sourceid; // Use the return value to avoid unused warning

    // 2. sqlite3_compileoption_get
    // Use input data to determine the index N, ensuring it stays within reasonable bounds
    int index = 0;
    if (Size >= 2) {
        index = (int)((Data[0] << 8) | Data[1]);
    } else if (Size == 1) {
        index = (int)Data[0];
    }
    // Clamp index to a safe range to avoid excessive iteration or invalid access logic
    if (index < 0) index = 0;
    if (index > 1000) index = 1000;

    const char *option = sqlite3_compileoption_get(index);
    (void)option; // Use the return value

    // 3. sqlite3_sleep
    // Use input data to determine sleep duration in milliseconds
    int sleep_ms = 0;
    if (Size >= 4) {
        sleep_ms = (int)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    } else if (Size >= 2) {
        sleep_ms = (int)((Data[0] << 8) | Data[1]);
    } else if (Size >= 1) {
        sleep_ms = (int)Data[0];
    }

    // Clamp sleep_ms to prevent hanging the fuzzer for too long
    if (sleep_ms < 0) sleep_ms = 0;
    if (sleep_ms > 100) sleep_ms = 100; // Limit to 100ms max for fuzzing efficiency

    int actual_sleep = sqlite3_sleep(sleep_ms);
    (void)actual_sleep;

    // 4. sqlite3_free
    // Allocate memory to safely test sqlite3_free
    // Use input data to determine allocation size
    size_t alloc_size = 0;
    if (Size >= 4) {
        alloc_size = (size_t)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    } else if (Size >= 2) {
        alloc_size = (size_t)((Data[0] << 8) | Data[1]);
    } else if (Size >= 1) {
        alloc_size = (size_t)Data[0];
    }

    // Prevent huge allocations that could crash the fuzzer
    if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024; // 1MB limit

    void *ptr = sqlite3_malloc(alloc_size);
    if (ptr != NULL) {
        // Optionally write some data to the allocated memory to trigger more states
        memset(ptr, 0x42, alloc_size);
        sqlite3_free(ptr);
        ptr = NULL; // Prevent double-free
    }

    // Test freeing NULL (safe operation)
    sqlite3_free(NULL);
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        // Handle empty input gracefully
        fuzz_helper(NULL, 0);
        return 0;
    }

    fuzz_helper(Data, Size);
    return 0;
}
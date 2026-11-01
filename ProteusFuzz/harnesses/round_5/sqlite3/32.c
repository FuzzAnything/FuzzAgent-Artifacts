// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sourceid at sqlite3.c:266758:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:190798:24 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Step 1: Prepare environment
    // No specific state preparation needed for these specific APIs as they are mostly stateless
    // or rely on internal static data. However, we can use the input data to drive parameters.

    // Invoke sqlite3_sourceid (always returns a static string, no arguments)
    const char *sourceId = sqlite3_sourceid();
    // Use the return value to ensure the compiler doesn't optimize it out
    if (sourceId != NULL) {
        volatile int len = strlen(sourceId);
        (void)len;
    }

    // Invoke sqlite3_compileoption_get
    // Use input data to determine the index N.
    // We clamp the index to a reasonable range to avoid excessive iteration or invalid memory access logic
    // although the function handles invalid indices by returning NULL.
    int index = 0;
    if (Size > 0) {
        index = Data[0];
    }
    if (Size > 1) {
        index += Data[1] * 256;
    }
    // Ensure index is non-negative for the fuzzing logic, though the API handles negatives gracefully
    if (index < 0) index = 0;
    
    const char *option = sqlite3_compileoption_get(index);
    if (option != NULL) {
        volatile int optLen = strlen(option);
        (void)optLen;
    }

    // Invoke sqlite3_sleep
    // Use input data to determine sleep duration in milliseconds.
    // Clamp to a small value to prevent the fuzzer from hanging too long on a single input.
    int sleepMs = 0;
    if (Size > 2) {
        sleepMs = Data[2];
    }
    // Cap sleep time to 100ms max to keep fuzzing fast
    if (sleepMs > 100) sleepMs = 100;
    // Also test negative values if the byte suggests it (treated as 0 in newer SQLite)
    if (Size > 3 && Data[3] > 128) {
        sleepMs = -1; 
    }

    int actualSleep = sqlite3_sleep(sleepMs);
    (void)actualSleep;

    // Invoke sqlite3_free
    // We must allocate memory first using sqlite3_malloc to safely free it.
    // We cannot free the return values of sourceid or compileoption_get as they are static.
    size_t allocSize = 0;
    if (Size > 4) {
        allocSize = Data[4];
    }
    if (allocSize > 1024) allocSize = 1024; // Limit allocation size

    void *ptr = sqlite3_malloc(allocSize);
    if (ptr != NULL) {
        // Write some fuzz data to the allocated memory to exercise memory handling
        if (allocSize > 0 && Size > 0) {
            size_t writeSize = (Size > allocSize) ? allocSize : Size;
            memcpy(ptr, Data, writeSize);
        }
        // Now free it
        sqlite3_free(ptr);
    }
    
    // Test freeing NULL (safe operation)
    sqlite3_free(NULL);

    return 0;
}
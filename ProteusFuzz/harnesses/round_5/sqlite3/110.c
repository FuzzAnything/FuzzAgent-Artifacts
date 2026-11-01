// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Static helper to create a dummy VFS structure for fuzzing.
 * This structure is intentionally minimal and may contain NULL function pointers
 * to test error handling paths in sqlite3_vfs_register.
 */
static sqlite3_vfs *create_dummy_vfs(const uint8_t *data, size_t size, int idx) {
    static sqlite3_vfs vfs_instance;
    memset(&vfs_instance, 0, sizeof(vfs_instance));

    vfs_instance.iVersion = 3;
    vfs_instance.szOsFile = 0;
    vfs_instance.mxPathname = 0;
    vfs_instance.zName = "fuzz_vfs";
    vfs_instance.pAppData = NULL;
    vfs_instance.pNext = NULL;

    /*
     * Intentionally leave function pointers as NULL or set to dummy values
     * to trigger various code paths and potential NULL pointer checks
     * within the SQLite library when this VFS is used or registered.
     */
    vfs_instance.xOpen = NULL;
    vfs_instance.xDelete = NULL;
    vfs_instance.xAccess = NULL;
    vfs_instance.xFullPathname = NULL;
    vfs_instance.xDlOpen = NULL;
    vfs_instance.xDlError = NULL;
    vfs_instance.xDlSym = NULL;
    vfs_instance.xDlClose = NULL;
    vfs_instance.xRandomness = NULL;
    vfs_instance.xSleep = NULL;
    vfs_instance.xCurrentTime = NULL;
    vfs_instance.xGetLastError = NULL;
    vfs_instance.xCurrentTimeInt64 = NULL;
    vfs_instance.xSetSystemCall = NULL;
    vfs_instance.xGetSystemCall = NULL;
    vfs_instance.xNextSystemCall = NULL;

    return &vfs_instance;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * Step 1: Prepare the environment.
     * Initialize SQLite to ensure global resources are available.
     * This is idempotent, so calling it multiple times is safe.
     */
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    /*
     * Step 2: Explore diverse program states by invoking target functions.
     * We use the input data to drive variations in arguments.
     */

    /*
     * Fuzz sqlite3_vfs_register
     * Create a dummy VFS and attempt to register it.
     * The 'makeDflt' flag is derived from the first byte of input.
     */
    int makeDflt = (Size > 0) ? (Data[0] % 2) : 0;
    sqlite3_vfs *vfs = create_dummy_vfs(Data, Size, 0);
    int reg_result = sqlite3_vfs_register(vfs, makeDflt);
    (void)reg_result; /* Suppress unused variable warning, result indicates success/error */

    /*
     * Fuzz sqlite3_vfs_find
     * Try to find the VFS we just registered, or the default.
     * We can also try to find a non-existent VFS using input data as a name.
     */
    sqlite3_vfs *found_vfs = sqlite3_vfs_find("fuzz_vfs");
    if (found_vfs == NULL) {
        /* Try to find default */
        found_vfs = sqlite3_vfs_find(NULL);
    }
    (void)found_vfs;

    /*
     * Try to find a VFS with a name derived from input data (if it looks like a string).
     * We limit the search to avoid potential issues with very long strings.
     */
    if (Size > 1 && Data[Size - 1] == 0) {
        /* Null-terminated string in input */
        sqlite3_vfs *found_custom = sqlite3_vfs_find((const char *)Data);
        (void)found_custom;
    }

    /*
     * Fuzz sqlite3_sleep
     * Use input data to determine sleep duration.
     * Ensure we don't sleep for too long to avoid hanging the fuzzer.
     * Clamp the value to a reasonable range (e.g., 0 to 100 ms).
     * 
     * CRITICAL FIX: sqlite3_sleep internally calls the default VFS's xSleep method.
     * If we registered our dummy VFS as the default (makeDflt=1) and its xSleep is NULL,
     * sqlite3_sleep will crash (SEGV).
     * To prevent this, we only call sqlite3_sleep if we are sure the default VFS has xSleep.
     * Since we can't easily check the default VFS's xSleep without complex logic,
     * and our dummy VFS has xSleep = NULL, we will:
     * 1. Only call sqlite3_sleep if we did NOT register our dummy VFS as default.
     * 2. Or, ensure we re-register the default VFS (which is hard without knowing its name).
     * 
     * Simpler approach: Only call sqlite3_sleep if makeDflt was 0 (so our dummy VFS is not default).
     * If makeDflt was 1, our dummy VFS (with NULL xSleep) is default, so calling sqlite3_sleep crashes.
     * We skip the call in that case to avoid the crash.
     */
    int sleep_ms = 0;
    if (Size >= 3) {
        sleep_ms = (int)((Data[1] << 8 | Data[2]) % 101); /* 0 to 100 */
    } else if (Size == 2) {
        sleep_ms = (int)(Data[1] % 101);
    } else if (Size == 1) {
        sleep_ms = (int)(Data[0] % 101);
    }
    /* Handle negative inputs as per spec (treated as 0 in 3.42+) */
    if (sleep_ms < 0) sleep_ms = 0;

    /* Only call sqlite3_sleep if our dummy VFS is NOT the default */
    if (makeDflt == 0) {
        int actual_sleep = sqlite3_sleep(sleep_ms);
        (void)actual_sleep;
    }

    /*
     * Fuzz sqlite3_randomness
     * Use input data to determine how many bytes to generate.
     * We allocate a buffer on the stack for small sizes, or heap for larger.
     */
    int rand_bytes = 0;
    if (Size >= 5) {
        rand_bytes = (int)((Data[3] << 8 | Data[4]) % 257); /* 0 to 256 */
    } else if (Size >= 4) {
        rand_bytes = (int)(Data[3] % 257);
    } else if (Size >= 1) {
        rand_bytes = (int)(Data[0] % 257);
    }

    if (rand_bytes > 0) {
        uint8_t *rand_buf = (uint8_t *)malloc(rand_bytes);
        if (rand_buf) {
            sqlite3_randomness(rand_bytes, rand_buf);
            free(rand_buf);
        }
    } else {
        /* Test seeding path with N <= 0 or NULL buffer */
        sqlite3_randomness(rand_bytes, NULL);
    }

    /*
     * Fuzz sqlite3_shutdown
     * This function is idempotent and should be called before exit.
     * We call it here to test the shutdown path.
     * Note: After this call, other SQLite functions should not be used in a real app,
     * but for fuzzing, we call it to ensure the code path is covered.
     * Since we are in a loop (LLVMFuzzerTestOneInput), the next iteration
     * will call sqlite3_initialize() again, which is allowed.
     */
    int shutdown_result = sqlite3_shutdown();
    (void)shutdown_result;

    /*
     * Step 3: Cleanup.
     * No explicit cleanup needed for the static vfs_instance.
     * sqlite3_shutdown() handles global cleanup.
     * The next iteration will re-initialize.
     */

    return 0;
}
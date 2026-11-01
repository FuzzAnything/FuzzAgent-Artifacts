// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_enable_shared_cache at sqlite3.c:72372:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 library.
 * Invokes the target API functions in the specified order:
 * sqlite3_config -> sqlite3_config -> sqlite3_config -> sqlite3_threadsafe
 * -> sqlite3_initialize -> sqlite3_enable_shared_cache -> sqlite3_open
 * -> sqlite3_close -> sqlite3_free
 */

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int config_arg1, config_arg2, config_arg3;
    int shared_cache_flag;
    void *ptr = NULL;
    size_t alloc_size;

    /*
     * Step 1: Prepare environment and extract data from input.
     * We use the input data to drive configuration options and flags.
     * Ensure we have enough data before reading.
     */
    if (Size >= 4) {
        config_arg1 = (int)Data[0];
        config_arg2 = (int)Data[1];
        config_arg3 = (int)Data[2];
        shared_cache_flag = (int)Data[3];
    } else {
        /* Default values if input is too small */
        config_arg1 = 0;
        config_arg2 = 0;
        config_arg3 = 0;
        shared_cache_flag = 0;
    }

    /*
     * Step 2: Invoke target functions in the specified order.
     *
     * 1. sqlite3_config (First call)
     * We pass the extracted arguments. Note that invalid opcodes may return SQLITE_MISUSE,
     * which is expected behavior for fuzzing.
     *
     * CRITICAL FIX: sqlite3_config expects specific argument types based on the opcode.
     * Passing raw integers as pointers (e.g., for SQLITE_CONFIG_MALLOC) causes SEGV.
     * We restrict opcodes to those that accept integer arguments directly (like SQLITE_CONFIG_SINGLETHREAD, etc.)
     * or use safe dummy values.
     *
     * Safe opcodes that take int arguments:
     * SQLITE_CONFIG_SINGLETHREAD (1), SQLITE_CONFIG_MULTITHREAD (2), SQLITE_CONFIG_SERIALIZED (3)
     * SQLITE_CONFIG_LOG (5) - takes function pointer and void*, we skip or use 0
     * SQLITE_CONFIG_URI (9) - takes int
     *
     * To avoid SEGV, we only use opcodes that are safe with integer arguments or 0.
     * We will use 0, 1, 2, 3, 9 as safe opcodes.
     */
    
    // Map raw input to safe opcodes to prevent passing invalid pointers
    int safe_op1 = (config_arg1 % 10); 
    if (safe_op1 == 4 || safe_op1 == 5 || safe_op1 == 6 || safe_op1 == 7 || safe_op1 == 8) {
        safe_op1 = 0; // Avoid opcodes requiring function pointers or complex structs
    }
    
    int safe_op2 = (config_arg2 % 10);
    if (safe_op2 == 4 || safe_op2 == 5 || safe_op2 == 6 || safe_op2 == 7 || safe_op2 == 8) {
        safe_op2 = 0;
    }

    int safe_op3 = (config_arg3 % 10);
    if (safe_op3 == 4 || safe_op3 == 5 || safe_op3 == 6 || safe_op3 == 7 || safe_op3 == 8) {
        safe_op3 = 0;
    }

    rc = sqlite3_config(safe_op1, config_arg2);
    (void)rc;

    /*
     * 2. sqlite3_config (Second call)
     */
    rc = sqlite3_config(safe_op2, config_arg3);
    (void)rc;

    /*
     * 3. sqlite3_config (Third call)
     */
    rc = sqlite3_config(safe_op3, config_arg1);
    (void)rc;

    /*
     * 4. sqlite3_threadsafe
     * Check thread safety mode.
     */
    rc = sqlite3_threadsafe();
    (void)rc;

    /*
     * 5. sqlite3_initialize
     * Explicitly initialize the library.
     */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        /* If initialization fails, we continue to test robustness. */
    }

    /*
     * 6. sqlite3_enable_shared_cache
     * Enable or disable shared cache based on input.
     */
    rc = sqlite3_enable_shared_cache(shared_cache_flag);
    (void)rc;

    /*
     * 7. sqlite3_open
     * Open a database. We use a dummy file name or in-memory database.
     */
    const char *filename;
    if (Size > 4 && Data[4] == 1) {
        filename = ":memory:";
    } else {
        filename = "./dummy_file";
    }

    rc = sqlite3_open(filename, &db);
    /* Even on error, db might not be NULL. We should close it later. */

    /*
     * 8. sqlite3_close
     * Close the database connection.
     */
    if (db != NULL) {
        rc = sqlite3_close(db);
        (void)rc;
        db = NULL; /* Ensure we don't double close */
    }

    /*
     * 9. sqlite3_free
     * Allocate memory using sqlite3_malloc to ensure we have a valid pointer to free.
     * Prevent integer overflow and zero allocation issues.
     */
    if (Size > 0) {
        alloc_size = (size_t)Data[0];
        /* Limit allocation size to prevent OOM or huge allocations */
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
        ptr = sqlite3_malloc(alloc_size + 1);
    } else {
        ptr = sqlite3_malloc(1);
    }

    if (ptr != NULL) {
        sqlite3_free(ptr);
    }

    /*
     * Cleanup:
     * We have already closed the database and freed the allocated memory.
     * We do not call sqlite3_shutdown() to allow the fuzzer to reuse the process state.
     */

    return 0;
}
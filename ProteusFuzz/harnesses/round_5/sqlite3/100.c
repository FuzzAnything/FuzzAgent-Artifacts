// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:189543:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite3 global and utility APIs.
 * Target APIs:
 * 1. sqlite3_global_recover
 * 2. sqlite3_status
 * 3. sqlite3_test_control
 * 4. sqlite3_threadsafe
 * 5. sqlite3_randomness
 * 6. sqlite3_initialize
 */

static int fuzz_sqlite3_status(const uint8_t *data, size_t size) {
    int pCurrent = 0;
    int pHighwater = 0;
    int resetFlag = 0;
    int op = 0;
    int ret;

    if (size >= 4) {
        op = (int)data[0];
        resetFlag = (data[1] % 2);
    }

    /* Use only widely supported SQLITE_STATUS_ codes to avoid compilation errors */
    int ops[] = {
        SQLITE_STATUS_MEMORY_USED,
        SQLITE_STATUS_PAGECACHE_USED,
        SQLITE_STATUS_PAGECACHE_OVERFLOW,
        SQLITE_STATUS_MALLOC_SIZE,
        SQLITE_STATUS_PARSER_STACK
    };
    int num_ops = sizeof(ops) / sizeof(ops[0]);

    /* Fuzz with selected op codes */
    for (int i = 0; i < num_ops; i++) {
        ret = sqlite3_status(ops[i], &pCurrent, &pHighwater, resetFlag);
        (void)ret;
    }

    /* Also try random op code from input */
    if (size >= 1) {
        ret = sqlite3_status((int)data[0], &pCurrent, &pHighwater, resetFlag);
        (void)ret;
    }

    return 0;
}

static int fuzz_sqlite3_test_control(const uint8_t *data, size_t size) {
    int op = 0;
    int ret;
    int dummy_int = 0;
    int *dummy_ptr = &dummy_int;
    sqlite3 *db = NULL;

    /* Create a valid in-memory database connection to satisfy test_control requirements */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    if (size >= 1) {
        op = (int)data[0];
    }

    /* Operation: SQLITE_TESTCTRL_ISINIT (no args) - Safe */
    if (size >= 1) {
        ret = sqlite3_test_control(SQLITE_TESTCTRL_ISINIT);
        (void)ret;
    }

    /* 
     * Fixed: SQLITE_TESTCTRL_OPTIMIZATIONS requires a valid sqlite3* pointer as the 2nd argument.
     * Previously, '0' was passed, causing a NULL pointer dereference inside the library.
     * Now we pass the valid 'db' handle created above.
     */
    if (size >= 1 && db != NULL) {
        ret = sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0);
        (void)ret;
    }

    sqlite3_close(db);
    return 0;
}

static int fuzz_sqlite3_randomness(const uint8_t *data, size_t size) {
    int n = 0;
    unsigned char *buffer = NULL;

    if (size >= 1) {
        n = (int)data[0];
    }

    /* Ensure n is within reasonable bounds for fuzzing */
    if (n < 0) n = 0;
    if (n > 1024) n = 1024;

    if (n > 0) {
        buffer = (unsigned char *)malloc(n);
        if (!buffer) return 0;
        sqlite3_randomness(n, buffer);
        free(buffer);
    } else {
        /* Call with NULL or n <= 0 to trigger seeding */
        sqlite3_randomness(n, NULL);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    /* Initialize SQLite library */
    sqlite3_initialize();

    /* Fuzz sqlite3_global_recover */
    sqlite3_global_recover();

    /* Fuzz sqlite3_status */
    fuzz_sqlite3_status(Data, Size);

    /* Fuzz sqlite3_test_control */
    fuzz_sqlite3_test_control(Data, Size);

    /* Fuzz sqlite3_threadsafe */
    int threadsafe = sqlite3_threadsafe();
    (void)threadsafe;

    /* Fuzz sqlite3_randomness */
    fuzz_sqlite3_randomness(Data, Size);

    /* Cleanup: shutdown SQLite library */
    sqlite3_shutdown();

    return 0;
}
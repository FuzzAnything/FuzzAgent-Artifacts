// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>

/*
 * Fuzz driver for sqlite3 mutex and connection functions.
 * Target APIs:
 * 1. sqlite3_db_mutex
 * 2. sqlite3_mutex_alloc
 * 3. sqlite3_mutex_enter
 * 4. sqlite3_mutex_leave
 * 5. sqlite3_mutex_free
 * 6. sqlite3_mutex_try
 */

static void cleanup_mutex(sqlite3_mutex *m) {
    if (m == NULL) return;
    /* Only free dynamic mutexes. Static ones should not be freed.
     * We only allocate SQLITE_MUTEX_FAST and SQLITE_MUTEX_RECURSIVE here.
     */
    sqlite3_mutex_free(m);
}

static void cleanup_db(sqlite3 *db) {
    if (db != NULL) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Use the first byte to determine the test scenario */
    int scenario = Data[0] % 5;

    sqlite3 *db = NULL;
    sqlite3_mutex *mutex = NULL;
    int rc;

    /* Initialize SQLite if needed (some functions auto-init, but explicit is safer for fuzzing) */
    sqlite3_initialize();

    if (scenario == 0) {
        /* Scenario 0: Test sqlite3_db_mutex with a valid connection */
        /* Create a dummy database file */
        FILE *f = fopen("./dummy_file", "w");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
        }

        rc = sqlite3_open("./dummy_file", &db);
        if (rc != SQLITE_OK) {
            /* Open failed, but we can still test with NULL or invalid db if we want,
             * but sqlite3_db_mutex expects a valid pointer. Let's just return. */
            return 0;
        }

        /* Call sqlite3_db_mutex */
        sqlite3_mutex *m = sqlite3_db_mutex(db);
        /* m might be NULL if not in Serialized mode, which is expected behavior */
        (void)m; /* Suppress unused warning */

        cleanup_db(db);
        unlink("./dummy_file");

    } else if (scenario == 1) {
        /* Scenario 1: Test sqlite3_mutex_alloc with dynamic types */
        int type = (Data[0] % 2) == 0 ? SQLITE_MUTEX_FAST : SQLITE_MUTEX_RECURSIVE;
        mutex = sqlite3_mutex_alloc(type);
        if (mutex != NULL) {
            cleanup_mutex(mutex);
        }

    } else if (scenario == 2) {
        /* Scenario 2: Test mutex lifecycle: alloc -> enter -> leave -> free */
        int type = (Data[0] % 2) == 0 ? SQLITE_MUTEX_FAST : SQLITE_MUTEX_RECURSIVE;
        mutex = sqlite3_mutex_alloc(type);
        if (mutex != NULL) {
            sqlite3_mutex_enter(mutex);
            sqlite3_mutex_leave(mutex);
            cleanup_mutex(mutex);
        }

    } else if (scenario == 3) {
        /* Scenario 3: Test sqlite3_mutex_try */
        int type = (Data[0] % 2) == 0 ? SQLITE_MUTEX_FAST : SQLITE_MUTEX_RECURSIVE;
        mutex = sqlite3_mutex_alloc(type);
        if (mutex != NULL) {
            int try_rc = sqlite3_mutex_try(mutex);
            (void)try_rc; /* Check return value */
            if (try_rc == SQLITE_OK) {
                sqlite3_mutex_leave(mutex);
            }
            cleanup_mutex(mutex);
        }

    } else if (scenario == 4) {
        /* Scenario 4: Test sqlite3_db_mutex with NULL (should be safe or handled) */
        /* Note: Passing NULL to sqlite3_db_mutex is undefined behavior in some contexts,
         * but the API description says "Pass a valid sqlite3* pointer".
         * However, to explore states, we might try to open a DB and pass it,
         * or just skip NULL test if it's strictly forbidden.
         * Let's try to open a DB in memory and test. */
        rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK) {
            sqlite3_mutex *m = sqlite3_db_mutex(db);
            (void)m;
            cleanup_db(db);
        }
    }

    /* Cleanup */
    sqlite3_shutdown();

    return 0;
}
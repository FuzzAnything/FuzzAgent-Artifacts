// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite mutex and connection mutex APIs.
 * Target APIs:
 * 1. sqlite3_db_mutex
 * 2. sqlite3_mutex_alloc
 * 3. sqlite3_mutex_enter
 * 4. sqlite3_mutex_leave
 * 5. sqlite3_mutex_free
 * 6. sqlite3_mutex_try
 */

/*
 * Helper to safely close a database connection if it is valid.
 */
static void safe_close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_mutex *m1 = NULL;
    sqlite3_mutex *m2 = NULL;
    
    /* Initialize SQLite if not already done */
    sqlite3_initialize();

    /*
     * Use the first byte to decide whether to create a database connection.
     * Creating a connection is necessary to test sqlite3_db_mutex effectively.
     */
    if (Size > 0 && Data[0] % 2 == 0) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            db = NULL;
        }
    }

    /*
     * Use the second byte (if available) to decide mutex allocation types.
     * SQLITE_MUTEX_FAST (1) and SQLITE_MUTEX_RECURSIVE (2) are dynamic.
     * Static mutexes (e.g., SQLITE_MUTEX_STATIC_MAIN) are also valid inputs for alloc
     * but must not be freed.
     */
    int type1 = 0;
    int type2 = 0;

    if (Size > 1) {
        type1 = Data[1] % 3; 
        if (type1 == 0) type1 = SQLITE_MUTEX_FAST;
        else if (type1 == 1) type1 = SQLITE_MUTEX_RECURSIVE;
        else type1 = SQLITE_MUTEX_STATIC_MAIN;
    } else {
        type1 = SQLITE_MUTEX_FAST;
    }

    if (Size > 2) {
        type2 = Data[2] % 3;
        if (type2 == 0) type2 = SQLITE_MUTEX_FAST;
        else if (type2 == 1) type2 = SQLITE_MUTEX_RECURSIVE;
        else type2 = SQLITE_MUTEX_STATIC_MAIN;
    } else {
        type2 = SQLITE_MUTEX_FAST;
    }

    /*
     * Allocate mutexes based on the fuzzed types.
     * We only track dynamic ones for freeing later.
     */
    int is_dynamic1 = (type1 == SQLITE_MUTEX_FAST || type1 == SQLITE_MUTEX_RECURSIVE);
    int is_dynamic2 = (type2 == SQLITE_MUTEX_FAST || type2 == SQLITE_MUTEX_RECURSIVE);

    if (is_dynamic1) {
        m1 = sqlite3_mutex_alloc(type1);
    } else {
        m1 = sqlite3_mutex_alloc(type1);
    }

    if (is_dynamic2) {
        m2 = sqlite3_mutex_alloc(type2);
    } else {
        m2 = sqlite3_mutex_alloc(type2);
    }

    /*
     * Test sqlite3_db_mutex
     * IMPORTANT: sqlite3_db_mutex does NOT handle NULL pointers safely in all builds.
     * It attempts to dereference the pointer to access the mutex field.
     * We must only call it with a valid, non-NULL sqlite3* pointer.
     */
    if (db != NULL) {
        sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
        /* db_mutex might be NULL if not in Serialized mode. This is expected. */
        if (db_mutex) {
            sqlite3_mutex_enter(db_mutex);
            /* Do some work inside the lock */
            sqlite3_mutex_leave(db_mutex);
        }
    }
    /* Do NOT call sqlite3_db_mutex(NULL) as it causes a segfault in the library implementation */

    /* Test sqlite3_mutex_enter/leave/try/free logic */
    
    /* Scenario A: Enter and Leave m1 */
    if (m1) {
        sqlite3_mutex_enter(m1);
        sqlite3_mutex_leave(m1);
    }

    /* Scenario B: Try to enter m2 (non-blocking) */
    if (m2) {
        int try_result = sqlite3_mutex_try(m2);
        (void)try_result; 
        if (try_result == SQLITE_OK) {
            sqlite3_mutex_leave(m2);
        }
    }

    /* Scenario C: Nested locks (if recursive) */
    if (m1 && type1 == SQLITE_MUTEX_RECURSIVE) {
        sqlite3_mutex_enter(m1);
        sqlite3_mutex_enter(m1);
        sqlite3_mutex_leave(m1);
        sqlite3_mutex_leave(m1);
    }

    /* Scenario D: Test with NULL pointers for mutex functions (these ARE safe) */
    sqlite3_mutex_enter(NULL);
    sqlite3_mutex_leave(NULL);
    sqlite3_mutex_free(NULL);
    int null_try = sqlite3_mutex_try(NULL);
    (void)null_try;

    /* Scenario E: Fuzzed loop for stress testing */
    /* 
     * FIX: The previous crash occurred because we called sqlite3_mutex_try(m2) 
     * while m1 was held, and m2 was the same mutex as m1 (if type1 == type2 and both dynamic).
     * If m1 is held and is NOT recursive, sqlite3_mutex_try(m2) triggers an assertion failure
     * inside the library's mutex implementation (pthreadMutexNotheld check).
     * 
     * To fix this, we must ensure that we do not call try/enter/leave on a mutex that is 
     * currently held by the same thread unless it is explicitly recursive.
     * However, since we don't know if m1 and m2 are the same object (they could be if we 
     * allocated static mutexes or if the allocator reuses memory, though unlikely for dynamic),
     * the safest approach in a fuzzer is to avoid nesting operations on potentially shared 
     * resources without explicit knowledge of their state.
     * 
     * Revised Strategy: Only perform the nested try if we are sure m2 is NOT currently held.
     * Since we just released m1 in Scenario A, m1 is free. But if m1 == m2, we are fine.
     * The crash happened because in the loop, we enter m1, then try m2. 
     * If m1 == m2 and it's not recursive, try fails assertion.
     * 
     * Correction: We must check if m1 and m2 are the same pointer. 
     * If they are the same, we cannot try m2 while m1 is held unless it is recursive.
     */
    if (Size > 3) {
        int iterations = Data[3] % 10;
        for (int i = 0; i < iterations; i++) {
            if (m1) {
                sqlite3_mutex_enter(m1);
                if (m2) {
                    /* 
                     * Safety check: If m1 and m2 are the same mutex, 
                     * we can only try to enter m2 if m1 is recursive.
                     * Otherwise, the library will assert.
                     */
                    if (m1 != m2 || type1 == SQLITE_MUTEX_RECURSIVE) {
                        if (sqlite3_mutex_try(m2) == SQLITE_OK) {
                             sqlite3_mutex_leave(m2);
                        }
                    }
                }
                sqlite3_mutex_leave(m1);
            }
        }
    }

    /*
     * Cleanup.
     * Free dynamic mutexes. Close database.
     * CRITICAL: Ensure no mutex is held before freeing.
     * The previous logic ensures we leave any mutex we entered.
     * However, we must be careful not to free static mutexes.
     */
    
    if (is_dynamic1 && m1) {
        sqlite3_mutex_free(m1);
    }
    if (is_dynamic2 && m2) {
        sqlite3_mutex_free(m2);
    }

    safe_close_db(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to safely open a database connection for fuzzing.
 * Returns NULL on failure.
 */
static sqlite3 *fuzz_open_db(const char *filename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Static helper to safely close a database connection.
 */
static void fuzz_close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Initialize SQLite mutex subsystem if needed.
     */
    sqlite3_initialize();

    /* 
     * Use the input data to determine the type of fuzzing operation.
     * We will fuzz:
     * 1. Dynamic mutex allocation (FAST vs RECURSIVE)
     * 2. Mutex enter/leave/try operations
     * 3. Database connection mutex retrieval
     */
    
    int op_type = Data[0] % 4;
    
    if (op_type == 0) {
        /* 
         * Fuzz sqlite3_mutex_alloc and sqlite3_mutex_free 
         * with dynamic mutex types.
         */
        int type;
        if (Size > 1) {
            type = (Data[1] % 2 == 0) ? SQLITE_MUTEX_FAST : SQLITE_MUTEX_RECURSIVE;
        } else {
            type = SQLITE_MUTEX_FAST;
        }
        
        sqlite3_mutex *m = sqlite3_mutex_alloc(type);
        
        if (m != NULL) {
            /* 
             * Perform some enter/leave cycles based on input data.
             */
            int iterations;
            if (Size > 1) {
                iterations = (Data[1] % 10) + 1;
            } else {
                iterations = 1;
            }
            
            for (int i = 0; i < iterations; i++) {
                sqlite3_mutex_enter(m);
                
                /* 
                 * Only re-enter if the mutex is RECURSIVE.
                 * For FAST mutexes, re-entering without leaving causes undefined behavior/asserts.
                 */
                if (Size > 2 && (Data[2] % 2 == 0)) {
                    if (type == SQLITE_MUTEX_RECURSIVE) {
                        sqlite3_mutex_enter(m);
                        sqlite3_mutex_leave(m);
                    }
                }
                
                sqlite3_mutex_leave(m);
            }
            
            /* Clean up */
            sqlite3_mutex_free(m);
        }
        
        /* 
         * Also test with NULL pointer to ensure no-op behavior.
         */
        sqlite3_mutex_free(NULL);
        sqlite3_mutex_enter(NULL);
        sqlite3_mutex_leave(NULL);
        sqlite3_mutex_try(NULL);

    } else if (op_type == 1) {
        /* 
         * Fuzz sqlite3_mutex_try and sqlite3_mutex_enter/leave logic.
         */
        int type;
        if (Size > 1) {
            type = (Data[1] % 2 == 0) ? SQLITE_MUTEX_FAST : SQLITE_MUTEX_RECURSIVE;
        } else {
            type = SQLITE_MUTEX_FAST;
        }
        
        sqlite3_mutex *m = sqlite3_mutex_alloc(type);
        
        if (m != NULL) {
            /* Try to lock */
            int rc = sqlite3_mutex_try(m);
            (void)rc; 
            
            /* If try succeeded, we must leave */
            if (rc == SQLITE_OK) {
                sqlite3_mutex_leave(m);
            }
            
            /* Enter and leave normally */
            sqlite3_mutex_enter(m);
            sqlite3_mutex_leave(m);
            
            sqlite3_mutex_free(m);
        }

    } else if (op_type == 2) {
        /* 
         * Fuzz sqlite3_db_mutex.
         * This requires a valid database connection.
         */
        sqlite3 *db = fuzz_open_db("./dummy_file");
        
        if (db != NULL) {
            sqlite3_mutex *m = sqlite3_db_mutex(db);
            
            if (m != NULL) {
                sqlite3_mutex_enter(m);
                sqlite3_mutex_leave(m);
            }
            
            fuzz_close_db(db);
        } else {
            sqlite3_mutex *m = sqlite3_db_mutex(NULL);
            (void)m;
        }

    } else {
        /* 
         * Fuzz static mutex types (should not be freed).
         */
        int static_type;
        if (Size > 1) {
            static_type = SQLITE_MUTEX_STATIC_MAIN + (Data[1] % 14);
        } else {
            static_type = SQLITE_MUTEX_STATIC_MAIN;
        }
        
        /* Ensure we stay within valid static range */
        if (static_type >= SQLITE_MUTEX_STATIC_MAIN && static_type <= SQLITE_MUTEX_STATIC_VFS3) {
            sqlite3_mutex *m = sqlite3_mutex_alloc(static_type);
            if (m != NULL) {
                sqlite3_mutex_enter(m);
                sqlite3_mutex_leave(m);
                /* Do NOT call sqlite3_mutex_free(m) for static mutexes */
            }
        }
    }

    /* 
     * Cleanup: Shutdown SQLite to reset global state for next iteration.
     */
    sqlite3_shutdown();

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Helper to safely open a database connection for testing sqlite3_db_mutex.
 * Uses a dummy file as per instructions.
 */
static sqlite3 *open_test_db(void) {
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    
    /* Ensure the dummy file exists and is writable */
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "dummy data for fuzzing\n");
        fclose(f);
    }

    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* If open fails, return NULL which is valid for sqlite3_db_mutex testing */
        return NULL;
    }
    return db;
}

/* 
 * Helper to safely close a database connection.
 */
static void close_test_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    /* 
     * Safety check: Ensure we have enough data to read indices safely.
     * If Size is too small, we can still run basic tests with default values.
     */
    if (Size == 0) {
        /* Test NULL handling even with empty input */
        sqlite3_mutex_free(NULL);
        sqlite3_mutex_enter(NULL);
        sqlite3_mutex_leave(NULL);
        sqlite3_mutex_try(NULL);
        /* Do not call sqlite3_db_mutex(NULL) as it may dereference NULL internally in some builds */
        return 0;
    }

    /* 
     * Extract parameters from input data to drive fuzzing diversity.
     * We use the first few bytes to determine operations and types.
     * Check bounds before accessing Data[1] and Data[2].
     */
    int op_type = Data[0] % 6; /* 0-5: select operation */
    int mutex_type = SQLITE_MUTEX_FAST;
    
    if (Size > 1) {
        mutex_type = (Data[1] % 2) ? SQLITE_MUTEX_RECURSIVE : SQLITE_MUTEX_FAST;
    }
    
    /* Initialize mutex pointer */
    sqlite3_mutex *mutex = NULL;
    sqlite3 *db = NULL;
    int rc;

    /* 
     * Case 0: Test sqlite3_mutex_alloc and sqlite3_mutex_free
     */
    if (op_type == 0) {
        mutex = sqlite3_mutex_alloc(mutex_type);
        if (mutex != NULL) {
            sqlite3_mutex_free(mutex);
            mutex = NULL;
        }
    }
    
    /* 
     * Case 1: Test sqlite3_mutex_alloc, enter, leave, free
     */
    else if (op_type == 1) {
        mutex = sqlite3_mutex_alloc(mutex_type);
        if (mutex != NULL) {
            sqlite3_mutex_enter(mutex);
            sqlite3_mutex_leave(mutex);
            sqlite3_mutex_free(mutex);
            mutex = NULL;
        }
    }
    
    /* 
     * Case 2: Test sqlite3_mutex_try
     */
    else if (op_type == 2) {
        mutex = sqlite3_mutex_alloc(mutex_type);
        if (mutex != NULL) {
            rc = sqlite3_mutex_try(mutex);
            /* Try to enter again if it's recursive */
            if (mutex_type == SQLITE_MUTEX_RECURSIVE) {
                rc = sqlite3_mutex_try(mutex);
                sqlite3_mutex_leave(mutex);
            }
            if (rc == SQLITE_OK) {
                sqlite3_mutex_leave(mutex);
            }
            sqlite3_mutex_free(mutex);
            mutex = NULL;
        }
    }
    
    /* 
     * Case 3: Test sqlite3_db_mutex with a valid database connection
     */
    else if (op_type == 3) {
        db = open_test_db();
        if (db != NULL) {
            sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
            /* 
             * If db_mutex is not NULL (Serialized mode), we can test enter/leave
             * Note: We do not free db_mutex as it is a static mutex owned by SQLite
             */
            if (db_mutex != NULL) {
                sqlite3_mutex_enter(db_mutex);
                sqlite3_mutex_leave(db_mutex);
            }
            close_test_db(db);
        }
    }
    
    /* 
     * Case 4: Test sqlite3_db_mutex with NULL database connection
     * Note: We skip this case if it causes issues, as some builds may not handle NULL gracefully
     * Instead, we test with a valid DB that might return NULL mutex (non-serialized mode)
     */
    else if (op_type == 4) {
        db = open_test_db();
        if (db != NULL) {
            sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
            /* db_mutex might be NULL in non-serialized mode, which is safe to pass to enter/leave */
            sqlite3_mutex_enter(db_mutex);
            sqlite3_mutex_leave(db_mutex);
            close_test_db(db);
        }
    }
    
    /* 
     * Case 5: Test NULL pointer handling for all mutex functions
     */
    else if (op_type == 5) {
        /* Test with NULL pointers */
        sqlite3_mutex_free(NULL);
        sqlite3_mutex_enter(NULL);
        sqlite3_mutex_leave(NULL);
        rc = sqlite3_mutex_try(NULL);
        (void)rc; /* Suppress unused variable warning */
    }

    /* 
     * Additional exploration: Try to allocate multiple mutexes and perform operations
     * This helps explore state where multiple mutexes exist simultaneously.
     * Only proceed if we have enough data to determine the count.
     */
    if (Size > 2) {
        int count = Data[2] % 5;
        sqlite3_mutex *m[5];
        for (int i = 0; i < count; i++) {
            m[i] = sqlite3_mutex_alloc((i % 2) ? SQLITE_MUTEX_RECURSIVE : SQLITE_MUTEX_FAST);
        }
        
        /* Enter all allocated mutexes */
        for (int i = 0; i < count; i++) {
            if (m[i] != NULL) {
                sqlite3_mutex_enter(m[i]);
            }
        }
        
        /* Leave all allocated mutexes */
        for (int i = 0; i < count; i++) {
            if (m[i] != NULL) {
                sqlite3_mutex_leave(m[i]);
            }
        }
        
        /* Free all allocated mutexes */
        for (int i = 0; i < count; i++) {
            if (m[i] != NULL) {
                sqlite3_mutex_free(m[i]);
            }
        }
    }

    return 0;
}
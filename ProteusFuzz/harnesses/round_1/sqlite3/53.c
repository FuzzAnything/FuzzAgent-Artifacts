// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fuzz driver for sqlite3 mutex and database connection mutex APIs.
 * Target APIs:
 *  - sqlite3_mutex_alloc
 *  - sqlite3_mutex_free
 *  - sqlite3_mutex_enter
 *  - sqlite3_mutex_leave
 *  - sqlite3_mutex_try
 *  - sqlite3_db_mutex
 */

/* Helper to safely open a database connection for testing sqlite3_db_mutex */
static sqlite3 *fuzz_open_db(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    /* Use a dummy file path as per instructions */
    const char *filename = "./dummy_file";
    
    /* Ensure we have some data to write to the dummy file to make it valid */
    FILE *f = fopen(filename, "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        } else {
            /* Write a minimal header if input is empty to avoid empty file issues */
            const char *header = "SQLite format 3\000";
            fwrite(header, 1, strlen(header), f);
        }
        fclose(f);
    }

    if (sqlite3_open(filename, &db) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/* Helper to safely close a database connection */
static void fuzz_close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize SQLite if necessary (though mutex alloc often does this) */
    sqlite3_initialize();

    /* 
     * Strategy:
     * 1. Use the input data to determine which mutex types to allocate.
     * 2. Perform various lock/unlock operations.
     * 3. Test sqlite3_db_mutex with a real connection.
     * 4. Ensure proper cleanup to avoid leaks or crashes.
     */

    /* Array to store dynamically allocated mutexes for cleanup */
    sqlite3_mutex *dynamic_mutexes[10];
    int dynamic_count = 0;

    /* Iterate through the input data to generate test scenarios */
    for (size_t i = 0; i < Size; i++) {
        uint8_t val = Data[i];
        
        /* Determine mutex type based on input byte */
        int type = val % 12; /* 0-11 covers SQLITE_MUTEX_FAST, RECURSIVE, and some static ones */
        
        /* Map to valid constants */
        int alloc_type;
        if (type == 0) alloc_type = SQLITE_MUTEX_FAST;
        else if (type == 1) alloc_type = SQLITE_MUTEX_RECURSIVE;
        else if (type == 2) alloc_type = SQLITE_MUTEX_STATIC_MAIN;
        else if (type == 3) alloc_type = SQLITE_MUTEX_STATIC_MEM;
        else if (type == 4) alloc_type = SQLITE_MUTEX_STATIC_OPEN;
        else if (type == 5) alloc_type = SQLITE_MUTEX_STATIC_PRNG;
        else if (type == 6) alloc_type = SQLITE_MUTEX_STATIC_LRU;
        else if (type == 7) alloc_type = SQLITE_MUTEX_STATIC_PMEM;
        else if (type == 8) alloc_type = SQLITE_MUTEX_STATIC_APP1;
        else if (type == 9) alloc_type = SQLITE_MUTEX_STATIC_APP2;
        else if (type == 10) alloc_type = SQLITE_MUTEX_STATIC_APP3;
        else alloc_type = SQLITE_MUTEX_STATIC_VFS1;

        sqlite3_mutex *mx = sqlite3_mutex_alloc(alloc_type);

        if (mx == NULL) {
            /* Allocation failed, skip operations for this iteration */
            continue;
        }

        /* Track dynamic mutexes for cleanup. Static mutexes should not be freed. */
        if (alloc_type == SQLITE_MUTEX_FAST || alloc_type == SQLITE_MUTEX_RECURSIVE) {
            if (dynamic_count < 10) {
                dynamic_mutexes[dynamic_count++] = mx;
            }
        }

        /* Perform lock operations based on subsequent bytes if available */
        if (i + 1 < Size) {
            uint8_t op = Data[i + 1];
            if (op % 3 == 0) {
                /* Enter mutex */
                sqlite3_mutex_enter(mx);
            } else if (op % 3 == 1) {
                /* Try to enter mutex */
                sqlite3_mutex_try(mx);
            }
        }

        if (i + 2 < Size) {
            uint8_t op2 = Data[i + 2];
            if (op2 % 2 == 0) {
                /* Leave mutex */
                sqlite3_mutex_leave(mx);
            }
        }
    }

    /* Test sqlite3_db_mutex with a database connection */
    sqlite3 *db = fuzz_open_db(Data, Size);
    if (db) {
        sqlite3_mutex *db_mx = sqlite3_db_mutex(db);
        
        /* If we got a mutex, try to interact with it */
        if (db_mx != NULL) {
            /* Attempt to enter/leave based on input data */
            if (Size > 0 && Data[0] % 2 == 0) {
                sqlite3_mutex_enter(db_mx);
            }
            if (Size > 1 && Data[1] % 2 == 0) {
                sqlite3_mutex_leave(db_mx);
            }
            /* Try non-blocking */
            if (Size > 2 && Data[2] % 2 == 0) {
                sqlite3_mutex_try(db_mx);
            }
        }
        
        fuzz_close_db(db);
    }

    /* Cleanup dynamic mutexes */
    for (int i = 0; i < dynamic_count; i++) {
        if (dynamic_mutexes[i] != NULL) {
            /* Ensure we don't free a held mutex if possible, but free regardless for fuzzing safety */
            sqlite3_mutex_free(dynamic_mutexes[i]);
        }
    }

    /* Shutdown SQLite */
    sqlite3_shutdown();

    return 0;
}
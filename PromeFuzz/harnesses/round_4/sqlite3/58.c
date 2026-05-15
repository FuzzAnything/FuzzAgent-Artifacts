// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:171772:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_mutex_operations(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create a database connection for testing
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return;
    }
    
    // Use the first byte to determine which mutex type to allocate
    int mutex_type = data[0] % 15; // 15 possible mutex types (0-14)
    
    // Map to actual mutex types
    static const int mutex_types[] = {
        SQLITE_MUTEX_FAST,
        SQLITE_MUTEX_RECURSIVE,
        SQLITE_MUTEX_STATIC_MAIN,
        SQLITE_MUTEX_STATIC_MEM,
        SQLITE_MUTEX_STATIC_OPEN,
        SQLITE_MUTEX_STATIC_PRNG,
        SQLITE_MUTEX_STATIC_LRU,
        SQLITE_MUTEX_STATIC_PMEM,
        SQLITE_MUTEX_STATIC_APP1,
        SQLITE_MUTEX_STATIC_APP2,
        SQLITE_MUTEX_STATIC_APP3,
        SQLITE_MUTEX_STATIC_VFS1,
        SQLITE_MUTEX_STATIC_VFS2,
        SQLITE_MUTEX_STATIC_VFS3,
        SQLITE_MUTEX_FAST // Duplicate for more coverage
    };
    
    int actual_type = mutex_types[mutex_type % (sizeof(mutex_types)/sizeof(mutex_types[0]))];
    
    // Test sqlite3_mutex_alloc
    sqlite3_mutex *mutex = sqlite3_mutex_alloc(actual_type);
    
    if (mutex) {
        // Test sqlite3_mutex_enter and sqlite3_mutex_leave
        sqlite3_mutex_enter(mutex);
        sqlite3_mutex_leave(mutex);
        
        // Test sqlite3_mutex_try
        int try_result = sqlite3_mutex_try(mutex);
        if (try_result == SQLITE_OK) {
            sqlite3_mutex_leave(mutex);
        }
        
        // Test sqlite3_mutex_free for dynamic mutexes
        // Only free dynamic mutexes (FAST and RECURSIVE)
        if (actual_type == SQLITE_MUTEX_FAST || actual_type == SQLITE_MUTEX_RECURSIVE) {
            sqlite3_mutex_free(mutex);
        }
    }
    
    // Test sqlite3_db_mutex
    sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
    if (db_mutex) {
        // Try some operations on the database mutex if it exists
        sqlite3_mutex_enter(db_mutex);
        sqlite3_mutex_leave(db_mutex);
        
        int try_result = sqlite3_mutex_try(db_mutex);
        if (try_result == SQLITE_OK) {
            sqlite3_mutex_leave(db_mutex);
        }
    }
    
    // Test with NULL pointers (should be safe no-ops)
    sqlite3_mutex_free(NULL);
    sqlite3_mutex_enter(NULL);
    sqlite3_mutex_leave(NULL);
    sqlite3_mutex_try(NULL);
    
    // Clean up
    sqlite3_close(db);
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Initialize SQLite if needed
    static int initialized = 0;
    if (!initialized) {
        sqlite3_initialize();
        initialized = 1;
    }
    
    fuzz_mutex_operations(Data, Size);
    
    return 0;
}
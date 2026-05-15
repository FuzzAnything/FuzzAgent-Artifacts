// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
    SQLITE_MUTEX_STATIC_VFS3
};

static void test_mutex_operations(sqlite3_mutex *mutex, const uint8_t *data, size_t size) {
    if (mutex == NULL) return;
    
    // Try to enter the mutex
    sqlite3_mutex_enter(mutex);
    
    // Check if held (should be true after entering)
    int held = sqlite3_mutex_held(mutex);
    
    // Try to lock without blocking (should return SQLITE_BUSY since we already hold it)
    int try_result = sqlite3_mutex_try(mutex);
    
    // Leave the mutex
    sqlite3_mutex_leave(mutex);
    
    // Check if held (should be false after leaving)
    held = sqlite3_mutex_held(mutex);
    
    // Try to lock again (might succeed now)
    try_result = sqlite3_mutex_try(mutex);
    if (try_result == SQLITE_OK) {
        sqlite3_mutex_leave(mutex);
    }
    
    // Test recursive locking for recursive mutexes
    // We can't easily determine if mutex is recursive, so we'll just test
    // based on fuzzer input
    if (size > 0 && (data[0] & 0x01)) {
        sqlite3_mutex_enter(mutex);
        sqlite3_mutex_enter(mutex);  // Second enter
        sqlite3_mutex_leave(mutex);
        sqlite3_mutex_leave(mutex);
    }
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Select mutex type based on input
    size_t type_index = Data[0] % (sizeof(mutex_types) / sizeof(mutex_types[0]));
    int mutex_type = mutex_types[type_index];
    
    // Allocate mutex
    sqlite3_mutex *mutex = sqlite3_mutex_alloc(mutex_type);
    
    if (mutex != NULL) {
        // Test mutex operations
        test_mutex_operations(mutex, Data, Size);
        
        // Only free dynamic mutexes (FAST and RECURSIVE)
        if (mutex_type == SQLITE_MUTEX_FAST || mutex_type == SQLITE_MUTEX_RECURSIVE) {
            sqlite3_mutex_free(mutex);
        }
        
        // Test with NULL mutex (should be safe no-ops)
        sqlite3_mutex_enter(NULL);
        sqlite3_mutex_try(NULL);
        sqlite3_mutex_leave(NULL);
        sqlite3_mutex_held(NULL);
        sqlite3_mutex_free(NULL);
    }
    
    // Test allocation with different types based on input
    if (Size > 1) {
        for (size_t i = 1; i < Size && i < sizeof(mutex_types) / sizeof(mutex_types[0]); i++) {
            int alt_type = mutex_types[Data[i] % (sizeof(mutex_types) / sizeof(mutex_types[0]))];
            sqlite3_mutex *alt_mutex = sqlite3_mutex_alloc(alt_type);
            if (alt_mutex != NULL) {
                // Quick test
                sqlite3_mutex_enter(alt_mutex);
                sqlite3_mutex_leave(alt_mutex);
                
                // Only free dynamic mutexes
                if (alt_type == SQLITE_MUTEX_FAST || alt_type == SQLITE_MUTEX_RECURSIVE) {
                    sqlite3_mutex_free(alt_mutex);
                }
            }
        }
    }
    
    return 0;
}
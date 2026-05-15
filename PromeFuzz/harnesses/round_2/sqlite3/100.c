// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:174875:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    // Use first byte to determine mutex type for allocation
    if (Size == 0) return 0;
    
    uint8_t mutex_type_selector = Data[0] % 15;
    int mutex_type;
    
    // Map selector to actual mutex type constants
    switch (mutex_type_selector) {
        case 0: mutex_type = SQLITE_MUTEX_FAST; break;
        case 1: mutex_type = SQLITE_MUTEX_RECURSIVE; break;
        case 2: mutex_type = SQLITE_MUTEX_STATIC_MAIN; break;
        case 3: mutex_type = SQLITE_MUTEX_STATIC_MEM; break;
        case 4: mutex_type = SQLITE_MUTEX_STATIC_OPEN; break;
        case 5: mutex_type = SQLITE_MUTEX_STATIC_PRNG; break;
        case 6: mutex_type = SQLITE_MUTEX_STATIC_LRU; break;
        case 7: mutex_type = SQLITE_MUTEX_STATIC_PMEM; break;
        case 8: mutex_type = SQLITE_MUTEX_STATIC_APP1; break;
        case 9: mutex_type = SQLITE_MUTEX_STATIC_APP2; break;
        case 10: mutex_type = SQLITE_MUTEX_STATIC_APP3; break;
        case 11: mutex_type = SQLITE_MUTEX_STATIC_VFS1; break;
        case 12: mutex_type = SQLITE_MUTEX_STATIC_VFS2; break;
        case 13: mutex_type = SQLITE_MUTEX_STATIC_VFS3; break;
        default: mutex_type = SQLITE_MUTEX_FAST; break;
    }
    
    // Allocate mutex
    sqlite3_mutex *mutex = sqlite3_mutex_alloc(mutex_type);
    
    // Use second byte to determine which operations to perform
    if (Size > 1) {
        uint8_t operation_selector = Data[1] % 8;
        
        switch (operation_selector) {
            case 0:
                // Test thread cleanup
                sqlite3_thread_cleanup();
                break;
            case 1:
                // Test mutex held check
                if (mutex != NULL) {
                    sqlite3_mutex_held(mutex);
                }
                break;
            case 2:
                // Test mutex enter
                sqlite3_mutex_enter(mutex);
                break;
            case 3:
                // Test mutex try
                if (mutex != NULL) {
                    sqlite3_mutex_try(mutex);
                }
                break;
            case 4:
                // Test mutex notheld
                if (mutex != NULL) {
                    sqlite3_mutex_notheld(mutex);
                }
                break;
            case 5:
                // Test combination: enter and check
                sqlite3_mutex_enter(mutex);
                if (mutex != NULL) {
                    sqlite3_mutex_held(mutex);
                    sqlite3_mutex_notheld(mutex);
                }
                break;
            case 6:
                // Test try and check
                if (mutex != NULL) {
                    int try_result = sqlite3_mutex_try(mutex);
                    (void)try_result; // Use result to avoid unused warning
                    sqlite3_mutex_held(mutex);
                }
                break;
            case 7:
                // Test all operations
                sqlite3_thread_cleanup();
                if (mutex != NULL) {
                    sqlite3_mutex_enter(mutex);
                    sqlite3_mutex_try(mutex);
                    sqlite3_mutex_held(mutex);
                    sqlite3_mutex_notheld(mutex);
                }
                break;
        }
    }
    
    // Test with NULL mutex pointer (should be no-op according to docs)
    if (Size > 2 && (Data[2] % 2 == 0)) {
        sqlite3_mutex_enter(NULL);
        sqlite3_mutex_try(NULL);
        sqlite3_mutex_held(NULL);
        sqlite3_mutex_notheld(NULL);
    }
    
    // Free the allocated mutex to avoid memory leaks
    // According to SQLite documentation, we can only free dynamic mutexes
    // Static mutexes should not be freed. We need to check the mutex type.
    if (mutex != NULL) {
        // Check if it's a dynamic mutex (FAST or RECURSIVE)
        if (mutex_type == SQLITE_MUTEX_FAST || mutex_type == SQLITE_MUTEX_RECURSIVE) {
            // We need to use sqlite3_mutex_free to deallocate dynamic mutexes
            // However, this function is not in our target API list
            // We'll include it anyway to fix the memory leak issue
            sqlite3_mutex_free(mutex);
        }
        // For static mutexes, we don't free them (undefined behavior per docs)
    }
    
    return 0;
}
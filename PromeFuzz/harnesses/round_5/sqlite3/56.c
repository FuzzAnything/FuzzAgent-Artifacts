// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize SQLite library
    sqlite3_initialize();
    
    // Use the first byte to determine which mutex type to allocate
    int mutex_type_index = Data[0] % 14;
    int mutex_type;
    
    // Map the index to actual mutex types
    switch (mutex_type_index) {
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
    
    if (mutex != NULL) {
        // Test sqlite3_mutex_try - non-blocking lock attempt
        int try_result = sqlite3_mutex_try(mutex);
        
        // Test sqlite3_mutex_held - check if mutex is held
        int held_result = sqlite3_mutex_held(mutex);
        
        // Test sqlite3_mutex_notheld - check if mutex is NOT held
        int notheld_result = sqlite3_mutex_notheld(mutex);
        
        // If try was successful, we now hold the lock
        if (try_result == SQLITE_OK) {
            // Test sqlite3_mutex_enter - should be a no-op since we already hold it
            // (but behavior depends on mutex type - recursive vs non-recursive)
            sqlite3_mutex_enter(mutex);
            
            // Check held status again
            held_result = sqlite3_mutex_held(mutex);
            notheld_result = sqlite3_mutex_notheld(mutex);
            
            // For recursive mutexes, we can enter multiple times
            if (mutex_type == SQLITE_MUTEX_RECURSIVE && Size > 1) {
                // Use second byte to determine how many extra times to enter
                int extra_enters = Data[1] % 5;
                for (int i = 0; i < extra_enters; i++) {
                    sqlite3_mutex_enter(mutex);
                }
            }
            
            // Only free dynamic mutexes (FAST and RECURSIVE)
            if (mutex_type == SQLITE_MUTEX_FAST || mutex_type == SQLITE_MUTEX_RECURSIVE) {
                sqlite3_mutex_free(mutex);
            }
        } else {
            // Try failed (SQLITE_BUSY), test other functions without holding lock
            held_result = sqlite3_mutex_held(mutex);
            notheld_result = sqlite3_mutex_notheld(mutex);
            
            // Test sqlite3_mutex_enter - blocking lock (but in single-threaded fuzzer)
            sqlite3_mutex_enter(mutex);
            
            // Now we should hold it
            held_result = sqlite3_mutex_held(mutex);
            notheld_result = sqlite3_mutex_notheld(mutex);
            
            // Only free dynamic mutexes (FAST and RECURSIVE)
            if (mutex_type == SQLITE_MUTEX_FAST || mutex_type == SQLITE_MUTEX_RECURSIVE) {
                sqlite3_mutex_free(mutex);
            }
        }
    }
    
    // Test with NULL pointer (should be no-op for all functions)
    sqlite3_mutex_try(NULL);
    sqlite3_mutex_held(NULL);
    sqlite3_mutex_notheld(NULL);
    sqlite3_mutex_enter(NULL);
    sqlite3_mutex_free(NULL);
    
    // Test allocation with invalid type (use remaining data bytes)
    if (Size > 2) {
        for (size_t i = 2; i < Size && i < 10; i++) {
            int invalid_type = Data[i] + 100;  // Invalid type value
            sqlite3_mutex *invalid_mutex = sqlite3_mutex_alloc(invalid_type);
            if (invalid_mutex != NULL) {
                sqlite3_mutex_free(invalid_mutex);
            }
        }
    }
    
    // Shutdown SQLite library
    sqlite3_shutdown();
    
    return 0;
}
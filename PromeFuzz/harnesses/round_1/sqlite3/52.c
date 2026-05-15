// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Define all mutex type constants
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
    
    // Use input data to determine mutex type
    int mutex_type;
    if (Size > 0) {
        mutex_type = mutex_types[Data[0] % (sizeof(mutex_types) / sizeof(mutex_types[0]))];
    } else {
        mutex_type = SQLITE_MUTEX_FAST;
    }
    
    // Allocate mutex
    sqlite3_mutex *mutex = sqlite3_mutex_alloc(mutex_type);
    
    if (mutex != NULL) {
        // Test mutex operations
        sqlite3_mutex_enter(mutex);
        
        // Check if held/notheld
        int held = sqlite3_mutex_held(mutex);
        int notheld = sqlite3_mutex_notheld(mutex);
        
        // Leave mutex
        sqlite3_mutex_leave(mutex);
        
        // For dynamic mutexes (FAST/RECURSIVE), we can free them
        if (mutex_type == SQLITE_MUTEX_FAST || mutex_type == SQLITE_MUTEX_RECURSIVE) {
            sqlite3_mutex_free(mutex);
        }
        
        // Test with NULL mutex (should be no-op)
        sqlite3_mutex_enter(NULL);
        sqlite3_mutex_leave(NULL);
        sqlite3_mutex_free(NULL);
        
        // These may return implementation-defined values for NULL
        (void)sqlite3_mutex_held(NULL);
        (void)sqlite3_mutex_notheld(NULL);
    }
    
    // Test allocation of different mutex types based on input data
    if (Size > 1) {
        for (int i = 0; i < (int)Size && i < 14; i++) {
            int test_type = mutex_types[Data[i] % (sizeof(mutex_types) / sizeof(mutex_types[0]))];
            sqlite3_mutex *test_mutex = sqlite3_mutex_alloc(test_type);
            
            if (test_mutex != NULL) {
                // Quick test of enter/leave
                sqlite3_mutex_enter(test_mutex);
                sqlite3_mutex_leave(test_mutex);
                
                // Only free dynamic mutexes
                if (test_type == SQLITE_MUTEX_FAST || test_type == SQLITE_MUTEX_RECURSIVE) {
                    sqlite3_mutex_free(test_mutex);
                }
            }
        }
    }
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
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

static void fuzz_mutex_operations(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Use first byte to determine mutex type for allocation
    int mutex_type = data[0] % 15;  // 15 possible mutex types (0-14)
    
    // Allocate a mutex
    sqlite3_mutex *mutex = sqlite3_mutex_alloc(mutex_type);
    
    if (mutex != NULL) {
        // Try to enter the mutex
        sqlite3_mutex_enter(mutex);
        
        // Check if mutex is held
        int held = sqlite3_mutex_held(mutex);
        
        // Check if mutex is not held (should return false since we just entered it)
        int notheld = sqlite3_mutex_notheld(mutex);
        
        // Try to acquire mutex without blocking
        int try_result = sqlite3_mutex_try(mutex);
        
        // Leave the mutex if we entered it
        // Note: sqlite3_mutex_leave is not in our target list, but we need it
        // to avoid leaving the mutex locked. We'll use it carefully.
        if (held || try_result == SQLITE_OK) {
            // We need to leave the mutex to clean up
            // This is safe even if we didn't successfully enter
            sqlite3_mutex_leave(mutex);
        }
        
        // Free the mutex if it's a dynamic one
        // Only free mutexes of type SQLITE_MUTEX_FAST (0) or SQLITE_MUTEX_RECURSIVE (1)
        if (mutex_type == SQLITE_MUTEX_FAST || mutex_type == SQLITE_MUTEX_RECURSIVE) {
            sqlite3_mutex_free(mutex);
        }
    }
    
    // Test with NULL pointer (should be safe according to documentation)
    sqlite3_mutex_free(NULL);
    sqlite3_mutex_notheld(NULL);
    sqlite3_mutex_enter(NULL);
    sqlite3_mutex_held(NULL);
    sqlite3_mutex_try(NULL);
}

static void fuzz_static_mutexes(const uint8_t *data, size_t size) {
    // Test operations on static mutexes (types 2-14)
    if (size < 2) return;
    
    // Use second byte to select static mutex type (2-14)
    int static_type = 2 + (data[1] % 13);
    
    sqlite3_mutex *static_mutex = sqlite3_mutex_alloc(static_type);
    
    if (static_mutex != NULL) {
        // Perform various operations on static mutex
        sqlite3_mutex_enter(static_mutex);
        sqlite3_mutex_held(static_mutex);
        sqlite3_mutex_notheld(static_mutex);
        sqlite3_mutex_try(static_mutex);
        sqlite3_mutex_leave(static_mutex);
        
        // Note: We don't free static mutexes as per documentation
    }
}

static void fuzz_multiple_allocations(const uint8_t *data, size_t size) {
    // Allocate multiple mutexes based on input data
    if (size < 3) return;
    
    int num_allocations = data[2] % 10;  // Up to 10 allocations
    sqlite3_mutex *mutexes[10];
    
    for (int i = 0; i < num_allocations && i < 10; i++) {
        int type = (data[i % size] + i) % 15;
        mutexes[i] = sqlite3_mutex_alloc(type);
        
        if (mutexes[i] != NULL) {
            // Perform quick test on each mutex
            sqlite3_mutex_enter(mutexes[i]);
            sqlite3_mutex_held(mutexes[i]);
            sqlite3_mutex_leave(mutexes[i]);
        }
    }
    
    // Clean up dynamic mutexes
    for (int i = 0; i < num_allocations && i < 10; i++) {
        if (mutexes[i] != NULL) {
            int type = (data[i % size] + i) % 15;
            if (type == SQLITE_MUTEX_FAST || type == SQLITE_MUTEX_RECURSIVE) {
                sqlite3_mutex_free(mutexes[i]);
            }
        }
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    // Initialize sqlite3 if needed (some implementations require this)
    // sqlite3_initialize();
    
    // Perform various mutex operations
    fuzz_mutex_operations(Data, Size);
    fuzz_static_mutexes(Data, Size);
    fuzz_multiple_allocations(Data, Size);
    
    // Additional fuzzing: use data to influence mutex operations
    if (Size > 0) {
        // Use data bytes to create different scenarios
        for (size_t i = 0; i < Size && i < 20; i++) {
            int mutex_type = Data[i] % 15;
            sqlite3_mutex *m = sqlite3_mutex_alloc(mutex_type);
            
            if (m != NULL) {
                // Mix of operations based on data
                if (Data[i] & 0x01) {
                    sqlite3_mutex_enter(m);
                }
                if (Data[i] & 0x02) {
                    sqlite3_mutex_held(m);
                }
                if (Data[i] & 0x04) {
                    sqlite3_mutex_notheld(m);
                }
                if (Data[i] & 0x08) {
                    sqlite3_mutex_try(m);
                }
                
                // Leave if we entered
                if (Data[i] & 0x01) {
                    sqlite3_mutex_leave(m);
                }
                
                // Free if dynamic
                if (mutex_type == SQLITE_MUTEX_FAST || mutex_type == SQLITE_MUTEX_RECURSIVE) {
                    sqlite3_mutex_free(m);
                }
            }
        }
    }
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:171772:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_mutex_operations(sqlite3_mutex *mutex) {
    if (mutex == NULL) return;
    
    // Try to enter and leave the mutex
    sqlite3_mutex_enter(mutex);
    sqlite3_mutex_leave(mutex);
    
    // Check if mutex is held (likely false since we just left it)
    sqlite3_mutex_held(mutex);
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    // Initialize SQLite if needed
    static int initialized = 0;
    if (!initialized) {
        sqlite3_initialize();
        initialized = 1;
    }
    
    // Create a temporary database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Get database mutex
    sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
    
    // Use fuzzer input to determine which mutex operations to perform
    if (Size > 0) {
        uint8_t operation = Data[0];
        
        // Allocate different types of mutexes based on input
        int mutex_type = SQLITE_MUTEX_FAST;
        if (Size > 1) {
            mutex_type = Data[1] % 14; // 14 possible mutex types
        }
        
        sqlite3_mutex *allocated_mutex = sqlite3_mutex_alloc(mutex_type);
        
        // Perform operations based on input
        switch (operation % 4) {
            case 0:
                // Test database mutex operations
                if (db_mutex != NULL) {
                    dummy_mutex_operations(db_mutex);
                }
                break;
                
            case 1:
                // Test allocated mutex operations
                if (allocated_mutex != NULL) {
                    dummy_mutex_operations(allocated_mutex);
                }
                break;
                
            case 2:
                // Test both mutexes
                if (db_mutex != NULL) {
                    dummy_mutex_operations(db_mutex);
                }
                if (allocated_mutex != NULL) {
                    dummy_mutex_operations(allocated_mutex);
                }
                break;
                
            case 3:
                // Test mutex held check with various mutexes
                if (db_mutex != NULL) {
                    sqlite3_mutex_held(db_mutex);
                }
                if (allocated_mutex != NULL) {
                    sqlite3_mutex_held(allocated_mutex);
                }
                break;
        }
        
        // Free allocated mutex if it's dynamic
        if (allocated_mutex != NULL) {
            // Only free dynamic mutexes (FAST and RECURSIVE)
            if (mutex_type == SQLITE_MUTEX_FAST || mutex_type == SQLITE_MUTEX_RECURSIVE) {
                sqlite3_mutex_free(allocated_mutex);
            }
        }
    }
    
    // Test with NULL mutex pointers (should be safe no-ops)
    sqlite3_mutex_enter(NULL);
    sqlite3_mutex_leave(NULL);
    sqlite3_mutex_free(NULL);
    sqlite3_mutex_held(NULL);
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}
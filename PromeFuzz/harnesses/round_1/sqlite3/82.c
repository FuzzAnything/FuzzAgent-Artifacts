// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:171772:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    // Initialize sqlite3 if needed
    sqlite3_initialize();
    
    // Create a database connection for testing sqlite3_db_mutex
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc == SQLITE_OK && db != NULL) {
        // Test sqlite3_db_mutex - get the mutex for the database connection
        sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
        
        // Test sqlite3_mutex_alloc with various mutex types based on input data
        if (Size > 0) {
            // Use the first byte to determine mutex type (mod 16 for valid types)
            int mutex_type = Data[0] % 16;
            sqlite3_mutex *allocated_mutex = sqlite3_mutex_alloc(mutex_type);
            
            if (allocated_mutex != NULL) {
                // Test sqlite3_mutex_notheld on the allocated mutex
                int not_held = sqlite3_mutex_notheld(allocated_mutex);
                (void)not_held; // Use variable to avoid unused warning
                
                // Test sqlite3_mutex_enter and sqlite3_mutex_leave
                sqlite3_mutex_enter(allocated_mutex);
                sqlite3_mutex_leave(allocated_mutex);
                
                // Test sqlite3_mutex_free for dynamic mutexes
                // Only free dynamic mutexes (FAST and RECURSIVE)
                if (mutex_type == SQLITE_MUTEX_FAST || mutex_type == SQLITE_MUTEX_RECURSIVE) {
                    sqlite3_mutex_free(allocated_mutex);
                }
            }
        }
        
        // Test with additional mutex allocations using different parts of input
        if (Size > 1) {
            // Use second byte for another mutex type
            int mutex_type2 = Data[1 % Size] % 16;
            sqlite3_mutex *mutex2 = sqlite3_mutex_alloc(mutex_type2);
            
            if (mutex2 != NULL) {
                // Test notheld on this mutex too
                int not_held2 = sqlite3_mutex_notheld(mutex2);
                (void)not_held2;
                
                // Test enter/leave multiple times to explore different states
                for (int i = 0; i < 3 && i < (int)Size; i++) {
                    sqlite3_mutex_enter(mutex2);
                    sqlite3_mutex_leave(mutex2);
                }
                
                // Free if dynamic
                if (mutex_type2 == SQLITE_MUTEX_FAST || mutex_type2 == SQLITE_MUTEX_RECURSIVE) {
                    sqlite3_mutex_free(mutex2);
                }
            }
        }
        
        // Test sqlite3_mutex_notheld on database mutex if available
        if (db_mutex != NULL) {
            int db_not_held = sqlite3_mutex_notheld(db_mutex);
            (void)db_not_held;
            
            // Test enter/leave on database mutex
            sqlite3_mutex_enter(db_mutex);
            sqlite3_mutex_leave(db_mutex);
        }
        
        // Test with NULL pointers (should be safe according to documentation)
        sqlite3_mutex_enter(NULL);
        sqlite3_mutex_leave(NULL);
        sqlite3_mutex_free(NULL);
        int null_not_held = sqlite3_mutex_notheld(NULL);
        (void)null_not_held;
        
        // Close the database
        sqlite3_close(db);
    }
    
    // Test sqlite3_mutex_alloc with all static mutex types
    if (Size > 0) {
        // Try a few different static mutex types
        static const int static_mutex_types[] = {
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
        
        size_t num_types = sizeof(static_mutex_types) / sizeof(static_mutex_types[0]);
        for (size_t i = 0; i < num_types && i < Size; i++) {
            sqlite3_mutex *static_mutex = sqlite3_mutex_alloc(static_mutex_types[i]);
            if (static_mutex != NULL) {
                // Test operations on static mutex
                sqlite3_mutex_enter(static_mutex);
                sqlite3_mutex_leave(static_mutex);
                int static_not_held = sqlite3_mutex_notheld(static_mutex);
                (void)static_not_held;
                // Note: Do NOT free static mutexes
            }
        }
    }
    
    // Final cleanup
    sqlite3_shutdown();
    
    return 0;
}
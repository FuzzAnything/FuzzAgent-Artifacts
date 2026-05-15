// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_held at sqlite3.c:15940:16 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_mutex_type_from_input(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return SQLITE_MUTEX_FAST;
    int type = data[*offset] % 15;
    (*offset)++;
    return type;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Test sqlite3_mutex_alloc with various types
    int mutex_type = get_mutex_type_from_input(Data, Size, &offset);
    sqlite3_mutex *mutex1 = sqlite3_mutex_alloc(mutex_type);
    
    // Test with another type
    mutex_type = get_mutex_type_from_input(Data, Size, &offset);
    sqlite3_mutex *mutex2 = sqlite3_mutex_alloc(mutex_type);
    
    // Test sqlite3_mutex_enter and sqlite3_mutex_try
    if (mutex1) {
        sqlite3_mutex_enter(mutex1);
        
        // Try to enter the same mutex (should fail for non-recursive)
        int try_result = sqlite3_mutex_try(mutex1);
        (void)try_result; // Use result to avoid unused variable warning
    }
    
    // Test sqlite3_mutex_held and sqlite3_mutex_notheld
    if (mutex1) {
        int held = sqlite3_mutex_held(mutex1);
        int notheld = sqlite3_mutex_notheld(mutex1);
        (void)held;
        (void)notheld;
    }
    
    if (mutex2) {
        int held = sqlite3_mutex_held(mutex2);
        int notheld = sqlite3_mutex_notheld(mutex2);
        (void)held;
        (void)notheld;
    }
    
    // Test with NULL pointers (should be safe)
    sqlite3_mutex_enter(NULL);
    sqlite3_mutex_try(NULL);
    (void)sqlite3_mutex_held(NULL);
    (void)sqlite3_mutex_notheld(NULL);
    
    // Test sqlite3_mutex_free
    sqlite3_mutex_free(mutex1);
    sqlite3_mutex_free(mutex2);
    sqlite3_mutex_free(NULL); // Should be safe
    
    // Test allocation and immediate free in a loop with different types
    for (int i = 0; i < 5 && offset < Size; i++) {
        mutex_type = get_mutex_type_from_input(Data, Size, &offset);
        sqlite3_mutex *temp_mutex = sqlite3_mutex_alloc(mutex_type);
        
        if (temp_mutex) {
            // Try some operations
            sqlite3_mutex_enter(temp_mutex);
            (void)sqlite3_mutex_try(temp_mutex);
            (void)sqlite3_mutex_held(temp_mutex);
            (void)sqlite3_mutex_notheld(temp_mutex);
            
            // Enter again if recursive type
            if (mutex_type == SQLITE_MUTEX_RECURSIVE) {
                sqlite3_mutex_enter(temp_mutex);
            }
            
            sqlite3_mutex_free(temp_mutex);
        }
    }
    
    // Test with specific static mutex types
    sqlite3_mutex *static_mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_MAIN);
    if (static_mutex) {
        sqlite3_mutex_enter(static_mutex);
        (void)sqlite3_mutex_held(static_mutex);
        (void)sqlite3_mutex_notheld(static_mutex);
        // Note: Do NOT free static mutexes (undefined behavior)
    }
    
    // Test another static type
    static_mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_MEM);
    if (static_mutex) {
        (void)sqlite3_mutex_try(static_mutex);
    }
    
    return 0;
}
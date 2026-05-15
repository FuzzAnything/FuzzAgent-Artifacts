// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:171772:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:15910:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:171772:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
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
#include <stdio.h>

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static sqlite3 *create_test_db(void) {
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        sqlite3_initialize();
        initialized = 1;
    }
    
    if (Size < 1) return 0;
    
    // Use first byte to determine mutex type
    int mutex_type = Data[0] % 15;
    int mutex_types[] = {
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
        999  // Invalid type for edge case
    };
    
    int actual_type = mutex_types[mutex_type % 15];
    
    // Test sqlite3_mutex_alloc
    sqlite3_mutex *mutex = sqlite3_mutex_alloc(actual_type);
    
    // Test sqlite3_mutex_enter if mutex is not NULL
    if (mutex) {
        int try_result = sqlite3_mutex_try(mutex);
        if (try_result == SQLITE_OK) {
            sqlite3_mutex_leave(mutex);
        } else {
            sqlite3_mutex_enter(mutex);
            sqlite3_mutex_leave(mutex);
        }
        
        // Try again after leaving
        if (Size > 1) {
            int second_try = sqlite3_mutex_try(mutex);
            if (second_try == SQLITE_OK) {
                sqlite3_mutex_leave(mutex);
            }
        }
        
        // Test sqlite3_mutex_free for dynamic mutexes
        if (actual_type == SQLITE_MUTEX_FAST || actual_type == SQLITE_MUTEX_RECURSIVE) {
            sqlite3_mutex_free(mutex);
            mutex = NULL;
        }
    }
    
    // Test with database connection
    if (Size > 2) {
        sqlite3 *db = create_test_db();
        if (db) {
            // Test sqlite3_db_mutex
            sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
            
            if (db_mutex) {
                if ((Data[1] & 0x1) && Size > 3) {
                    int db_try = sqlite3_mutex_try(db_mutex);
                    if (db_try == SQLITE_OK) {
                        sqlite3_mutex_leave(db_mutex);
                    }
                }

                sqlite3_mutex_enter(db_mutex);
                sqlite3_mutex_leave(db_mutex);
            }
            
            sqlite3_close(db);
        }
    }
    
    // Test with file operations if we have enough data
    if (Size > 10) {
        dummy_file_write(Data, Size > 1024 ? 1024 : Size);
        
        sqlite3 *file_db = NULL;
        if (sqlite3_open("./dummy_file", &file_db) == SQLITE_OK) {
            sqlite3_mutex *file_mutex = sqlite3_db_mutex(file_db);
            if (file_mutex) {
                int file_try = sqlite3_mutex_try(file_mutex);
                if (file_try == SQLITE_OK) {
                    sqlite3_mutex_leave(file_mutex);
                } else {
                    sqlite3_mutex_enter(file_mutex);
                    sqlite3_mutex_leave(file_mutex);
                }
            }
            sqlite3_close(file_db);
        }
        
        remove("./dummy_file");
    }
    
    // Allocate and free multiple mutexes to test edge cases
    if (Size > 20) {
        for (int i = 0; i < 3 && i < (Size % 10); i++) {
            int idx = (Data[i] + i) % 15;
            int type = mutex_types[idx % 15];
            sqlite3_mutex *m = sqlite3_mutex_alloc(type);
            if (m) {
                if (type == SQLITE_MUTEX_FAST || type == SQLITE_MUTEX_RECURSIVE) {
                    sqlite3_mutex_free(m);
                }
            }
        }
    }
    
    return 0;
}
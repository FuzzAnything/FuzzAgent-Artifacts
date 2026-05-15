// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:15899:17 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:15925:17 in sqlite3.h
// sqlite3_mutex_notheld at sqlite3.c:15949:16 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:15888:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:174693:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:174714:16 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:110746:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:171772:27 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:15867:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

static int dummy_busy_handler(void *arg, int count) {
    return 0;
}

static int dummy_progress_callback(void *arg) {
    return 0;
}

static void dummy_collation_needed(void *arg, sqlite3 *db, int eTextRep, const char *name) {
    // Do nothing
}

static void dummy_collation_needed16(void *arg, sqlite3 *db, int eTextRep, const void *name) {
    // Do nothing
}

static int dummy_authorization_callback(void *arg, int action, const char *detail1,
                                        const char *detail2, const char *detail3,
                                        const char *detail4) {
    return SQLITE_OK;
}

static void dummy_trace_callback(void *arg, const char *sql) {
    // Do nothing
}

static void dummy_profile_callback(void *arg, const char *sql, sqlite3_uint64 time) {
    // Do nothing
}

static void dummy_update_callback(void *arg, int action, const char *db_name,
                                  const char *table_name, sqlite3_int64 rowid) {
    // Do nothing
}

static void dummy_preupdate_callback(void *arg, sqlite3 *db, int action,
                                     const char *db_name, const char *table_name,
                                     sqlite3_int64 old_rowid, sqlite3_int64 new_rowid) {
    // Do nothing
}

static int dummy_wal_callback(void *arg, sqlite3 *db, const char *db_name, int pages) {
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Create a database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some callbacks to explore more states
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    sqlite3_progress_handler(db, 100, dummy_progress_callback, NULL);
    sqlite3_collation_needed(db, dummy_collation_needed, NULL);
    sqlite3_collation_needed16(db, dummy_collation_needed16, NULL);
    sqlite3_set_authorizer(db, dummy_authorization_callback, NULL);
    sqlite3_trace(db, dummy_trace_callback, NULL);
    sqlite3_profile(db, dummy_profile_callback, NULL);
    sqlite3_update_hook(db, dummy_update_callback, NULL);
    sqlite3_preupdate_hook(db, dummy_preupdate_callback, NULL);
    sqlite3_wal_hook(db, dummy_wal_callback, NULL);
    
    // Use input data to determine which mutex operations to perform
    size_t data_index = 0;
    
    // Get database mutex
    sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
    
    // Allocate various mutexes based on input data
    sqlite3_mutex *mutexes[16] = {0};
    int mutex_count = 0;
    
    while (data_index < Size && mutex_count < 16) {
        uint8_t mutex_type = Data[data_index] % 15; // 0-14 for mutex types
        mutexes[mutex_count] = sqlite3_mutex_alloc(mutex_type);
        mutex_count++;
        data_index++;
        
        if (data_index >= Size) break;
    }
    
    // Perform mutex operations based on remaining input data
    while (data_index < Size) {
        uint8_t op = Data[data_index] % 8;
        uint8_t mutex_idx = (data_index + 1 < Size) ? (Data[data_index + 1] % mutex_count) : 0;
        
        switch (op) {
            case 0:
                if (db_mutex) {
                    sqlite3_mutex_enter(db_mutex);
                }
                break;
            case 1:
                if (db_mutex) {
                    sqlite3_mutex_leave(db_mutex);
                }
                break;
            case 2:
                if (db_mutex) {
                    sqlite3_mutex_notheld(db_mutex);
                }
                break;
            case 3:
                if (mutex_count > 0 && mutexes[mutex_idx]) {
                    sqlite3_mutex_enter(mutexes[mutex_idx]);
                }
                break;
            case 4:
                if (mutex_count > 0 && mutexes[mutex_idx]) {
                    sqlite3_mutex_leave(mutexes[mutex_idx]);
                }
                break;
            case 5:
                if (mutex_count > 0 && mutexes[mutex_idx]) {
                    sqlite3_mutex_notheld(mutexes[mutex_idx]);
                }
                break;
            case 6:
                if (mutex_count > 0 && mutexes[mutex_idx]) {
                    sqlite3_mutex_free(mutexes[mutex_idx]);
                    mutexes[mutex_idx] = NULL;
                }
                break;
            case 7:
                // Allocate a new mutex
                if (mutex_count < 16) {
                    uint8_t new_type = (data_index + 2 < Size) ? (Data[data_index + 2] % 15) : 0;
                    mutexes[mutex_count] = sqlite3_mutex_alloc(new_type);
                    mutex_count++;
                    data_index += 2;
                }
                break;
        }
        
        data_index++;
        if (data_index >= Size) break;
    }
    
    // Clean up allocated mutexes
    for (int i = 0; i < mutex_count; i++) {
        if (mutexes[i]) {
            sqlite3_mutex_free(mutexes[i]);
        }
    }
    
    // Close database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

#ifdef __cplusplus
}
#endif
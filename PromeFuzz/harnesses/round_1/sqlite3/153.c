// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void rollback_hook_callback(void *arg) {
    // Simple callback that does nothing
    (void)arg;
}

static unsigned int autovacuum_pages_callback(
    void *pClientData,
    const char *zSchema,
    unsigned int nDbPage,
    unsigned int nFreePage,
    unsigned int nBytePerPage) {
    (void)pClientData;
    (void)zSchema;
    (void)nBytePerPage;
    // Return a value based on input parameters
    if (nFreePage > 0 && nDbPage > 0) {
        return nFreePage / 2;
    }
    return 0;
}

static void autovacuum_destructor(void *arg) {
    // Simple destructor that does nothing
    (void)arg;
}

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Use the fuzz data to influence behavior
    // We'll use the first few bytes to decide which operations to perform
    
    if (Size > 0) {
        // 1. Test sqlite3_rollback_hook
        void *hook_arg = (void*)(uintptr_t)Data[0];
        void *prev_arg = sqlite3_rollback_hook(db, rollback_hook_callback, hook_arg);
        (void)prev_arg;
        
        // Call it again with NULL callback to clear it
        if (Size > 1 && (Data[1] & 1)) {
            sqlite3_rollback_hook(db, NULL, NULL);
        }
    }
    
    if (Size > 2) {
        // 2. Test sqlite3_autovacuum_pages
        void *autovac_arg = (void*)(uintptr_t)Data[2];
        rc = sqlite3_autovacuum_pages(db, autovacuum_pages_callback, 
                                      autovac_arg, autovacuum_destructor);
        (void)rc;
        
        // Clear the callback
        if (Size > 3 && (Data[3] & 1)) {
            sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
        }
    }
    
    if (Size > 4) {
        // 3. Test sqlite3_txn_state
        // Create a simple schema name from the data
        char schema_name[32] = {0};
        size_t schema_len = Size > 36 ? 32 : (Size - 4);
        if (schema_len > 0) {
            memcpy(schema_name, Data + 4, schema_len);
            // Ensure null termination
            schema_name[schema_len < 32 ? schema_len : 31] = '\0';
        }
        
        int txn_state = sqlite3_txn_state(db, schema_name);
        (void)txn_state;
        
        // Also test with NULL schema
        if (Size > 5 && (Data[5] & 1)) {
            txn_state = sqlite3_txn_state(db, NULL);
            (void)txn_state;
        }
    }
    
    if (Size > 6) {
        // 4. Test sqlite3_db_status with various operations
        int current = 0, highwater = 0;
        
        // Try different op codes based on input data
        int op_codes[] = {
            SQLITE_DBSTATUS_LOOKASIDE_USED,
            SQLITE_DBSTATUS_LOOKASIDE_HIT,
            SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
            SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL,
            SQLITE_DBSTATUS_CACHE_USED,
            SQLITE_DBSTATUS_SCHEMA_USED,
            SQLITE_DBSTATUS_STMT_USED,
            SQLITE_DBSTATUS_CACHE_HIT,
            SQLITE_DBSTATUS_CACHE_MISS,
            SQLITE_DBSTATUS_CACHE_WRITE
        };
        
        size_t num_ops = sizeof(op_codes) / sizeof(op_codes[0]);
        for (size_t i = 0; i < num_ops && (6 + i) < Size; i++) {
            int reset_flag = (Data[6 + i] & 1) ? 1 : 0;
            rc = sqlite3_db_status(db, op_codes[i], &current, &highwater, reset_flag);
            (void)rc;
        }
    }
    
    if (Size > 16) {
        // 5. Test sqlite3_db_config with various operations
        // Try different configuration options based on input data
        
        // SQLITE_DBCONFIG_LOOKASIDE
        if (Data[16] & 1) {
            int buf_size = 128;
            int buf_count = 8;
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, NULL, buf_size, buf_count);
            (void)rc;
        }
        
        // SQLITE_DBCONFIG_ENABLE_FKEY
        if (Size > 17 && (Data[17] & 1)) {
            int enable = (Data[17] & 2) ? 1 : 0;
            int old_value = 0;
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, enable, &old_value);
            (void)rc;
        }
        
        // SQLITE_DBCONFIG_ENABLE_TRIGGER
        if (Size > 18 && (Data[18] & 1)) {
            int enable = (Data[18] & 2) ? 1 : 0;
            int old_value = 0;
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, enable, &old_value);
            (void)rc;
        }
    }
    
    // 6. Test sqlite3_close
    // We'll close the database at the end
    rc = sqlite3_close(db);
    (void)rc;
    
    return 0;
}
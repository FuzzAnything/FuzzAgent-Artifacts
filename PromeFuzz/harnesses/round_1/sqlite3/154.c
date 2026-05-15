// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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
#include "/promefuzz/database/sqlite3/latest/code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_rollback_callback(void *arg) {
    // Do nothing
}

static unsigned int dummy_autovacuum_callback(void *arg, const char *zSchema,
                                             unsigned int nDbPage,
                                             unsigned int nFreePage,
                                             unsigned int nBytePerPage) {
    // Return a value based on fuzzer input
    if (arg != NULL) {
        return *(unsigned int*)arg % (nFreePage + 1);
    }
    return nFreePage; // Default: vacuum all free pages
}

static void dummy_autovacuum_destructor(void *arg) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer data to influence decisions
    size_t data_index = 0;
    
    // 1. Test sqlite3_rollback_hook
    if (Size > 0) {
        void *arg = (void*)(Data + (data_index % Size));
        void *prev_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, arg);
        // Could also test with NULL callback
        if (Size > 1 && (Data[data_index % Size] & 0x01)) {
            sqlite3_rollback_hook(db, NULL, NULL);
        }
        data_index++;
    }
    
    // 2. Test sqlite3_autovacuum_pages
    if (Size > data_index) {
        unsigned int callback_arg = 0;
        if (Size - data_index >= sizeof(unsigned int)) {
            memcpy(&callback_arg, Data + data_index, sizeof(unsigned int));
            data_index += sizeof(unsigned int);
        } else {
            callback_arg = Data[data_index++];
        }
        
        rc = sqlite3_autovacuum_pages(db, dummy_autovacuum_callback, 
                                      &callback_arg, dummy_autovacuum_destructor);
        // Test with NULL callback to cancel
        if (Size > data_index && (Data[data_index % Size] & 0x02)) {
            sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
        }
        data_index++;
    }
    
    // 3. Test sqlite3_txn_state
    if (Size > data_index) {
        // Create a schema name from fuzzer data
        char schema_name[256] = {0};
        size_t schema_len = Size - data_index;
        if (schema_len > 255) schema_len = 255;
        
        if (schema_len > 0) {
            memcpy(schema_name, Data + data_index, schema_len);
            schema_name[schema_len] = '\0';
            data_index += schema_len;
        } else {
            strcpy(schema_name, "main");
        }
        
        int txn_state = sqlite3_txn_state(db, schema_name);
        
        // Also test with NULL schema
        if (Size > data_index && (Data[data_index % Size] & 0x04)) {
            txn_state = sqlite3_txn_state(db, NULL);
        }
        data_index++;
    }
    
    // 4. Test sqlite3_db_status with various operations
    if (Size > data_index) {
        int op_codes[] = {
            SQLITE_DBSTATUS_LOOKASIDE_USED,
            SQLITE_DBSTATUS_CACHE_USED,
            SQLITE_DBSTATUS_SCHEMA_USED,
            SQLITE_DBSTATUS_STMT_USED,
            SQLITE_DBSTATUS_LOOKASIDE_HIT,
            SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
            SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL,
            SQLITE_DBSTATUS_CACHE_HIT,
            SQLITE_DBSTATUS_CACHE_MISS,
            SQLITE_DBSTATUS_CACHE_WRITE,
            SQLITE_DBSTATUS_DEFERRED_FKS,
            SQLITE_DBSTATUS_CACHE_USED_SHARED
        };
        
        int num_ops = sizeof(op_codes) / sizeof(op_codes[0]);
        int current_value = 0;
        int highwater_value = 0;
        int reset_flag = 0;
        
        for (int i = 0; i < 3 && data_index < Size; i++) {
            int op_index = Data[data_index++] % num_ops;
            if (data_index < Size) {
                reset_flag = Data[data_index++] & 0x01;
            }
            
            rc = sqlite3_db_status(db, op_codes[op_index], 
                                   &current_value, &highwater_value, reset_flag);
        }
    }
    
    // 5. Test sqlite3_db_config with various operations
    if (Size > data_index) {
        // Test SQLITE_DBCONFIG_LOOKASIDE
        void *pBuf = NULL;
        int sz = 128;
        int cnt = 100;
        
        if (Size - data_index >= 8) {
            sz = *(int*)(Data + data_index) % 4096;
            data_index += 4;
            cnt = *(int*)(Data + data_index) % 1000;
            data_index += 4;
        }
        
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, pBuf, sz, cnt);
        
        // Test SQLITE_DBCONFIG_ENABLE_FKEY
        if (data_index < Size) {
            int enable_fkey = Data[data_index++] & 0x01;
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 
                                   enable_fkey, -1);
        }
        
        // Test SQLITE_DBCONFIG_ENABLE_TRIGGER
        if (data_index < Size) {
            int enable_trigger = Data[data_index++] & 0x01;
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, 
                                   enable_trigger, -1);
        }
    }
    
    // 6. Test sqlite3_close
    rc = sqlite3_close(db);
    
    // Also test with NULL (should be harmless)
    sqlite3_close(NULL);
    
    return 0;
}
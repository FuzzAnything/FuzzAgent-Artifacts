// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_rollback_callback(void *arg) {
    // Empty callback for rollback hook
}

static unsigned int dummy_autovacuum_callback(
    void *arg,
    const char *zSchema,
    unsigned int nDbPage,
    unsigned int nFreePage,
    unsigned int nBytePerPage) {
    // Simple callback that returns a value based on input
    if (arg != NULL) {
        return (unsigned int)(uintptr_t)arg % (nFreePage + 1);
    }
    return nFreePage;
}

static void dummy_destructor(void *arg) {
    // Empty destructor for autovacuum pages
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database for fuzzing
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Use the input data to determine which operations to perform
    if (Size > 0) {
        uint8_t op_selector = Data[0];
        size_t data_offset = 1;
        
        // 1. Test sqlite3_rollback_hook
        void *rollback_arg = (void*)(uintptr_t)op_selector;
        void *prev_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, rollback_arg);
        
        // Call again with NULL callback to test unregistering
        if (op_selector & 0x01) {
            sqlite3_rollback_hook(db, NULL, NULL);
        }
        
        // 2. Test sqlite3_autovacuum_pages
        void *autovac_arg = (void*)(uintptr_t)(op_selector * 256);
        rc = sqlite3_autovacuum_pages(db, dummy_autovacuum_callback, autovac_arg, dummy_destructor);
        
        // Test with NULL callback
        if (op_selector & 0x02) {
            sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
        }
        
        // 3. Test sqlite3_txn_state
        const char *schema_name = NULL;
        if (Size > data_offset + 1 && (op_selector & 0x04)) {
            // Use a small portion of input as schema name
            size_t name_len = Data[data_offset] % 32;
            if (name_len > 0 && data_offset + name_len < Size) {
                char *temp_name = (char*)malloc(name_len + 1);
                if (temp_name) {
                    memcpy(temp_name, Data + data_offset + 1, name_len);
                    temp_name[name_len] = '\0';
                    schema_name = temp_name;
                }
                data_offset += name_len + 1;
            }
        }
        
        int txn_state = sqlite3_txn_state(db, schema_name);
        
        // Test with invalid schema name
        if (op_selector & 0x08) {
            sqlite3_txn_state(db, "invalid_schema_name_that_probably_doesnt_exist");
        }
        
        if (schema_name != NULL) {
            free((void*)schema_name);
        }
        
        // 4. Test sqlite3_db_status with various operations
        if (Size > data_offset + 2) {
            int current = 0;
            int highwater = 0;
            
            // Test a few different status operations
            int status_op = Data[data_offset] % 10; // SQLITE_DBSTATUS options are 0-9
            int reset_flag = Data[data_offset + 1] & 0x01;
            
            rc = sqlite3_db_status(db, status_op, &current, &highwater, reset_flag);
            
            // Test another status operation
            if (Size > data_offset + 3) {
                status_op = Data[data_offset + 2] % 10;
                reset_flag = Data[data_offset + 3] & 0x01;
                rc = sqlite3_db_status(db, status_op, &current, &highwater, reset_flag);
            }
        }
        
        // 5. Test sqlite3_db_config with various operations
        // We need to be careful with db_config as it uses varargs
        // and different operations require different numbers of arguments
        
        // Only call db_config if db is still valid
        if (db != NULL) {
            // Test SQLITE_DBCONFIG_LOOKASIDE (op code 1001)
            if (op_selector & 0x10) {
                int lookaside_size = 128;
                int lookaside_count = 8;
                rc = sqlite3_db_config(db, 1001, lookaside_size, lookaside_count);
            }
            
            // Test SQLITE_DBCONFIG_ENABLE_FKEY (op code 1002)
            if (op_selector & 0x20) {
                int fkey_enable = 1;
                int old_val;
                rc = sqlite3_db_config(db, 1002, fkey_enable, &old_val);
            }
            
            // Test SQLITE_DBCONFIG_ENABLE_TRIGGER (op code 1003)
            if (op_selector & 0x40) {
                int trigger_enable = 1;
                int old_val;
                rc = sqlite3_db_config(db, 1003, trigger_enable, &old_val);
            }
        }
    }
    
    // 6. Test sqlite3_close
    // First, try to close the database
    if (db != NULL) {
        rc = sqlite3_close(db);
        
        // If close fails with SQLITE_BUSY, we need to handle it
        if (rc == SQLITE_BUSY) {
            // In a real scenario, we would finalize statements here
            // For fuzzing, we'll just try to close again
            rc = sqlite3_close(db);
        }
    }
    
    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
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

static void rollback_callback(void *arg) {
    /* Empty callback for rollback hook */
}

static int commit_callback(void *arg) {
    /* Return 0 to allow commit, non-zero would convert to rollback */
    return 0;
}

static void unlock_notify_callback(void **apArg, int nArg) {
    /* Empty callback for unlock notification */
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    void *prev_arg;
    int prev_callback_ret;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Use the fuzzer input to determine various parameters */
    size_t offset = 0;
    
    /* 1. Test sqlite3_rollback_hook */
    if (Size > offset) {
        uint8_t hook_type = Data[offset] % 3;
        offset = (offset + 1) % Size;
        
        if (hook_type == 0) {
            prev_arg = sqlite3_rollback_hook(db, rollback_callback, (void*)Data);
        } else if (hook_type == 1) {
            prev_arg = sqlite3_rollback_hook(db, NULL, NULL);
        } else {
            prev_arg = sqlite3_rollback_hook(db, rollback_callback, db);
        }
        
        /* Call again to test overriding previous hook */
        if (Size > offset) {
            uint8_t override_type = Data[offset] % 2;
            offset = (offset + 1) % Size;
            if (override_type == 0) {
                sqlite3_rollback_hook(db, NULL, NULL);
            } else {
                sqlite3_rollback_hook(db, rollback_callback, (void*)&Size);
            }
        }
    }
    
    /* 2. Test sqlite3_db_status with various op codes */
    if (Size > offset) {
        /* Use fuzzer data to select op codes */
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
            SQLITE_DBSTATUS_CACHE_WRITE,
            SQLITE_DBSTATUS_DEFERRED_FKS,
            SQLITE_DBSTATUS_CACHE_SPILL
        };
        
        int op_index = (Size > 0) ? Data[offset] % (sizeof(op_codes)/sizeof(op_codes[0])) : 0;
        offset = (offset + 1) % Size;
        
        int current_val = 0;
        int highwater_val = 0;
        int reset_flag = (Size > offset) ? (Data[offset] & 1) : 0;
        offset = (offset + 1) % Size;
        
        rc = sqlite3_db_status(db, op_codes[op_index], &current_val, &highwater_val, reset_flag);
        
        /* Test with different reset flags */
        if (Size > offset) {
            reset_flag = Data[offset] & 1;
            offset = (offset + 1) % Size;
            sqlite3_db_status(db, op_codes[op_index], &current_val, &highwater_val, reset_flag);
        }
    }
    
    /* 3. Test sqlite3_unlock_notify */
    /* Note: This requires SQLITE_ENABLE_UNLOCK_NOTIFY to be defined at compile time */
    if (Size > offset) {
        uint8_t notify_type = Data[offset] % 3;
        offset = (offset + 1) % Size;
        
        if (notify_type == 0) {
            rc = sqlite3_unlock_notify(db, unlock_notify_callback, (void*)Data);
        } else if (notify_type == 1) {
            rc = sqlite3_unlock_notify(db, NULL, NULL);
        } else {
            rc = sqlite3_unlock_notify(db, unlock_notify_callback, db);
        }
        
        /* Test overriding previous registration */
        if (Size > offset && (Data[offset] & 1)) {
            offset = (offset + 1) % Size;
            sqlite3_unlock_notify(db, NULL, NULL);
        }
    }
    
    /* 4. Test sqlite3_txn_state */
    if (Size > offset) {
        uint8_t schema_type = Data[offset] % 4;
        offset = (offset + 1) % Size;
        
        const char *schemas[] = {NULL, "main", "temp", "invalid_schema"};
        int txn_state = sqlite3_txn_state(db, schemas[schema_type]);
        
        /* Test with different schemas */
        if (Size > offset) {
            schema_type = Data[offset] % 4;
            offset = (offset + 1) % Size;
            sqlite3_txn_state(db, schemas[schema_type]);
        }
    }
    
    /* 5. Test sqlite3_commit_hook */
    if (Size > offset) {
        uint8_t commit_hook_type = Data[offset] % 3;
        offset = (offset + 1) % Size;
        
        if (commit_hook_type == 0) {
            prev_arg = sqlite3_commit_hook(db, commit_callback, (void*)Data);
        } else if (commit_hook_type == 1) {
            prev_arg = sqlite3_commit_hook(db, NULL, NULL);
        } else {
            prev_arg = sqlite3_commit_hook(db, commit_callback, db);
        }
        
        /* Test overriding */
        if (Size > offset && (Data[offset] & 1)) {
            offset = (offset + 1) % Size;
            sqlite3_commit_hook(db, NULL, NULL);
        }
    }
    
    /* 6. Test sqlite3_get_autocommit */
    int autocommit_state = sqlite3_get_autocommit(db);
    
    /* Test autocommit state changes by executing SQL */
    if (Size > offset) {
        uint8_t sql_type = Data[offset] % 3;
        offset = (offset + 1) % Size;
        
        char *sql = NULL;
        if (sql_type == 0) {
            sql = "BEGIN TRANSACTION";
        } else if (sql_type == 1) {
            sql = "CREATE TABLE test(id INTEGER)";
        } else {
            sql = "SELECT 1";
        }
        
        sqlite3_exec(db, sql, NULL, NULL, NULL);
        
        /* Check autocommit state again */
        autocommit_state = sqlite3_get_autocommit(db);
        
        /* If we started a transaction, roll it back */
        if (sql_type == 0) {
            sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
        }
    }
    
    /* Create a dummy file and write some data if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, (Size < 100) ? Size : 100, fp);
        }
        fclose(fp);
    }
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}
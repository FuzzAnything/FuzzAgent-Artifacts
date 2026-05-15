// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_preupdate_depth at sqlite3.c:80573:16 in sqlite3.h
// sqlite3_preupdate_count at sqlite3.c:80550:16 in sqlite3.h
// sqlite3_preupdate_blobwrite at sqlite3.c:80589:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_preupdate_depth at sqlite3.c:80573:16 in sqlite3.h
// sqlite3_preupdate_count at sqlite3.c:80550:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_blobwrite at sqlite3.c:80589:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void preupdate_callback(
    void *pCtx,
    sqlite3 *db,
    int op,
    char const *zDb,
    char const *zName,
    sqlite3_int64 iKey1,
    sqlite3_int64 iKey2
) {
    // Use fuzzer data to decide which functions to call
    uint8_t *data = (uint8_t *)pCtx;
    if (data == NULL) return;
    
    // Call preupdate_depth
    int depth = sqlite3_preupdate_depth(db);
    
    // Call preupdate_count
    int count = sqlite3_preupdate_count(db);
    
    // Call preupdate_old for valid column indices
    if (count > 0) {
        for (int i = 0; i < 3 && i < count; i++) {
            sqlite3_value *old_val = NULL;
            int rc = sqlite3_preupdate_old(db, i, &old_val);
            // rc is ignored as we're fuzzing
        }
    }
    
    // Call preupdate_blobwrite
    int blobwrite = sqlite3_preupdate_blobwrite(db);
    
    // Call preupdate_new for valid column indices
    if (count > 0 && (op == SQLITE_INSERT || op == SQLITE_UPDATE)) {
        for (int i = 0; i < 3 && i < count; i++) {
            sqlite3_value *new_val = NULL;
            int rc = sqlite3_preupdate_new(db, i, &new_val);
            // rc is ignored as we're fuzzing
        }
    }
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create in-memory database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Create a simple table
    char *errmsg = NULL;
    rc = sqlite3_exec(db, 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, name TEXT, value BLOB);",
        NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return 0;
    }
    
    // Set pre-update hook with fuzzer data as context
    sqlite3_preupdate_hook(db, preupdate_callback, (void *)Data);
    
    // Perform operations based on fuzzer input
    uint8_t op_type = Data[0] % 3;
    size_t data_offset = 1;
    
    if (op_type == 0) { // INSERT
        // Use fuzzer data for values
        const char *name = "test";
        if (Size - data_offset > 0) {
            // Create a string from fuzzer data
            char *dynamic_name = malloc(Size - data_offset + 1);
            if (dynamic_name) {
                memcpy(dynamic_name, Data + data_offset, Size - data_offset);
                dynamic_name[Size - data_offset] = '\0';
                name = dynamic_name;
            }
        }
        
        sqlite3_stmt *stmt = NULL;
        rc = sqlite3_prepare_v2(db, 
            "INSERT INTO test(name, value) VALUES(?, ?);", -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
            
            // Bind blob from remaining fuzzer data
            if (Size - data_offset > 0) {
                sqlite3_bind_blob(stmt, 2, Data + data_offset, 
                    Size - data_offset > 100 ? 100 : Size - data_offset, 
                    SQLITE_STATIC);
            } else {
                sqlite3_bind_null(stmt, 2);
            }
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        if (name != "test") free((void *)name);
        
    } else if (op_type == 1) { // UPDATE
        // First ensure we have a row to update
        sqlite3_exec(db, "INSERT OR IGNORE INTO test(id, name) VALUES(1, 'initial');",
                    NULL, NULL, NULL);
        
        sqlite3_stmt *stmt = NULL;
        rc = sqlite3_prepare_v2(db, 
            "UPDATE test SET name = ?, value = ? WHERE id = 1;", -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            const char *name = "updated";
            if (Size - data_offset > 0) {
                char *dynamic_name = malloc(Size - data_offset + 1);
                if (dynamic_name) {
                    memcpy(dynamic_name, Data + data_offset, Size - data_offset);
                    dynamic_name[Size - data_offset] = '\0';
                    name = dynamic_name;
                }
            }
            
            sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
            
            if (Size - data_offset > 0) {
                sqlite3_bind_blob(stmt, 2, Data + data_offset, 
                    Size - data_offset > 100 ? 100 : Size - data_offset, 
                    SQLITE_STATIC);
            } else {
                sqlite3_bind_null(stmt, 2);
            }
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            
            if (name != "updated") free((void *)name);
        }
        
    } else if (op_type == 2) { // DELETE
        // First ensure we have a row to delete
        sqlite3_exec(db, "INSERT OR IGNORE INTO test(id, name) VALUES(1, 'todelete');",
                    NULL, NULL, NULL);
        
        sqlite3_exec(db, "DELETE FROM test WHERE id = 1;", NULL, NULL, NULL);
    }
    
    // Also call the functions outside of hook context
    int depth = sqlite3_preupdate_depth(db);
    int count = sqlite3_preupdate_count(db);
    int blobwrite = sqlite3_preupdate_blobwrite(db);
    
    // Try to call old/new with invalid context (should return SQLITE_MISUSE_BKPT)
    sqlite3_value *val = NULL;
    sqlite3_preupdate_old(db, 0, &val);
    sqlite3_preupdate_new(db, 0, &val);
    
    // Remove hook
    sqlite3_preupdate_hook(db, NULL, NULL);
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
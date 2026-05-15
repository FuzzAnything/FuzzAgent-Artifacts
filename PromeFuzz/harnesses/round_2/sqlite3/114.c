// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_preupdate_depth at sqlite3.c:80573:16 in sqlite3.h
// sqlite3_preupdate_count at sqlite3.c:80550:16 in sqlite3.h
// sqlite3_preupdate_blobwrite at sqlite3.c:80589:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static void preupdate_callback(
    void *pCtx,
    sqlite3 *db,
    int op,
    char const *zDb,
    char const *zName,
    sqlite3_int64 iKey1,
    sqlite3_int64 iKey2
) {
    (void)pCtx;
    (void)zDb;
    (void)zName;
    (void)iKey1;
    (void)iKey2;
    
    // Call target functions within the pre-update hook context
    int depth = sqlite3_preupdate_depth(db);
    int count = sqlite3_preupdate_count(db);
    int blobwrite = sqlite3_preupdate_blobwrite(db);
    
    (void)depth;
    (void)count;
    (void)blobwrite;
    
    // Try to access old and new values if available
    sqlite3_value *pValue = NULL;
    if (count > 0) {
        // Try to access a column (use modulo to stay within bounds)
        int col = 0;
        if (op == SQLITE_UPDATE || op == SQLITE_DELETE) {
            sqlite3_preupdate_old(db, col % count, &pValue);
        }
        if (op == SQLITE_INSERT || op == SQLITE_UPDATE) {
            sqlite3_preupdate_new(db, col % count, &pValue);
        }
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Set pre-update hook
    sqlite3_preupdate_hook(db, preupdate_callback, NULL);
    
    // Use fuzzer input to decide which operations to perform
    uint8_t operation = Data[0] % 4;
    const uint8_t *data_ptr = Data + 1;
    size_t data_size = Size > 1 ? Size - 1 : 0;
    
    sqlite3_stmt *stmt = NULL;
    
    switch (operation) {
        case 0: { // INSERT
            const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
            rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                if (data_size > 0) {
                    sqlite3_bind_blob(stmt, 1, data_ptr, 
                                     data_size > 1000 ? 1000 : (int)data_size, 
                                     SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_null(stmt, 1);
                }
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            break;
        }
        case 1: { // UPDATE
            // First insert a row if none exists
            sqlite3_exec(db, "INSERT OR IGNORE INTO test (data) VALUES (x'00');", 
                        NULL, NULL, NULL);
            
            const char *update_sql = "UPDATE test SET data = ? WHERE id = 1;";
            rc = sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                if (data_size > 0) {
                    sqlite3_bind_blob(stmt, 1, data_ptr,
                                     data_size > 1000 ? 1000 : (int)data_size,
                                     SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_null(stmt, 1);
                }
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            break;
        }
        case 2: { // DELETE
            // First ensure a row exists
            sqlite3_exec(db, "INSERT OR IGNORE INTO test (data) VALUES (x'00');",
                        NULL, NULL, NULL);
            
            const char *delete_sql = "DELETE FROM test WHERE id = 1;";
            rc = sqlite3_exec(db, delete_sql, NULL, NULL, NULL);
            (void)rc;
            break;
        }
        case 3: { // Multiple operations
            // Insert
            const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
            rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                if (data_size > 0) {
                    sqlite3_bind_blob(stmt, 1, data_ptr,
                                     data_size > 500 ? 500 : (int)data_size,
                                     SQLITE_TRANSIENT);
                }
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            
            // Update
            const char *update_sql = "UPDATE test SET data = ? WHERE id = (SELECT MAX(id) FROM test);";
            rc = sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                if (data_size > 0) {
                    sqlite3_bind_blob(stmt, 1, data_ptr + (data_size > 500 ? 500 : 0),
                                     data_size > 1000 ? 500 : (int)(data_size > 500 ? data_size - 500 : 0),
                                     SQLITE_TRANSIENT);
                }
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            
            // Delete
            sqlite3_exec(db, "DELETE FROM test WHERE id = (SELECT MAX(id) FROM test);",
                        NULL, NULL, NULL);
            break;
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}
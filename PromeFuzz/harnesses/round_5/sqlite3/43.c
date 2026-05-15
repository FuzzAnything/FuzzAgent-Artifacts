// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_preupdate_depth at sqlite3.c:80573:16 in sqlite3.h
// sqlite3_preupdate_blobwrite at sqlite3.c:80589:16 in sqlite3.h
// sqlite3_preupdate_count at sqlite3.c:80550:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
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

static void dummy_preupdate_callback(
    void *pCtx,
    sqlite3 *db,
    int op,
    char const *zDb,
    char const *zName,
    sqlite3_int64 iKey1,
    sqlite3_int64 iKey2
) {
    // This callback is called during pre-update operations
    // We can exercise the pre-update API functions here
    if (pCtx == NULL) return;
    
    uint8_t *data = (uint8_t *)pCtx;
    size_t size = *((size_t *)(data + 1));
    
    // Call preupdate functions within the callback context
    int depth = sqlite3_preupdate_depth(db);
    
    int blobwrite = sqlite3_preupdate_blobwrite(db);
    
    int count = sqlite3_preupdate_count(db);
    
    // Try to access old and new values for some columns
    if (count > 0) {
        sqlite3_value *old_val = NULL;
        sqlite3_value *new_val = NULL;
        
        // Use fuzzer data to determine which column to access
        int col_idx = 0;
        if (size > 2) {
            col_idx = data[2] % count;
        }
        
        // These will return SQLITE_MISUSE_BKPT if called in wrong context
        // but that's OK for fuzzing
        sqlite3_preupdate_old(db, col_idx, &old_val);
        sqlite3_preupdate_new(db, col_idx, &new_val);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a dummy database in memory
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Create a simple table
    char *err_msg = NULL;
    rc = sqlite3_exec(db, 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, value BLOB);",
        NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Set pre-update hook with our callback
    // Pass a pointer to a structure containing the fuzzer data
    uint8_t *context_data = malloc(Size + sizeof(size_t));
    if (context_data) {
        context_data[0] = Data[0];  // Store first byte as operation hint
        *((size_t *)(context_data + 1)) = Size;
        if (Size > 1) {
            memcpy(context_data + 1 + sizeof(size_t), Data + 1, Size - 1);
        }
        
        sqlite3_preupdate_hook(db, dummy_preupdate_callback, context_data);
        
        // Perform operations based on fuzzer input to trigger the hook
        switch (Data[0] % 3) {
            case 0: {  // INSERT
                sqlite3_exec(db, "INSERT INTO test_table (name, value) VALUES ('test', x'010203');",
                           NULL, NULL, &err_msg);
                break;
            }
            case 1: {  // UPDATE
                sqlite3_exec(db, "UPDATE test_table SET name = 'updated' WHERE id = 1;",
                           NULL, NULL, &err_msg);
                break;
            }
            case 2: {  // DELETE
                sqlite3_exec(db, "DELETE FROM test_table WHERE id = 1;",
                           NULL, NULL, &err_msg);
                break;
            }
        }
        
        if (err_msg) sqlite3_free(err_msg);
        free(context_data);
    }
    
    // Clear the hook
    sqlite3_preupdate_hook(db, NULL, NULL);
    
    // Try to trigger the hook again with another operation
    sqlite3_exec(db, "INSERT INTO test_table (name) VALUES ('another');", NULL, NULL, &err_msg);
    if (err_msg) sqlite3_free(err_msg);
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
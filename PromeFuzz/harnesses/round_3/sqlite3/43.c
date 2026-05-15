// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_preupdate_depth at sqlite3.c:80573:16 in sqlite3.h
// sqlite3_preupdate_count at sqlite3.c:80550:16 in sqlite3.h
// sqlite3_preupdate_blobwrite at sqlite3.c:80589:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_preupdate_depth at sqlite3.c:80573:16 in sqlite3.h
// sqlite3_preupdate_count at sqlite3.c:80550:16 in sqlite3.h
// sqlite3_preupdate_blobwrite at sqlite3.c:80589:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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
    
    // Call all pre-update functions within the callback context
    int depth = sqlite3_preupdate_depth(db);
    int count = sqlite3_preupdate_count(db);
    int blobwrite = sqlite3_preupdate_blobwrite(db);
    
    // Use fuzzer data to determine which column to access
    const uint8_t *data = (const uint8_t *)pCtx;
    size_t size = *((size_t *)pCtx + 1);
    
    if (size > 0 && count > 0) {
        int col_index = data[0] % count;
        sqlite3_value *val = NULL;
        
        if (op == SQLITE_UPDATE || op == SQLITE_DELETE) {
            sqlite3_preupdate_old(db, col_index, &val);
        }
        if (op == SQLITE_INSERT || op == SQLITE_UPDATE) {
            sqlite3_preupdate_new(db, col_index, &val);
        }
    }
    
    // Use remaining fuzzer data to influence behavior
    if (size > 1) {
        // Additional fuzzing: vary operations based on input
        int extra_col = (size > 1) ? data[1] % (count > 0 ? count : 1) : 0;
        sqlite3_value *extra_val = NULL;
        if (extra_col >= 0 && extra_col < count) {
            if (op == SQLITE_UPDATE) {
                sqlite3_preupdate_old(db, extra_col, &extra_val);
                sqlite3_preupdate_new(db, extra_col, &extra_val);
            }
        }
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    static char *err_msg = NULL;
    
    // Initialize database once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Create a table with various column types
        const char *create_sql = 
            "CREATE TABLE test (id INTEGER PRIMARY KEY, "
            "name TEXT, value REAL, data BLOB);";
        if (sqlite3_exec(db, create_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return 0;
        }
        
        // Insert some initial data
        const char *insert_sql = 
            "INSERT INTO test (name, value, data) VALUES "
            "('test1', 1.5, X'010203'), "
            "('test2', 2.5, X'040506');";
        if (sqlite3_exec(db, insert_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
            sqlite3_free(err_msg);
        }
        
        initialized = 1;
    }
    
    if (!db) {
        return 0;
    }
    
    // Pack fuzzer data into a structure for the callback
    struct {
        const uint8_t *data;
        size_t size;
    } fuzz_data = {Data, Size};
    
    // Set pre-update hook with fuzzer data as context
    sqlite3_preupdate_hook(db, preupdate_callback, &fuzz_data);
    
    // Use fuzzer data to determine operation type
    int op_type = Size > 0 ? Data[0] % 3 : 0;
    
    // Execute different operations to trigger the hook
    switch (op_type) {
        case 0: { // UPDATE
            const char *update_sql = "UPDATE test SET value = value + 1 WHERE id = 1;";
            sqlite3_exec(db, update_sql, NULL, NULL, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
            err_msg = NULL;
            break;
        }
        case 1: { // INSERT
            const char *insert_sql = "INSERT INTO test (name, value, data) VALUES ('fuzz', 3.14, X'070809');";
            sqlite3_exec(db, insert_sql, NULL, NULL, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
            err_msg = NULL;
            break;
        }
        case 2: { // DELETE
            const char *delete_sql = "DELETE FROM test WHERE id = 2;";
            sqlite3_exec(db, delete_sql, NULL, NULL, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
            err_msg = NULL;
            break;
        }
    }
    
    // Also test the functions outside callback context (should return SQLITE_MISUSE or default values)
    sqlite3_value *dummy_val = NULL;
    sqlite3_preupdate_old(db, 0, &dummy_val);
    sqlite3_preupdate_new(db, 0, &dummy_val);
    sqlite3_preupdate_depth(db);
    sqlite3_preupdate_count(db);
    sqlite3_preupdate_blobwrite(db);
    
    // Clear the hook
    sqlite3_preupdate_hook(db, NULL, NULL);
    
    // Additional fuzzing: create a dummy file if needed
    if (Size > 10) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 100 ? 100 : Size, fp);
            fclose(fp);
            remove("./dummy_file");
        }
    }
    
    return 0;
}
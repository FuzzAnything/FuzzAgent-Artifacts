// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"

// Helper function to create a test database with a pre-update hook
static void setup_test_database(sqlite3 **db) {
    if (sqlite3_open(":memory:", db) != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    // Enable pre-update hooks
    sqlite3_exec(*db, "PRAGMA recursive_triggers=ON;", NULL, NULL, NULL);
    
    // Create test table
    sqlite3_exec(*db, 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value INTEGER, "
        "data BLOB"
        ");", 
        NULL, NULL, NULL);
    
    // Insert some initial data
    sqlite3_exec(*db, 
        "INSERT INTO test_table (name, value, data) VALUES "
        "('test1', 100, X'010203'), "
        "('test2', 200, X'040506'), "
        "('test3', 300, X'070809');", 
        NULL, NULL, NULL);
}

// Pre-update hook callback
static void preupdate_hook(
    void *arg,
    sqlite3 *db,
    int op,
    const char *zDb,
    const char *zName,
    sqlite3_int64 iKey1,
    sqlite3_int64 iKey2
) {
    // Test sqlite3_preupdate_old
    if (op == SQLITE_UPDATE || op == SQLITE_DELETE) {
        for (int i = 0; i < 4; i++) {  // Try 4 columns
            sqlite3_value *old_val = NULL;
            int rc = sqlite3_preupdate_old(db, i, &old_val);
            if (rc == SQLITE_OK && old_val) {
                // Test value APIs on old value
                int type = sqlite3_value_type(old_val);
                int int_val = sqlite3_value_int(old_val);
                int nochange = sqlite3_value_nochange(old_val);
                (void)type;
                (void)int_val;
                (void)nochange;
            }
        }
    }
    
    // Test sqlite3_preupdate_new
    if (op == SQLITE_INSERT || op == SQLITE_UPDATE) {
        for (int i = 0; i < 4; i++) {  // Try 4 columns
            sqlite3_value *new_val = NULL;
            int rc = sqlite3_preupdate_new(db, i, &new_val);
            if (rc == SQLITE_OK && new_val) {
                // Test value APIs on new value
                int type = sqlite3_value_type(new_val);
                int int_val = sqlite3_value_int(new_val);
                int nochange = sqlite3_value_nochange(new_val);
                (void)type;
                (void)int_val;
                (void)nochange;
            }
        }
    }
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize database
    sqlite3 *db = NULL;
    setup_test_database(&db);
    if (!db) return 0;
    
    // Set pre-update hook
    sqlite3_preupdate_hook(db, preupdate_hook, NULL);
    
    // Test 1: Test sqlite3_column_value on a simple query
    sqlite3_stmt *stmt = NULL;
    const char *select_sql = "SELECT id, name, value, data FROM test_table;";
    
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                sqlite3_value *col_val = sqlite3_column_value(stmt, i);
                if (col_val) {
                    // Test value APIs
                    int type = sqlite3_value_type(col_val);
                    int int_val = sqlite3_value_int(col_val);
                    int nochange = sqlite3_value_nochange(col_val);
                    (void)type;
                    (void)int_val;
                    (void)nochange;
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test 2: Perform operations to trigger pre-update hooks
    // Use fuzzer input to decide which operation to perform
    uint8_t operation = Data[0] % 3;
    
    switch (operation) {
        case 0: {  // INSERT
            sqlite3_stmt *insert_stmt = NULL;
            const char *insert_sql = "INSERT INTO test_table (name, value, data) VALUES (?, ?, ?);";
            
            if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL) == SQLITE_OK) {
                // Bind some values based on fuzzer input
                char name[32];
                snprintf(name, sizeof(name), "fuzz_%u", Data[0]);
                sqlite3_bind_text(insert_stmt, 1, name, -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(insert_stmt, 2, (int)Data[1 % Size]);
                
                if (Size > 2) {
                    sqlite3_bind_blob(insert_stmt, 3, Data + 2, 
                                     (Size - 2 > 100) ? 100 : (int)(Size - 2), 
                                     SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_null(insert_stmt, 3);
                }
                
                sqlite3_step(insert_stmt);
                sqlite3_finalize(insert_stmt);
            }
            break;
        }
        
        case 1: {  // UPDATE
            sqlite3_stmt *update_stmt = NULL;
            const char *update_sql = "UPDATE test_table SET value = ?, data = ? WHERE id = 1;";
            
            if (sqlite3_prepare_v2(db, update_sql, -1, &update_stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_int(update_stmt, 1, (int)Data[1 % Size]);
                
                if (Size > 2) {
                    sqlite3_bind_blob(update_stmt, 2, Data + 2, 
                                     (Size - 2 > 100) ? 100 : (int)(Size - 2), 
                                     SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_null(update_stmt, 2);
                }
                
                sqlite3_step(update_stmt);
                sqlite3_finalize(update_stmt);
            }
            break;
        }
        
        case 2: {  // DELETE
            sqlite3_stmt *delete_stmt = NULL;
            const char *delete_sql = "DELETE FROM test_table WHERE id = 2;";
            
            if (sqlite3_prepare_v2(db, delete_sql, -1, &delete_stmt, NULL) == SQLITE_OK) {
                sqlite3_step(delete_stmt);
                sqlite3_finalize(delete_stmt);
            }
            break;
        }
    }
    
    // Test 3: Create a more complex query with joins to test sqlite3_column_value
    sqlite3_stmt *complex_stmt = NULL;
    const char *complex_sql = 
        "SELECT t1.id, t1.name, t1.value, t2.name "
        "FROM test_table t1 "
        "LEFT JOIN test_table t2 ON t1.id = t2.id + 1;";
    
    if (sqlite3_prepare_v2(db, complex_sql, -1, &complex_stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(complex_stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(complex_stmt);
            for (int i = 0; i < col_count; i++) {
                sqlite3_value *col_val = sqlite3_column_value(complex_stmt, i);
                if (col_val) {
                    // Test value APIs
                    int type = sqlite3_value_type(col_val);
                    int int_val = sqlite3_value_int(col_val);
                    (void)type;
                    (void)int_val;
                }
            }
        }
        sqlite3_finalize(complex_stmt);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
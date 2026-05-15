// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:110746:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

static int authorizer_callback(void *pUserData, int code, const char *zArg1,
                               const char *zArg2, const char *zArg3,
                               const char *zArg4) {
    (void)pUserData;
    (void)code;
    (void)zArg1;
    (void)zArg2;
    (void)zArg3;
    (void)zArg4;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    const char *data_type = NULL;
    const char *coll_seq = NULL;
    int not_null = 0;
    int primary_key = 0;
    int autoinc = 0;
    void *allocated_mem = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy table for testing
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "value REAL);";
    
    if (sqlite3_exec(db, create_table_sql, callback, 0, &errmsg) != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some dummy data
    const char *insert_sql = 
        "INSERT INTO test_table (name, value) VALUES ('test', 123.45);";
    if (sqlite3_exec(db, insert_sql, callback, 0, &errmsg) != SQLITE_OK) {
        sqlite3_free(errmsg);
    }
    
    // 1. sqlite3_exec - use fuzzer input as SQL if it's valid
    if (Size > 0 && Size < 1024) {
        char *sql = (char*)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            // Try to execute the fuzzer-provided SQL
            if (sqlite3_exec(db, sql, callback, 0, &errmsg) != SQLITE_OK) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql);
        }
    }
    
    // 2. sqlite3_set_authorizer
    sqlite3_set_authorizer(db, authorizer_callback, NULL);
    
    // 3. sqlite3_table_column_metadata
    sqlite3_table_column_metadata(db, "main", "test_table", "name",
                                  &data_type, &coll_seq, &not_null,
                                  &primary_key, &autoinc);
    
    // 4. sqlite3_test_control - use a safe, known opcode
    // Using SQLITE_TESTCTRL_OPTIMIZATIONS (25) which is commonly available
    // and doesn't require additional arguments in some configurations
    sqlite3_test_control(25, db, 0);
    
    // 5. sqlite3_malloc - allocate memory based on input size
    if (Size > 0 && Size < 1024 * 1024) {  // Limit to 1MB
        allocated_mem = sqlite3_malloc((int)Size);
        if (allocated_mem) {
            // Use the allocated memory
            memcpy(allocated_mem, Data, Size > 1024 ? 1024 : Size);
            sqlite3_free(allocated_mem);
        }
    }
    
    // Cleanup
    sqlite3_close(db);
    return 0;
}
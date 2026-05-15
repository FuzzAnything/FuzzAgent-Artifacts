// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:110746:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
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

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

static int dummy_authorizer(void *pUserData, int action_code, const char *arg1,
                           const char *arg2, const char *arg3, const char *arg4) {
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    const char *data_type = NULL;
    const char *coll_seq = NULL;
    int not_null = 0;
    int primary_key = 0;
    int autoinc = 0;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy table for testing
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "name TEXT NOT NULL, "
                             "value REAL);";
    sqlite3_exec(db, create_sql, NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // 1. sqlite3_exec
    if (Size > 0) {
        // Use fuzzer input as SQL statement (with safety limit)
        size_t sql_size = Size > 1024 ? 1024 : Size;
        char *sql = (char*)malloc(sql_size + 1);
        if (sql) {
            memcpy(sql, Data, sql_size);
            sql[sql_size] = '\0';
            
            sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql);
        }
    }
    
    // 2. sqlite3_set_authorizer
    sqlite3_set_authorizer(db, dummy_authorizer, NULL);
    
    // 3. sqlite3_table_column_metadata
    // Test with various combinations of parameters
    sqlite3_table_column_metadata(db, "main", "test_table", "id",
                                  &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
    
    sqlite3_table_column_metadata(db, "main", "test_table", "name",
                                  &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
    
    sqlite3_table_column_metadata(db, "main", "test_table", "value",
                                  &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
    
    // Test with NULL column name (checks table existence)
    sqlite3_table_column_metadata(db, "main", "test_table", NULL,
                                  NULL, NULL, NULL, NULL, NULL);
    
    // Test with NULL database name (searches all attached databases)
    sqlite3_table_column_metadata(db, NULL, "test_table", "id",
                                  &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
    
    // 4. sqlite3_test_control
    // Use only safe, known opcodes to avoid crashes
    // SQLITE_TESTCTRL_OPTIMIZATIONS is a safe opcode that doesn't dereference user data
    sqlite3_test_control(1, 0);  // SQLITE_TESTCTRL_OPTIMIZATIONS with mask 0
    
    // 5. sqlite3_malloc
    if (Size > 0) {
        // Use fuzzer input to determine allocation size (limit to reasonable size)
        int alloc_size = Data[0] % 4096;
        if (alloc_size > 0) {
            void *mem = sqlite3_malloc(alloc_size);
            if (mem) {
                // Try to use the memory
                memset(mem, 0, alloc_size);
                sqlite3_free(mem);
            }
        }
        
        // Test edge cases
        sqlite3_malloc(0);
        sqlite3_malloc(1);
        sqlite3_malloc(1024);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
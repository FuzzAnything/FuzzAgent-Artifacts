// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:110746:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

static int authorizer_callback(void *pUserData, int action_code,
                               const char *arg1, const char *arg2,
                               const char *arg3, const char *arg4) {
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
    void *mem_block = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy table for testing
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);";
    if (sqlite3_exec(db, create_table_sql, NULL, NULL, &errmsg) != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return 0;
    }
    
    // 1. sqlite3_exec
    // Use fuzzer input as SQL statement (ensure null termination)
    if (Size > 0) {
        char *sql = (char *)malloc(Size + 1);
        if (!sql) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql, Data, Size);
        sql[Size] = '\0';
        
        int rc = sqlite3_exec(db, sql, callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        free(sql);
    }
    
    // 2. sqlite3_set_authorizer
    sqlite3_set_authorizer(db, authorizer_callback, NULL);
    
    // 3. sqlite3_table_column_metadata
    // Try to get metadata for our test table
    sqlite3_table_column_metadata(db, NULL, "test_table", "id",
                                  &data_type, &coll_seq,
                                  &not_null, &primary_key, &autoinc);
    
    // 4. sqlite3_test_control
    // Use a few different opcodes based on input data
    if (Size > 0) {
        int opcode = Data[0] % 10; // Use first byte to choose opcode
        switch (opcode) {
            case 0:
                sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0xFFFFFFFF);
                break;
            case 1:
                sqlite3_test_control(SQLITE_TESTCTRL_BITVEC_TEST, 32);
                break;
            case 2:
                sqlite3_test_control(SQLITE_TESTCTRL_PENDING_BYTE, 0x0010000);
                break;
            default:
                // Use default opcode
                sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0);
                break;
        }
    }
    
    // 5. sqlite3_malloc
    // Allocate memory based on input size
    size_t alloc_size = Size % 1024; // Limit to reasonable size
    mem_block = sqlite3_malloc((int)alloc_size);
    if (mem_block) {
        sqlite3_free(mem_block);
    }
    
    // Cleanup
    sqlite3_close(db);
    return 0;
}
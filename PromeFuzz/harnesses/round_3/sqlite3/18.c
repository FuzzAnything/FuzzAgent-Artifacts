// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:110746:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

static int authorizer_callback(void *pUserData, int code, const char *arg1,
                               const char *arg2, const char *arg3, const char *arg4) {
    (void)pUserData;
    (void)code;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // 1. sqlite3_exec
    // Use fuzz data as SQL statement (limit to avoid huge allocations)
    size_t sql_size = Size > 1024 ? 1024 : Size;
    char *sql = (char*)malloc(sql_size + 1);
    if (sql) {
        memcpy(sql, Data, sql_size);
        sql[sql_size] = '\0';
        sqlite3_exec(db, sql, callback, NULL, &errmsg);
        free(sql);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }
    
    // 2. sqlite3_set_authorizer
    sqlite3_set_authorizer(db, authorizer_callback, NULL);
    
    // 3. sqlite3_table_column_metadata
    // Create a table first
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT)", 
                 NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    const char *data_type = NULL;
    const char *coll_seq = NULL;
    int not_null = 0;
    int primary_key = 0;
    int autoinc = 0;
    
    sqlite3_table_column_metadata(db, "main", "test", "id",
                                  &data_type, &coll_seq,
                                  &not_null, &primary_key, &autoinc);
    
    // 4. sqlite3_test_control
    // Use a safe operation code (SQLITE_TESTCTRL_OPTIMIZATIONS)
    // and use fuzz data to determine which optimizations to disable
    if (Size > 0) {
        int op = SQLITE_TESTCTRL_OPTIMIZATIONS;
        int mask = (int)Data[0];
        sqlite3_test_control(op, db, mask);
    }
    
    // 5. sqlite3_malloc
    // Use fuzz data to determine allocation size (limit to avoid huge allocations)
    int alloc_size = (Size > 0) ? (Data[0] % 1024) : 16;
    void *mem = sqlite3_malloc(alloc_size);
    if (mem) {
        sqlite3_free(mem);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
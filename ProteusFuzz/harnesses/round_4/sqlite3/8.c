// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:124753:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:189627:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int authorizer_callback(void *pUserData, int actionCode, const char *p1, const char *p2, const char *p3, const char *p4) {
    (void)pUserData;
    (void)actionCode;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    return SQLITE_OK;
}

static int exec_callback(void *pUserData, int argc, char **argv, char **colNames) {
    (void)pUserData;
    (void)argc;
    (void)argv;
    (void)colNames;
    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int rc;

    if (Size < 1) {
        return 0;
    }

    // Open database in memory to avoid file system dependencies
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // 1. sqlite3_exec
    // Create a simple table to ensure metadata functions have something to query
    const char *init_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT NOT NULL);";
    rc = sqlite3_exec(db, init_sql, exec_callback, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return 0;
    }

    // Execute fuzzed SQL if it looks like valid text, otherwise skip
    if (Size > 0) {
        char *fuzzed_sql = (char *)sqlite3_malloc(Size + 1);
        if (fuzzed_sql) {
            memcpy(fuzzed_sql, Data, Size);
            fuzzed_sql[Size] = '\0';
            // Only execute if it's not empty or whitespace (basic check)
            if (Size > 0) {
                sqlite3_exec(db, fuzzed_sql, exec_callback, NULL, &errmsg);
            }
            sqlite3_free(fuzzed_sql);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
        }
    }

    // 2. sqlite3_set_authorizer
    rc = sqlite3_set_authorizer(db, authorizer_callback, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 3. sqlite3_table_column_metadata
    const char *dbName = NULL;
    const char *tableName = "test_table";
    const char *columnName = "id";
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;

    rc = sqlite3_table_column_metadata(db, dbName, tableName, columnName,
                                        &pzDataType, &pzCollSeq, &pNotNull, &pPrimaryKey, &pAutoinc);
    // Ignore return value for fuzzing purposes, just ensure we call it

    // 4. sqlite3_test_control
    // FIXED: Pass the valid database pointer 'db' as the first argument instead of 0.
    // SQLITE_TESTCTRL_OPTIMIZATIONS expects (sqlite3*, int).
    int dummy_int = 0;
    rc = sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0);
    
    // 5. sqlite3_malloc
    int alloc_size = (Size % 1024) + 1;
    void *ptr = sqlite3_malloc(alloc_size);
    if (ptr != NULL) {
        // Write some data to ensure the memory is valid
        memset(ptr, 0, alloc_size);
        sqlite3_free(ptr);
    }

    // Cleanup
    sqlite3_close(db);

    return 0;
}
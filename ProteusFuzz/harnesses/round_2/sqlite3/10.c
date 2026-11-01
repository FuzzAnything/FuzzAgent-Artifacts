// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:124753:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:189627:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:189627:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to create a simple authorizer callback.
 */
static int simple_authorizer(void *pUserData, int action, const char *p1, const char *p2, const char *p3, const char *p4) {
    (void)pUserData;
    (void)action;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    return SQLITE_OK;
}

/*
 * Static helper function for sqlite3_exec callback.
 */
static int exec_callback(void *pUserData, int argc, char **argv, char **colNames) {
    (void)pUserData;
    (void)argc;
    (void)argv;
    (void)colNames;
    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int rc;
    int i;
    
    /* 
     * Step 1: Prepare the environment.
     * Open an in-memory database.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_close(db);
        rc = sqlite3_open("./dummy_file", &db);
        if (rc != SQLITE_OK || db == NULL) {
            return 0;
        }
    }

    /* Create a simple table to ensure metadata functions have valid data. */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, name TEXT, value REAL);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /*
     * Step 2: Invoke the target functions in the specified order.
     */

    /* 1. sqlite3_exec */
    const char *select_sql = "SELECT * FROM fuzz_test LIMIT 1;";
    rc = sqlite3_exec(db, select_sql, exec_callback, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* 2. sqlite3_set_authorizer */
    rc = sqlite3_set_authorizer(db, simple_authorizer, NULL);
    /* Ignore return value for fuzzing robustness */

    /* 3. sqlite3_table_column_metadata */
    const char *zDbName = NULL;
    const char *zTableName = "fuzz_test";
    const char *zColumnName = "name";
    char const *pzDataType = NULL;
    char const *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;

    rc = sqlite3_table_column_metadata(db, zDbName, zTableName, zColumnName,
                                       &pzDataType, &pzCollSeq, &pNotNull, &pPrimaryKey, &pAutoinc);
    
    /* Try with NULL column name */
    rc = sqlite3_table_column_metadata(db, zDbName, zTableName, NULL,
                                       NULL, NULL, NULL, NULL, NULL);

    /* 4. sqlite3_test_control */
    /* 
     * The previous crash occurred because sqlite3_test_control was called with opcodes
     * that require additional arguments (like SQLITE_TESTCTRL_OPTIMIZATIONS).
     * We only call opcodes that take no extra arguments or are safe to call with minimal args.
     * SQLITE_TESTCTRL_ISINIT and SQLITE_TESTCTRL_BYTEORDER are safe.
     * We avoid opcodes that expect va_args.
     */
    
    /* SQLITE_TESTCTRL_ISINIT: Returns 1 if initialized, 0 otherwise. No args. */
    rc = sqlite3_test_control(SQLITE_TESTCTRL_ISINIT);
    
    /* SQLITE_TESTCTRL_BYTEORDER: Returns byte order (1 or -1). No args. */
    rc = sqlite3_test_control(SQLITE_TESTCTRL_BYTEORDER);

    /* 
     * We skip SQLITE_TESTCTRL_OPTIMIZATIONS, SQLITE_TESTCTRL_SCRATCHMALLOC, etc.
     * because they require specific argument structures that we are not providing,
     * leading to the segfault seen in the ASAN report.
     */

    /* 5. sqlite3_malloc */
    int alloc_size = 0;
    if (Size > 0) {
        alloc_size = Data[0];
        if (alloc_size > 1024) {
            alloc_size = 1024;
        }
    }
    void *ptr = sqlite3_malloc(alloc_size);
    
    if (ptr != NULL) {
        memset(ptr, 0xFF, alloc_size);
        sqlite3_free(ptr);
    }

    /*
     * Step 3: Cleanup.
     */
    sqlite3_close(db);
    if (errmsg != NULL) {
        sqlite3_free(errmsg);
    }

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_table_column_metadata at sqlite3.c:189627:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:189627:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:189627:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:124753:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Authorizer callback for sqlite3_set_authorizer.
 * Returns SQLITE_OK to allow all actions for fuzzing purposes.
 */
static int authorizer_callback(void *pUserData, int opCode, const char *p1, const char *p2, const char *p3, const char *p4) {
    (void)pUserData;
    (void)opCode;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    return SQLITE_OK;
}

/*
 * Callback for sqlite3_exec.
 * Returns 0 to continue execution.
 */
static int exec_callback(void *pUserData, int argc, char **argv, char **colNames) {
    (void)pUserData;
    (void)argc;
    (void)argv;
    (void)colNames;
    return 0;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *errmsg = NULL;
    char const *pzDataType = NULL;
    char const *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;
    void *pMalloc = NULL;

    /* 
     * Step 1: Prepare the environment.
     * Open an in-memory database to avoid file system dependencies.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * Step 2: Invoke target functions in the specified order.
     * Order: sqlite3_exec -> sqlite3_set_authorizer -> sqlite3_table_column_metadata -> sqlite3_test_control -> sqlite3_malloc
     */

    /* 1. sqlite3_exec */
    /* Create a simple table to ensure metadata calls have something to query */
    const char *sql_init = "CREATE TABLE fuzz_test (id INTEGER PRIMARY KEY, name TEXT);";
    rc = sqlite3_exec(db, sql_init, exec_callback, NULL, &errmsg);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return 0;
    }
    sqlite3_free(errmsg);
    errmsg = NULL;

    /* Execute a query based on fuzz input if it looks like valid SQL (simple heuristic) */
    if (Size > 0) {
        /* Ensure null termination for safety */
        char *sql_buf = (char *)malloc(Size + 1);
        if (sql_buf) {
            memcpy(sql_buf, Data, Size);
            sql_buf[Size] = '\0';
            
            /* Only attempt execution if it starts with a known SQL keyword to reduce noise */
            if (strncmp(sql_buf, "SELECT", 6) == 0 || 
                strncmp(sql_buf, "INSERT", 6) == 0 || 
                strncmp(sql_buf, "UPDATE", 6) == 0 || 
                strncmp(sql_buf, "DELETE", 6) == 0 ||
                strncmp(sql_buf, "CREATE", 6) == 0) {
                rc = sqlite3_exec(db, sql_buf, exec_callback, NULL, &errmsg);
            }
            free(sql_buf);
        }
    }
    if (errmsg) sqlite3_free(errmsg);
    errmsg = NULL;

    /* 2. sqlite3_set_authorizer */
    rc = sqlite3_set_authorizer(db, authorizer_callback, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 3. sqlite3_table_column_metadata */
    /* Try to get metadata for the table we created */
    rc = sqlite3_table_column_metadata(
        db,
        "main",
        "fuzz_test",
        "id",
        &pzDataType,
        &pzCollSeq,
        &pNotNull,
        &pPrimaryKey,
        &pAutoinc
    );
    /* Ignore return code as metadata might not exist if table creation failed or schema changed */

    /* Try with NULL column name to check table existence */
    rc = sqlite3_table_column_metadata(
        db,
        "main",
        "fuzz_test",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );

    /* Try with a non-existent table to trigger error path */
    rc = sqlite3_table_column_metadata(
        db,
        "main",
        "non_existent_table",
        "col",
        &pzDataType,
        &pzCollSeq,
        &pNotNull,
        &pPrimaryKey,
        &pAutoinc
    );

    /* 4. sqlite3_test_control */
    /* Use safe operation codes. SQLITE_TESTCTRL_FIRST is 0. */
    rc = sqlite3_test_control(SQLITE_TESTCTRL_FIRST, 0);
    rc = sqlite3_test_control(SQLITE_TESTCTRL_ISINIT, 0);
    /* Removed SQLITE_TESTCTRL_PENDINGBYTE as it is not defined in all builds */

    /* 5. sqlite3_malloc */
    /* Allocate memory based on input size, clamped to reasonable limits */
    int alloc_size = 0;
    if (Size > 0) {
        alloc_size = (Size % 1024) + 1;
    } else {
        alloc_size = 10;
    }
    
    /* Ensure we don't allocate too much or negative */
    if (alloc_size <= 0) alloc_size = 1;
    if (alloc_size > 10000) alloc_size = 10000;

    pMalloc = sqlite3_malloc(alloc_size);
    if (pMalloc) {
        /* Use the memory briefly to ensure it's valid */
        memset(pMalloc, 0, alloc_size);
        sqlite3_free(pMalloc);
    }
    
    /* Also test with 0 and negative size */
    pMalloc = sqlite3_malloc(0);
    sqlite3_free(pMalloc);
    pMalloc = sqlite3_malloc(-1);
    sqlite3_free(pMalloc);

    /*
     * Step 3: Cleanup.
     * Close the database connection.
     */
    sqlite3_close(db);

    return 0;
}
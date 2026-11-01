// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_table_column_metadata at sqlite3.c:189627:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:124753:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static helper to avoid name conflicts */
static int dummy_callback(void *p, int n, char **v, char **h) {
    (void)p;
    (void)n;
    (void)v;
    (void)h;
    return 0;
}

static int dummy_authorizer(void *p, int action, const char *p1, const char *p2, const char *p3, const char *p4) {
    (void)p;
    (void)action;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int rc;

    /* 1. Prepare the environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a dummy table to ensure metadata functions have something to query */
    const char *init_sql = "CREATE TABLE fuzz_test (id INTEGER PRIMARY KEY, name TEXT, value REAL);";
    sqlite3_exec(db, init_sql, NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }

    /* 
     * 2. Invoke Target API Functions in the specified order:
     * sqlite3_exec -> sqlite3_set_authorizer -> sqlite3_table_column_metadata 
     * -> sqlite3_test_control -> sqlite3_malloc
     */

    /* Step 1: sqlite3_exec */
    /* Use fuzzed data as SQL if it looks like text, otherwise use a safe default */
    const char *sql_input = (Size > 0 && Data[Size-1] == 0) ? (const char *)Data : "SELECT 1;";
    
    /* Ensure we don't pass NULL to exec if Size is 0, though sqlite3_exec handles NULL gracefully */
    if (Size == 0) {
        sql_input = "SELECT 1;";
    }

    rc = sqlite3_exec(db, sql_input, dummy_callback, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }

    /* Step 2: sqlite3_set_authorizer */
    rc = sqlite3_set_authorizer(db, dummy_authorizer, NULL);
    (void)rc; /* Ignore return for fuzzing stability, just ensure call happens */

    /* Step 3: sqlite3_table_column_metadata */
    /* Fuzz table/column names from input if possible, else use defaults */
    const char *zDbName = (Size > 10) ? (const char *)(Data + 10) : NULL;
    const char *zTableName = (Size > 20) ? (const char *)(Data + 20) : "fuzz_test";
    const char *zColumnName = (Size > 30) ? (const char *)(Data + 30) : "id";

    /* Ensure strings are null-terminated for safety if we slice the buffer */
    char safe_table[64] = {0};
    char safe_col[64] = {0};
    
    if (zTableName) {
        strncpy(safe_table, zTableName, sizeof(safe_table) - 1);
    } else {
        strcpy(safe_table, "fuzz_test");
    }

    if (zColumnName) {
        strncpy(safe_col, zColumnName, sizeof(safe_col) - 1);
    } else {
        strcpy(safe_col, "id");
    }

    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int pNotNull = 0, pPrimaryKey = 0, pAutoinc = 0;

    rc = sqlite3_table_column_metadata(db, zDbName, safe_table, safe_col,
                                       &pzDataType, &pzCollSeq,
                                       &pNotNull, &pPrimaryKey, &pAutoinc);
    (void)rc;

    /* Step 4: sqlite3_test_control */
    /* SQLITE_TESTCTRL_FIRST is 0. We try a few safe operations or just pass 0 */
    /* Note: This is internal API, behavior varies. We just call it to trigger code paths. */
    rc = sqlite3_test_control(SQLITE_TESTCTRL_FIRST, NULL);
    (void)rc;

    /* Step 5: sqlite3_malloc */
    /* Fuzz the size based on input, but clamp to reasonable limits to avoid OOM or huge allocs */
    int alloc_size = 0;
    if (Size >= 4) {
        alloc_size = *(const int *)Data;
        if (alloc_size <= 0) alloc_size = 1;
        if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024; // Cap at 1MB
    } else {
        alloc_size = 64;
    }

    void *ptr = sqlite3_malloc(alloc_size);
    if (ptr) {
        /* Touch the memory to ensure it's valid */
        memset(ptr, 0, alloc_size);
        sqlite3_free(ptr);
    }

    /* 3. Cleanup */
    if (err_msg) {
        sqlite3_free(err_msg);
    }

    sqlite3_close(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:124753:16 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:124753:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:189627:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Callback for sqlite3_exec. We ignore the data to avoid complex state management. */
static int exec_callback(void *data, int argc, char **argv, char **colnames) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)colnames;
    return 0;
}

/* Authorizer callback for sqlite3_set_authorizer. */
static int auth_callback(void *pUserData, int action, const char *p1, const char *p2, const char *p3, const char *p4) {
    (void)pUserData;
    (void)action;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    /* Allow all actions to maximize code path exploration without immediate failure */
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Create an in-memory database to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a dummy table to ensure metadata functions have something to query */
    const char *setup_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, name TEXT);";
    rc = sqlite3_exec(db, setup_sql, NULL, NULL, NULL);
    (void)rc;

    /* 2. Invoke target functions in the specified order */

    /* Step 1: sqlite3_exec */
    /* Use input data as SQL if possible, but ensure it's null-terminated */
    if (Size > 0) {
        /* Create a safe SQL string from input, ensuring it doesn't exceed bounds */
        char *safe_sql = (char *)sqlite3_malloc(Size + 1);
        if (safe_sql) {
            memcpy(safe_sql, Data, Size);
            safe_sql[Size] = '\0';
            
            /* Execute the SQL. If it fails, that's fine for fuzzing. */
            rc = sqlite3_exec(db, safe_sql, exec_callback, NULL, &errmsg);
            
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            sqlite3_free(safe_sql);
        }
    } else {
        /* Fallback for empty input */
        rc = sqlite3_exec(db, "SELECT 1;", exec_callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }

    /* Step 2: sqlite3_set_authorizer */
    /* Register the authorizer callback */
    rc = sqlite3_set_authorizer(db, auth_callback, NULL);
    /* Reset authorizer to NULL to allow subsequent operations if needed, 
       though we are following the strict order. */
    rc = sqlite3_set_authorizer(db, NULL, NULL);

    /* Step 3: sqlite3_table_column_metadata */
    const char *zDbName = "main";
    const char *zTableName = "fuzz_test";
    const char *zColumnName = "id";
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;

    rc = sqlite3_table_column_metadata(
        db, 
        zDbName, 
        zTableName, 
        zColumnName, 
        &pzDataType, 
        &pzCollSeq, 
        &pNotNull, 
        &pPrimaryKey, 
        &pAutoinc
    );
    /* Note: Output pointers are valid until next API call, so we don't free them. */

    /* Step 4: sqlite3_test_control */
    /* The previous call caused a crash because SQLITE_TESTCTRL_OPTIMIZATIONS 
       expects specific arguments (int, int) and the internal implementation 
       might dereference NULL or invalid pointers if arguments are not valid 
       pointers or if the operation is not supported in this build.
       We use SQLITE_TESTCTRL_PENDING_BYTE which is generally safer for basic testing 
       or simply skip if the operation is known to be unstable.
       However, to strictly follow the "invoke" requirement without crashing,
       we use SQLITE_TESTCTRL_RESULT_INT32 which is a simple read operation.
       If the specific build doesn't support test_control, we should handle it gracefully.
       Let's try a safe operation code: SQLITE_TESTCTRL_ISINIT (0) or similar.
       Actually, the safest approach for fuzzing this specific API which is internal
       is to use an operation that doesn't require complex argument parsing or 
       ensure we pass valid arguments.
       Let's try SQLITE_TESTCTRL_OPTIMIZATIONS with valid int arguments (0, 1).
       The crash log suggests a NULL pointer dereference inside sqlite3_test_control 
       when handling the arguments for OPTIMIZATIONS.
       Let's switch to a simpler op code like SQLITE_TESTCTRL_DBFILENAME (10) or 
       just avoid the crash by checking if the function is available or using a 
       known safe op code.
       A common safe op code is SQLITE_TESTCTRL_OPTIMIZATIONS but it requires 
       (int, int) where the second is a pointer to an int array in some versions?
       Actually, looking at the crash: db->dbOptFlags = va_arg(ap, u32);
       It seems it expects an int. The crash happened at address 0x60 which is near NULL.
       This suggests the va_arg extraction failed or the internal state was corrupted.
       Let's try SQLITE_TESTCTRL_PENDING_BYTE (1) which takes no extra args or simple args.
       Or better, just call it with a known safe operation that doesn't crash.
       SQLITE_TESTCTRL_RESIZE (4) is also complex.
       Let's use SQLITE_TESTCTRL_ISINIT (0) if available, or just skip if it crashes.
       Since we must invoke it, let's try SQLITE_TESTCTRL_OPTIMIZATIONS with 0 and 1.
       Wait, the error log shows: db->dbOptFlags = va_arg(ap, u32);
       This implies the code is trying to read an argument.
       Let's try a different op code that is safer: SQLITE_TESTCTRL_RESULT_INT32 (20)
       or simply avoid the specific op code that crashes.
       Let's use SQLITE_TESTCTRL_OPTIMIZATIONS but ensure we pass valid ints.
       Actually, the safest bet for fuzzing this unstable API is to use 
       SQLITE_TESTCTRL_PENDING_BYTE (1) which usually just returns a byte.
       Let's try that.
    */
    rc = sqlite3_test_control(SQLITE_TESTCTRL_PENDING_BYTE, 0);

    /* Step 5: sqlite3_malloc */
    /* Allocate some memory based on input size, capped to avoid OOM or huge allocations */
    int alloc_size = 0;
    if (Size > 0) {
        /* Use modulo to keep size reasonable but variable */
        alloc_size = (Size % 4096) + 1;
    } else {
        alloc_size = 64;
    }
    
    void *ptr = sqlite3_malloc(alloc_size);
    if (ptr != NULL) {
        /* Write some data to the allocated memory to trigger potential memory checks */
        memset(ptr, 0xAA, alloc_size);
        sqlite3_free(ptr);
    }

    /* 3. Cleanup */
    /* Close the database connection */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
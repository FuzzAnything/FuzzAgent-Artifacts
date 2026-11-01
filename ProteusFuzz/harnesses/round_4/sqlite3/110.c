// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a dummy file for testing file-based operations.
 */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        const char *content = "SQLite Fuzz Test Data";
        fwrite(content, 1, strlen(content), f);
        fclose(f);
    }
}

/*
 * Static callback for sqlite3_collation_needed.
 * This function is invoked when an undefined collation sequence is encountered.
 */
static void collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    (void)pArg;
    (void)eTextRep;
    (void)zName;
    (void)db;
    /* 
     * We do not register a real collation here to avoid potential crashes 
     * if the fuzzer provides invalid names or if the callback is invoked 
     * in an unexpected state. The invocation itself is the test.
     */
}

/*
 * Fuzzing entry point.
 */
int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int cur, hiwtr;
    
    /* 
     * Ensure we have enough data to safely access indices used later.
     * We need at least 5 bytes for the logic below (indices 0 to 4).
     */
    if (Size < 5) {
        return 0;
    }

    /* 
     * Step 1: Prepare the environment.
     * Create a dummy file to allow file-based database opening if needed.
     */
    create_dummy_file();

    /* 
     * Determine the database filename from the input data.
     * We use a subset of the input to form a filename to explore different states.
     * We ensure the filename is null-terminated and safe.
     */
    char filename[256];
    size_t len = (Size < sizeof(filename) - 1) ? Size : sizeof(filename) - 1;
    memcpy(filename, Data, len);
    filename[len] = '\0';

    /* 
     * Strategy: Try to open a database.
     * We try multiple modes to explore different code paths.
     */
    
    /* Attempt 1: Standard open (Read-Write, Create) */
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK && rc != SQLITE_CANTOPEN && rc != SQLITE_PERM) {
        /* If open fails for reasons other than file permissions or existence, 
           we might still have a valid handle (as per docs) or NULL.
           If db is NULL, we can't proceed with other tests.
        */
        if (db == NULL) {
             /* Try opening an in-memory database if file open failed completely */
             rc = sqlite3_open(":memory:", &db);
             if (rc != SQLITE_OK || db == NULL) {
                 return 0;
             }
        }
    }

    if (db == NULL) {
        return 0;
    }

    /* 
     * Step 2: Invoke target functions with diverse inputs.
     */

    /* 1. sqlite3_collation_needed */
    /* Register a callback. We pass a pointer to the input data as user data. */
    rc = sqlite3_collation_needed(db, (void*)Data, collation_needed_callback);
    (void)rc; /* Ignore return for fuzzing, just ensure no crash */

    /* 2. sqlite3_db_config */
    /* Try various operations. We use input data to select the operation and value. */
    int op = Data[0] % 10; /* Limit to a small range to avoid undefined behavior with unknown ops */
    int value = (int)(Data[1] & 0xFF);
    int prev_value = 0;
    
    /* 
     * Note: sqlite3_db_config requires 4 arguments for some ops, but the signature is varargs.
     * We pass a pointer to prev_value to satisfy the requirement for ops that need it.
     */
    rc = sqlite3_db_config(db, op, value, &prev_value);
    (void)rc;

    /* 3. sqlite3_limit */
    /* Set or query limits. Use input to determine limit ID and new value. */
    int limit_id = Data[2] % SQLITE_LIMIT_LIKE_PATTERN_LENGTH; /* Ensure valid range */
    int new_limit = (int)(Data[3] & 0xFF) * 100; /* Scale up a bit */
    
    /* Query current limit first */
    int old_limit = sqlite3_limit(db, limit_id, -1);
    (void)old_limit;

    /* Set new limit */
    rc = sqlite3_limit(db, limit_id, new_limit);
    (void)rc;

    /* 4. sqlite3_db_status */
    /* Retrieve runtime stats. */
    int status_op = Data[4] % 10; /* Limit to a safe range of status ops */
    rc = sqlite3_db_status(db, status_op, &cur, &hiwtr, 0); /* Don't reset high water mark */
    (void)rc;

    /* Try resetting high water mark */
    rc = sqlite3_db_status(db, status_op, &cur, &hiwtr, 1);
    (void)rc;

    /* 5. sqlite3_db_release_memory */
    /* Attempt to free memory. */
    rc = sqlite3_db_release_memory(db);
    (void)rc;

    /* 
     * Step 3: Cleanup.
     * Always close the database connection.
     */
    if (db != NULL) {
        sqlite3_close(db);
    }

    return 0;
}
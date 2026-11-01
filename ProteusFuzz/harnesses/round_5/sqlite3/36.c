// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for sqlite3 error handling APIs.
 * Target sequence: sqlite3_errcode -> sqlite3_mprintf -> sqlite3_errmsg -> sqlite3_extended_errcode
 */

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *zDbName = ":memory:";
    int rc;
    int errCode;
    int extErrCode;
    const char *errMsg;
    char *mprintfResult = NULL;

    /* 1. Prepare the environment */
    /* Initialize SQLite if not already done (usually done by the fuzzer harness, but safe to call) */
    sqlite3_initialize();

    /* Open a database connection */
    rc = sqlite3_open(zDbName, &db);
    if (rc != SQLITE_OK && rc != SQLITE_ERROR) {
        /* If opening fails for reasons other than SQLITE_ERROR (which is normal for :memory: if no error),
           we might not have a valid db to test error codes on.
           However, sqlite3_open usually returns SQLITE_OK or SQLITE_ERROR.
           If it returns something else, we stop. */
        goto cleanup;
    }

    /* Try to trigger an error to ensure error codes/messages are set.
       We use the fuzz input to construct a potentially invalid SQL statement. */
    if (Size > 0) {
        /* Ensure null termination for safety when using as string */
        char *sql = (char *)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';

            /* Attempt to prepare a statement. This might fail, setting error codes. */
            sqlite3_stmt *stmt;
            const char *tail;
            rc = sqlite3_prepare_v2(db, sql, Size, &stmt, &tail);
            if (rc != SQLITE_OK) {
                /* Error occurred, error codes should be set */
            }
            if (stmt) {
                sqlite3_finalize(stmt);
            }
            free(sql);
        }
    } else {
        /* If no data, try a known invalid query to trigger an error state */
        sqlite3_exec(db, "SELECT * FROM nonexistent_table_xyz;", NULL, NULL, NULL);
    }

    /* 2. Invoke Target API Functions in Order */

    /* Step 1: sqlite3_errcode */
    /* Retrieves the numeric error code from the last failed operation */
    errCode = sqlite3_errcode(db);

    /* Step 2: sqlite3_mprintf */
    /* Format a string. We use the error code and some fuzz data if available.
       This function allocates memory that must be freed later. */
    if (Size > 0 && Size < 100) {
        /* Use a subset of input as a format string argument, ensuring safety */
        char safeBuf[100] = {0};
        memcpy(safeBuf, Data, Size < 99 ? Size : 99);
        /* Use a safe format string to avoid format string vulnerabilities in the fuzzer input itself */
        mprintfResult = sqlite3_mprintf("Error: %d, Data: %s", errCode, safeBuf);
    } else {
        mprintfResult = sqlite3_mprintf("Error: %d", errCode);
    }

    /* Step 3: sqlite3_errmsg */
    /* Retrieves the human-readable error message */
    errMsg = sqlite3_errmsg(db);

    /* Step 4: sqlite3_extended_errcode */
    /* Retrieves the extended error code */
    extErrCode = sqlite3_extended_errcode(db);

    /* 3. Explore more program states (Optional logic to ensure coverage) */
    /* We already triggered an error via SQL execution.
       The functions above are called regardless of whether an error occurred.
       If no error occurred, errCode will likely be SQLITE_OK (0) and errMsg might be NULL or empty. */

    /* Cleanup */
    if (mprintfResult) {
        sqlite3_free(mprintfResult);
    }

    if (db) {
        sqlite3_close(db);
    }

    sqlite3_shutdown();

    return 0;

cleanup:
    /* Fallback cleanup if open failed unexpectedly */
    if (db) {
        sqlite3_close(db);
    }
    sqlite3_shutdown();
    return 0;
}
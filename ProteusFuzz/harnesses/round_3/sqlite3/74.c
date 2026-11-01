// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
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

/* Static helper to avoid name conflicts */
static int dummy_progress_callback(void *pArg) {
    /* Do not modify the database connection here */
    if (pArg) {
        int val = *(int*)pArg;
        return val;
    }
    return 0;
}

static int dummy_commit_callback(void *pArg) {
    /* Do not modify the database connection here */
    if (pArg) {
        int val = *(int*)pArg;
        return val;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";

    /* Ensure we have at least some data to work with */
    if (Size == 0) {
        return 0;
    }

    /* Prepare flags based on input data to explore different states */
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0x07; /* Limit to reasonable flag combinations */
        /* Ensure SQLITE_OPEN_CREATE is set if we want to create the file */
        if ((flags & SQLITE_OPEN_READONLY) == 0) {
            flags |= SQLITE_OPEN_CREATE;
        }
    }

    /* Open the database */
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        /* If open fails, we can still test error reporting on NULL or invalid handle if needed,
           but sqlite3_errcode requires a valid handle. We'll skip further tests for this run. */
        return 0;
    }

    /* Test sqlite3_errcode immediately after open (should be SQLITE_OK or similar) */
    (void)sqlite3_errcode(db);

    /* Test sqlite3_extended_errcode */
    (void)sqlite3_extended_errcode(db);

    /* Test sqlite3_errmsg */
    const char *errmsg = sqlite3_errmsg(db);
    (void)errmsg; /* Suppress unused warning */

    /* Register a commit hook */
    int commit_arg_val = (Size > 1) ? Data[1] : 0;
    void *commit_arg = &commit_arg_val;
    void *prev_commit_arg = sqlite3_commit_hook(db, dummy_commit_callback, commit_arg);
    (void)prev_commit_arg;

    /* Register a progress handler */
    int progress_ops = (Size > 2) ? (Data[2] + 1) : 100; /* Ensure > 0 */
    int progress_arg_val = (Size > 3) ? Data[3] : 0;
    void *progress_arg = &progress_arg_val;
    sqlite3_progress_handler(db, progress_ops, dummy_progress_callback, progress_arg);

    /* Try to trigger an error to test error reporting functions */
    /* We can execute a simple invalid SQL to force an error state */
    const char *invalid_sql = "SELECT * FROM nonexistent_table_xyz;";
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, invalid_sql, -1, &stmt, NULL);
    
    /* If preparation failed, check error codes and messages */
    if (rc != SQLITE_OK) {
        int err_code = sqlite3_errcode(db);
        int ext_err_code = sqlite3_extended_errcode(db);
        const char *err_msg = sqlite3_errmsg(db);
        (void)err_code;
        (void)ext_err_code;
        (void)err_msg;
    }
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }

    /* Clean up: remove hooks */
    sqlite3_commit_hook(db, NULL, NULL);
    sqlite3_progress_handler(db, 0, NULL, NULL);

    /* Close the database */
    sqlite3_close(db);

    return 0;
}
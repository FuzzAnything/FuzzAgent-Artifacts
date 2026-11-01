// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static helper to create a dummy file if needed */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fclose(f);
    }
}

/* Progress callback that may return non-zero to abort */
static int progress_callback_abort(void *arg) {
    int *val = (int *)arg;
    return (val != NULL && *val != 0) ? 1 : 0;
}

/* Progress callback that always continues */
static int progress_callback_continue(void *arg) {
    (void)arg;
    return 0;
}

/* Commit hook that may return non-zero to rollback */
static int commit_hook_abort(void *arg) {
    int *val = (int *)arg;
    return (val != NULL && *val != 0) ? 1 : 0;
}

/* Commit hook that always allows commit */
static int commit_hook_continue(void *arg) {
    (void)arg;
    return 0;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int progress_ops = 0;
    int progress_abort_flag = 0;
    int commit_abort_flag = 0;
    void *commit_arg = NULL;

    /* Ensure we have some data to work with */
    if (Size == 0) {
        return 0;
    }

    /* Create dummy file for testing */
    create_dummy_file();

    /* Open database */
    rc = sqlite3_open_v2("./dummy_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        /* Even if open fails, we can test error functions with NULL or invalid handle */
        (void)sqlite3_errcode(NULL);
        (void)sqlite3_extended_errcode(NULL);
        (void)sqlite3_errmsg(NULL);
        return 0;
    }

    /* Fuzz sqlite3_progress_handler */
    /* Extract parameters from input data */
    if (Size >= 2) {
        progress_ops = (int)(Data[0] | (Data[1] << 8));
        progress_abort_flag = (Size > 2) ? (int)Data[2] : 0;
    }

    /* Test with various configurations */
    sqlite3_progress_handler(db, progress_ops, progress_callback_abort, &progress_abort_flag);
    sqlite3_progress_handler(db, progress_ops + 1, progress_callback_continue, NULL);
    sqlite3_progress_handler(db, -1, NULL, NULL); /* Disable */
    sqlite3_progress_handler(db, 0, NULL, NULL);  /* Disable */

    /* Fuzz sqlite3_commit_hook */
    if (Size >= 4) {
        commit_abort_flag = (int)Data[3];
    }

    /* Test commit hook */
    commit_arg = sqlite3_commit_hook(db, commit_hook_abort, &commit_abort_flag);
    sqlite3_commit_hook(db, commit_hook_continue, NULL);
    sqlite3_commit_hook(db, NULL, NULL); /* Disable */

    /* Fuzz error code retrieval */
    /* Trigger an error by executing invalid SQL */
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM nonexistent_table_xyz;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        int err = sqlite3_errcode(db);
        int ext_err = sqlite3_extended_errcode(db);
        const char *msg = sqlite3_errmsg(db);
        (void)err;
        (void)ext_err;
        (void)msg;
    }
    sqlite3_finalize(stmt);

    /* Additional error state exploration */
    (void)sqlite3_errcode(db);
    (void)sqlite3_extended_errcode(db);
    (void)sqlite3_errmsg(db);

    /* Close database */
    sqlite3_close(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
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
 * Static helper function to create a dummy file for fuzzing.
 * This ensures the file exists and contains some data for sqlite3_open_v2.
 */
static int create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    
    /* Write some dummy data to make it a valid file */
    const char *dummy = "CREATE TABLE test (id INTEGER PRIMARY KEY, data TEXT);";
    fwrite(dummy, 1, strlen(dummy), f);
    fclose(f);
    return 0;
}

/*
 * Static callback for progress handler.
 * Returns 0 to allow continuation, or 1 to abort based on fuzz input.
 */
static int progress_callback(void *arg) {
    int *pArg = (int *)arg;
    if (pArg && *pArg) {
        return 1; /* Abort */
    }
    return 0; /* Continue */
}

/*
 * Static callback for commit hook.
 * Returns 0 to allow commit, or 1 to rollback based on fuzz input.
 */
static int commit_callback(void *arg) {
    int *pArg = (int *)arg;
    if (pArg && *pArg) {
        return 1; /* Rollback */
    }
    return 0; /* Commit */
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Ensure we have enough data to interpret flags and arguments */
    if (Size < 4) {
        return 0;
    }

    /* Create a dummy file to ensure sqlite3_open_v2 has something to work with */
    if (create_dummy_file() != 0) {
        return 0;
    }

    /* 
     * Interpret fuzz input:
     * Data[0]: Flags for sqlite3_open_v2 (masked to valid bits)
     * Data[1]: Progress handler N (ops count)
     * Data[2]: Progress callback abort flag
     * Data[3]: Commit callback abort flag
     */
    
    int flags = Data[0] & (SQLITE_OPEN_READWRITE | SQLITE_OPEN_READONLY | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXCLUSIVE);
    int progress_n = Data[1];
    int progress_abort = Data[2];
    int commit_abort = Data[3];

    /* 
     * Open database with fuzzed flags.
     * Use "./dummy_file" as the filename.
     * Pass NULL for VFS to use default.
     */
    rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);
    
    /* 
     * Even if open fails, db might be partially allocated. 
     * We must attempt to close it if it's not NULL.
     * However, if rc != SQLITE_OK and db is NULL, we just proceed to error checks on NULL.
     */
    
    if (db) {
        /* 
         * Set progress handler with fuzzed parameters.
         * If progress_n < 1, it disables the handler.
         */
        sqlite3_progress_handler(db, progress_n, progress_callback, &progress_abort);
        
        /* 
         * Set commit hook with fuzzed parameters.
         */
        sqlite3_commit_hook(db, commit_callback, &commit_abort);
        
        /* 
         * Trigger some operations to potentially set error states.
         * We try to prepare a statement that might fail or succeed.
         * Using a simple query to exercise the engine.
         */
        sqlite3_stmt *stmt = NULL;
        const char *sql = "SELECT 1;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        /* 
         * Try to trigger an error by preparing invalid SQL if data suggests it.
         * We use a portion of the fuzz data as SQL if it looks like text.
         */
        if (Size > 10) {
            /* Construct a potentially invalid SQL string from fuzz data */
            char invalid_sql[64];
            int len = Size > 64 ? 64 : Size;
            memcpy(invalid_sql, Data + 4, len);
            invalid_sql[len] = '\0';
            
            /* Try to prepare invalid SQL to trigger error codes */
            sqlite3_stmt *bad_stmt = NULL;
            rc = sqlite3_prepare_v2(db, invalid_sql, -1, &bad_stmt, NULL);
            if (rc != SQLITE_OK) {
                /* Error occurred, now test error retrieval functions */
            }
            if (bad_stmt) {
                sqlite3_finalize(bad_stmt);
            }
        }
        
        /* 
         * Test error retrieval functions.
         * These are safe to call even if no error occurred.
         */
        const char *errmsg = sqlite3_errmsg(db);
        (void)errmsg; /* Use the value to avoid unused warnings */
        
        int errcode = sqlite3_errcode(db);
        (void)errcode;
        
        int ext_errcode = sqlite3_extended_errcode(db);
        (void)ext_errcode;
        
        /* 
         * Close the database connection.
         * This is critical for cleanup.
         */
        sqlite3_close(db);
        db = NULL;
    } else {
        /* 
         * If db is NULL, test error functions with NULL pointer.
         * According to docs, sqlite3_errmsg(NULL) returns "out of memory".
         * sqlite3_errcode(NULL) returns SQLITE_MISUSE.
         */
        const char *errmsg_null = sqlite3_errmsg(NULL);
        (void)errmsg_null;
        
        int errcode_null = sqlite3_errcode(NULL);
        (void)errcode_null;
        
        int ext_errcode_null = sqlite3_extended_errcode(NULL);
        (void)ext_errcode_null;
    }
    
    /* 
     * Ensure sqlite3_shutdown is not called here because we didn't call sqlite3_initialize.
     * The default behavior of sqlite3_open_v2 initializes the library if needed.
     * We rely on the process exit to clean up global state.
     */

    return 0;
}
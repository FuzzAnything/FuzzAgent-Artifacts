// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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

static int invoke_target_sequence(sqlite3 *db, const uint8_t *Data, size_t Size) {
    int rc;
    const char *err_msg;
    int ext_err;
    char *fmt_str;
    int mprintf_rc = 0;

    /* 1. sqlite3_errcode */
    rc = sqlite3_errcode(db);

    /* 2. sqlite3_mprintf */
    /* Construct a format string from input data, ensuring it's null-terminated */
    if (Size > 0 && Size < 4096) {
        char *safe_input = (char *)malloc(Size + 1);
        if (safe_input) {
            memcpy(safe_input, Data, Size);
            safe_input[Size] = '\0';
            
            /* Attempt to use the data as a format string or argument */
            /* We use a simple format to avoid complex parsing, but pass data as argument */
            fmt_str = sqlite3_mprintf("%s", safe_input);
            if (fmt_str) {
                /* Do nothing with the string, just check it exists */
                mprintf_rc = 1;
                sqlite3_free(fmt_str);
            }
            free(safe_input);
        }
    } else {
        /* Fallback if data is too large or empty */
        fmt_str = sqlite3_mprintf("fuzz_test_%d", (int)(Size % 1000));
        if (fmt_str) {
            mprintf_rc = 1;
            sqlite3_free(fmt_str);
        }
    }

    /* 3. sqlite3_errmsg */
    err_msg = sqlite3_errmsg(db);
    (void)err_msg; /* Suppress unused warning */

    /* 4. sqlite3_extended_errcode */
    ext_err = sqlite3_extended_errcode(db);

    return (rc == SQLITE_OK && mprintf_rc == 1 && ext_err == SQLITE_OK) ? 0 : 1;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Prepare environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* If opening fails, we can't test the sequence properly */
        sqlite3_close(db);
        return 0;
    }

    /* Ensure we have a valid state. 
       The target functions rely on "last failed operation".
       To explore states, we can intentionally trigger an error first.
    */
    const char *sql = (Size > 0 && Size < 1000) ? (const char *)Data : "SELECT invalid_column FROM nonexistent_table";
    sqlite3_stmt *stmt = NULL;
    
    /* Try to execute a potentially failing statement to set an error code */
    if (Size > 0) {
        /* We need a null-terminated string for SQL. 
           If the input contains nulls, sqlite3_prepare_v2 handles it, 
           but let's ensure we don't pass garbage that crashes immediately.
           We'll just use a fixed invalid query to guarantee an error state for testing.
        */
        rc = sqlite3_prepare_v2(db, "SELECT * FROM non_existent_table_xyz", -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            /* Error set, good for testing errcode/errmsg */
        }
        if (stmt) {
            sqlite3_finalize(stmt);
        }
    }

    /* Invoke the target sequence */
    invoke_target_sequence(db, Data, Size);

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}
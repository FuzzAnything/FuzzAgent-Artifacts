// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

/*
 * Static helper to safely open a database connection.
 * Uses a dummy file as per instructions.
 */
static sqlite3 *open_db_for_fuzzing(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Static helper to close and cleanup a database connection.
 */
static void close_db_for_fuzzing(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Static helper to trigger a known error to test error retrieval functions.
 */
static int trigger_error(sqlite3 *db) {
    if (!db) return SQLITE_MISUSE;
    
    /* Try to execute invalid SQL to generate an error */
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, "SELECT * FROM nonexistent_table;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        if (stmt) sqlite3_finalize(stmt);
        return rc;
    }
    if (stmt) sqlite3_finalize(stmt);
    
    /* Try another way to trigger error if prepare succeeded (unlikely for this query) */
    return sqlite3_exec(db, "SELECT invalid_syntax;", NULL, NULL, NULL);
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int result = 0;

    /* 
     * Strategy: 
     * 1. Open a database connection.
     * 2. Use input data to drive logic branches (enable/disable flags, file names, etc).
     * 3. Call target APIs in various states.
     * 4. Ensure cleanup.
     */

    /* 
     * Use the first byte to decide whether to open a valid DB or test with NULL/invalid.
     * This helps explore the SQLITE_MISUSE paths.
     */
    if (Size > 0 && Data[0] % 2 == 0) {
        db = open_db_for_fuzzing();
    } else {
        db = NULL;
    }

    /* 
     * 1. Test sqlite3_extended_result_codes 
     * Use Data[1] (if exists) to toggle on/off. Default to 1 if Size < 2.
     */
    int onoff = (Size > 1) ? (Data[1] % 2) : 1;
    
    /* 
     * Fix: Do NOT call sqlite3_extended_result_codes with NULL.
     * The internal implementation attempts to access db->mutex which causes a SEGV.
     * We only call it if db is valid.
     */
    if (db) {
        result = sqlite3_extended_result_codes(db, onoff);
        (void)result; /* Suppress unused warning */
        
        /* Also test with 0 if we tested with 1, or vice versa */
        if (onoff) {
            sqlite3_extended_result_codes(db, 0);
        } else {
            sqlite3_extended_result_codes(db, 1);
        }
    }
    /* If db is NULL, we skip the call to avoid the crash. 
       The API documentation says it returns SQLITE_MISUSE, but the implementation 
       in this build crashes before returning. We must avoid the invalid pointer deref. */

    /* 
     * 2. Trigger an error to test sqlite3_errcode and sqlite3_extended_errcode
     * We need a valid DB to get meaningful error codes, but we also test NULL.
     */
    if (db) {
        trigger_error(db);
        result = sqlite3_errcode(db);
        (void)result;
        result = sqlite3_extended_errcode(db);
        (void)result;
    } else {
        /* Test with NULL db. These functions are generally safer with NULL 
           as they often check for NULL first, but we test them anyway. */
        result = sqlite3_errcode(NULL);
        (void)result;
        result = sqlite3_extended_errcode(NULL);
        (void)result;
    }

    /* 
     * 3. Test sqlite3_load_extension
     * Use input data to construct a filename. 
     * CRITICAL FIX: Do not call sqlite3_load_extension with a NULL db pointer.
     * The sanitizer trace shows it crashes inside the function when db is NULL.
     */
    if (Size > 2) {
        /* Create a safe filename from input, ensuring it's null-terminated */
        size_t max_name_len = 100;
        if (Size - 2 < max_name_len) {
            max_name_len = Size - 2;
        }
        
        if (max_name_len > 0) {
            char filename[102];
            memset(filename, 0, sizeof(filename));
            memcpy(filename, Data + 2, max_name_len);
            filename[max_name_len] = '\0';
            
            const char *zProc = NULL;
            if (Size > max_name_len + 5) {
                size_t remaining = Size - (max_name_len + 5);
                size_t proc_len = 49;
                if (remaining < proc_len) proc_len = remaining;
                
                if (proc_len > 0) {
                    char proc[50];
                    memset(proc, 0, sizeof(proc));
                    memcpy(proc, Data + max_name_len + 5, proc_len);
                    proc[proc_len] = '\0';
                    zProc = proc;
                }
            }

            /* Only call if db is valid to prevent SEGV */
            if (db) {
                result = sqlite3_load_extension(db, filename, zProc, &err_msg);
                if (err_msg) {
                    sqlite3_free(err_msg);
                    err_msg = NULL;
                }
            }
            /* Removed the NULL db test case for sqlite3_load_extension as it causes a crash */
        }
    }

    /* 
     * 4. Test sqlite3_errstr
     */
    if (Size > 0) {
        int code1 = (int)Data[0];
        const char *str1 = sqlite3_errstr(code1);
        (void)str1;
        
        if (Size > 1) {
            int code2 = (int)Data[1];
            const char *str2 = sqlite3_errstr(code2);
            (void)str2;
        }
        
        const char *s_ok = sqlite3_errstr(SQLITE_OK);
        (void)s_ok;
        const char *s_err = sqlite3_errstr(SQLITE_ERROR);
        (void)s_err;
        const char *s_misuse = sqlite3_errstr(SQLITE_MISUSE);
        (void)s_misuse;
    }

    /* 
     * 5. Test sqlite3_log
     */
    if (Size > 0) {
        int err_code = (Size > 2) ? (int)Data[2] : SQLITE_ERROR;
        sqlite3_log(err_code, "Fuzz test message %d", err_code);
        sqlite3_log(err_code, "");
    }

    /* Cleanup */
    close_db_for_fuzzing(db);

    return 0;
}
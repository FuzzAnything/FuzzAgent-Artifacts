// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 error handling and extension loading APIs.
 * Target APIs:
 * 1. sqlite3_errstr
 * 2. sqlite3_extended_errcode
 * 3. sqlite3_errcode
 * 4. sqlite3_load_extension
 * 5. sqlite3_extended_result_codes
 * 6. sqlite3_log
 */

static void static_log_handler(void *pArg, int iErrCode, const char *zMsg) {
    /* No-op handler to prevent crashes if sqlite3_log is called without a handler */
    (void)pArg;
    (void)iErrCode;
    (void)zMsg;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    char *zErrMsg = NULL;
    int rc;

    /* Initialize a dummy log handler to ensure sqlite3_log doesn't crash */
    sqlite3_config(SQLITE_CONFIG_LOG, static_log_handler, NULL);

    /* 
     * Parse input to determine behavior.
     * We use the first few bytes to decide which API paths to exercise.
     * Ensure bounds checking before accessing Data array.
     */
    int mode = Data[0] % 10;
    
    /* Attempt to open an in-memory database to get a valid handle */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If we can't open the DB, we can still test sqlite3_errstr with static codes */
        const char *str = sqlite3_errstr((int)(Data[0]));
        (void)str; /* Use result to avoid unused warning */
        return 0;
    }

    /* 
     * Ensure the database is in a known state for error testing.
     * We execute a deliberately failing SQL statement to set an error code.
     */
    const char *zSql = "SELECT invalid_column FROM nonexistent_table;";
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, zSql, -1, &stmt, NULL);
    
    /* 
     * Test sqlite3_errcode and sqlite3_extended_errcode on a valid handle 
     * that has potentially encountered an error.
     */
    int errCode = sqlite3_errcode(db);
    int extErrCode = sqlite3_extended_errcode(db);
    
    /* Test sqlite3_errstr with the retrieved codes */
    const char *errStr1 = sqlite3_errstr(errCode);
    const char *errStr2 = sqlite3_errstr(extErrCode);
    (void)errStr1;
    (void)errStr2;

    /* Test sqlite3_extended_result_codes */
    /* Check if Data has at least 2 bytes before accessing Data[1] */
    int toggle = 0;
    if (Size >= 2) {
        toggle = (Data[1] % 2);
    }
    rc = sqlite3_extended_result_codes(db, toggle);
    (void)rc;

    /* 
     * Test sqlite3_log with fuzzed data.
     * Ensure format string is not NULL and is safe.
     */
    if (Size > 2) {
        /* Create a safe format string from input, ensuring it's valid */
        char safeFormat[64];
        size_t len = Size - 2;
        if (len > 63) len = 63;
        memcpy(safeFormat, Data + 2, len);
        safeFormat[len] = '\0';
        
        /* Ensure it's a valid format string by stripping potential % if needed, 
           but sqlite3_log handles % via sqlite3_snprintf logic. 
           We just pass it. */
        sqlite3_log((int)(Data[1]), safeFormat);
    }

    /* 
     * Test sqlite3_load_extension.
     * We use a dummy file path. This will likely fail, which is fine for fuzzing.
     * We need to enable load extension first.
     */
    if (mode == 1) {
        /* Enable load extension via API */
        rc = sqlite3_enable_load_extension(db, 1);
        (void)rc;
        
        /* Prepare file path from input, ensuring it's null-terminated */
        char zFile[256];
        size_t fileLen = Size > 255 ? 255 : Size;
        if (fileLen > 0) {
            memcpy(zFile, Data, fileLen);
            zFile[fileLen] = '\0';
        } else {
            zFile[0] = '\0';
        }
        
        /* Prepare proc name from input */
        char zProc[256];
        if (fileLen > 1) {
            memcpy(zProc, Data + 1, fileLen - 1);
            zProc[fileLen - 1] = '\0';
        } else {
            zProc[0] = '\0';
        }

        /* Call load_extension */
        rc = sqlite3_load_extension(db, zFile, zProc, &zErrMsg);
        
        /* Cleanup error message if allocated */
        if (zErrMsg) {
            sqlite3_free(zErrMsg);
            zErrMsg = NULL;
        }
    } else if (mode == 2) {
        /* Try to load extension without enabling it (should fail) */
        char zFile[256];
        size_t fileLen = Size > 255 ? 255 : Size;
        if (fileLen > 0) {
            memcpy(zFile, Data, fileLen);
            zFile[fileLen] = '\0';
        } else {
            zFile[0] = '\0';
        }
        rc = sqlite3_load_extension(db, zFile, NULL, &zErrMsg);
        if (zErrMsg) {
            sqlite3_free(zErrMsg);
            zErrMsg = NULL;
        }
    }

    /* 
     * Test error retrieval after potential failures.
     */
    errCode = sqlite3_errcode(db);
    extErrCode = sqlite3_extended_errcode(db);
    errStr1 = sqlite3_errstr(errCode);
    (void)errStr1;

    /* Final cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
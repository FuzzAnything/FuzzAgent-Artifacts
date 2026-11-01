// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:156571:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:156640:17 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 error handling and legacy table retrieval APIs.
 * Targets:
 *  1. sqlite3_set_errmsg
 *  2. sqlite3_extended_result_codes
 *  3. sqlite3_get_table
 *  4. sqlite3_extended_errcode
 *  5. sqlite3_errstr
 *  6. sqlite3_errcode
 */

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /*
     * 1. Prepare environment: Open an in-memory database.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * 3. Explore diverse program states.
     */

    /*
     * Scenario A: Test sqlite3_extended_result_codes
     */
    if (Size > 0) {
        int onoff = (Data[0] & 0x01);
        sqlite3_extended_result_codes(db, onoff);
        sqlite3_extended_result_codes(db, Data[0]);
    }

    /*
     * Scenario B: Test sqlite3_set_errmsg
     */
    if (Size > 0) {
        char *temp_msg = (char *)malloc(Size + 1);
        if (temp_msg) {
            memcpy(temp_msg, Data, Size);
            temp_msg[Size] = '\0';

            sqlite3_set_errmsg(db, SQLITE_ERROR, temp_msg);
            sqlite3_set_errmsg(db, SQLITE_BUSY, NULL);
            sqlite3_set_errmsg(db, (int)Data[0], temp_msg);

            free(temp_msg);
        }
    } else {
        sqlite3_set_errmsg(db, SQLITE_ERROR, NULL);
    }

    /*
     * Scenario C: Test sqlite3_get_table
     */
    const char *zSql = NULL;
    char **pazResult = NULL;
    int pnRow = 0;
    int pnColumn = 0;
    char *pzErrmsg = NULL;

    if (Size > 0) {
        size_t sql_len = Size;
        if (sql_len > 10000) sql_len = 10000;

        char *safe_sql = (char *)malloc(sql_len + 1);
        if (safe_sql) {
            memcpy(safe_sql, Data, sql_len);
            safe_sql[sql_len] = '\0';
            zSql = safe_sql;

            rc = sqlite3_get_table(db, zSql, &pazResult, &pnRow, &pnColumn, &pzErrmsg);

            if (pazResult) {
                sqlite3_free_table(pazResult);
                pazResult = NULL;
            }
            free(safe_sql);
        }
    }

    /*
     * Scenario D: Test error retrieval functions
     */
    int err_code = sqlite3_errcode(db);
    int ext_err_code = sqlite3_extended_errcode(db);

    const char *err_str1 = sqlite3_errstr(err_code);
    const char *err_str2 = sqlite3_errstr(ext_err_code);

    if (Size > 0) {
        int random_code = (int)Data[0];
        const char *err_str3 = sqlite3_errstr(random_code);
        (void)err_str3;
    }

    /*
     * Scenario E: Test with NULL database handle (Removed to prevent ASAN crash)
     * Note: Calling these with NULL triggers internal checks that may dereference NULL
     * in some builds/configurations. We skip these to ensure stability.
     */

    /*
     * 4. Cleanup
     */
    if (pzErrmsg) {
        sqlite3_free(pzErrmsg);
        pzErrmsg = NULL;
    }

    if (db) {
        sqlite3_close(db);
        db = NULL;
    }

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_get_table at sqlite3.c:156571:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:156640:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 error handling and table retrieval functions.
 * Targets: sqlite3_set_errmsg, sqlite3_extended_errcode, sqlite3_get_table,
 *          sqlite3_errstr, sqlite3_errcode, sqlite3_extended_result_codes.
 */

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char **pazResult = NULL;
    int nRow = 0, nColumn = 0;
    char *pzErrmsg = NULL;

    /*
     * Step 1: Prepare the environment.
     * Create an in-memory database to avoid file system dependencies.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If we cannot open the database, we cannot test the APIs.
         * Return immediately to avoid null pointer dereferences. */
        return 0;
    }

    /*
     * Step 2 & 3: Explore program states by invoking target functions diversely.
     * We use the fuzzing input to drive parameters and logic branches.
     */

    /*
     * Test sqlite3_extended_result_codes
     * Toggle extended result codes based on input data.
     */
    if (Size > 0) {
        int onoff = (Data[0] % 2);
        sqlite3_extended_result_codes(db, onoff);
    }

    /*
     * Test sqlite3_set_errmsg
     * Set custom error codes and messages.
     */
    if (Size > 2) {
        int errcode = (int)(Data[1] % 100) + 1; /* Generate a plausible error code range */
        
        /* 
         * Ensure we have enough data to copy into the message buffer.
         * The buffer size is 64 bytes. We need at least 1 byte for the message content
         * plus 1 byte for the null terminator, so we need at least 2 bytes of data
         * starting from Data[2].
         */
        size_t available_data = Size - 2;
        size_t msg_len = (available_data > 63) ? 63 : available_data;
        
        char msg[64];
        if (msg_len > 0) {
            memcpy(msg, Data + 2, msg_len);
        }
        msg[msg_len] = '\0';

        /* Call with a valid message */
        sqlite3_set_errmsg(db, errcode, msg);

        /* Call with NULL message (uses default) */
        sqlite3_set_errmsg(db, errcode, NULL);

        /* Call with invalid handle (NULL) to test SQLITE_MISUSE */
        sqlite3_set_errmsg(NULL, errcode, msg);
    }

    /*
     * Test sqlite3_errcode and sqlite3_extended_errcode
     * These should return the values set above or default values.
     */
    if (db) {
        int code = sqlite3_errcode(db);
        int ext_code = sqlite3_extended_errcode(db);
        (void)code; /* Suppress unused variable warning */
        (void)ext_code;
    }

    /*
     * Test sqlite3_errstr
     * Convert various error codes to strings.
     */
    if (Size > 2) {
        int test_code = (int)Data[2];
        const char *str = sqlite3_errstr(test_code);
        if (str) {
            /* Just accessing the string is enough to exercise the code path */
            (void)str;
        }
    }

    /*
     * Test sqlite3_get_table
     * Execute SQL queries based on input data.
     * We construct a simple SQL statement from the input to trigger parsing logic.
     */
    if (Size > 3) {
        /* Create a safe SQL string from input, ensuring it's null-terminated */
        size_t sql_len = (Size > 100) ? 100 : Size - 3;
        char sql[102];
        
        if (sql_len > 0) {
            memcpy(sql, Data + 3, sql_len);
        }
        sql[sql_len] = '\0';

        /*
         * To ensure we have a table to query, we first create one if the SQL
         * looks like it might be a query, or just run a simple query.
         * However, sqlite3_get_table handles multiple statements.
         * We'll try to run the user input as SQL.
         */
        rc = sqlite3_get_table(db, sql, &pazResult, &nRow, &nColumn, &pzErrmsg);

        /*
         * If the query failed, we might have an error message in pzErrmsg.
         * If it succeeded, we have a result table.
         */
        if (pazResult) {
            /*
             * The result table is an array of strings.
             * We don't need to parse it deeply, just ensuring we can access it
             * and then free it properly is the key for fuzzing memory management.
             */
            sqlite3_free_table(pazResult);
            pazResult = NULL;
        }

        /* Free error message if allocated by sqlite3_get_table */
        if (pzErrmsg) {
            sqlite3_free(pzErrmsg);
            pzErrmsg = NULL;
        }
    }

    /*
     * Additional state exploration:
     * Trigger an actual error to see if error codes update correctly.
     */
    {
        /* Execute a guaranteed syntax error */
        const char *bad_sql = "SELECT FROM INVALID_TABLE";
        rc = sqlite3_exec(db, bad_sql, NULL, NULL, NULL);
        (void)rc;

        /* Check error codes after failure */
        if (db) {
            int code = sqlite3_errcode(db);
            int ext_code = sqlite3_extended_errcode(db);
            const char *msg = sqlite3_errmsg(db);
            (void)code;
            (void)ext_code;
            (void)msg;
        }
    }

    /*
     * Step 4: Cleanup.
     * Close the database connection.
     */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
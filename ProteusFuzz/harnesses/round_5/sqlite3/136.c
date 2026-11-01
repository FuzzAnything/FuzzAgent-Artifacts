// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_get_table at sqlite3.c:156571:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:156640:17 in sqlite3.h
// sqlite3_get_table at sqlite3.c:156571:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:156640:17 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 error handling and legacy table retrieval APIs.
 * Target APIs:
 * 1. sqlite3_set_errmsg
 * 2. sqlite3_extended_result_codes
 * 3. sqlite3_get_table
 * 4. sqlite3_extended_errcode
 * 5. sqlite3_errstr
 * 6. sqlite3_errcode
 */

static int fuzz_sqlite3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char **pazResult = NULL;
    int nRow = 0, nColumn = 0;
    char *pzErrmsg = NULL;
    int errcode;
    const char *zErrMsg;
    int extended_on;

    /* Ensure we have enough data for basic operations */
    if (Size < 2) {
        return 0;
    }

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If we cannot open the database, try to test error functions with NULL db */
        goto test_null_db;
    }

    /* Use input data to drive fuzzing decisions */
    errcode = Data[0] & 0xFF;
    extended_on = (Data[1] & 0x01);
    
    /* Create a dummy table for sqlite3_get_table testing */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        /* Ignore creation errors, proceed with other tests */
    }

    /* Test sqlite3_extended_result_codes */
    rc = sqlite3_extended_result_codes(db, extended_on);
    (void)rc; /* Suppress unused variable warning */

    /* Test sqlite3_set_errmsg with various inputs */
    /* Extract a potential message string from input if possible */
    size_t msg_len = 0;
    if (Size > 2) {
        /* Find null terminator or use fixed length */
        for (size_t i = 2; i < Size; i++) {
            if (Data[i] == 0) {
                msg_len = i - 2;
                break;
            }
        }
        if (msg_len == 0) msg_len = Size - 2;
    }

    if (msg_len > 0 && msg_len < 1024) {
        char *msg_buf = (char *)malloc(msg_len + 1);
        if (msg_buf) {
            memcpy(msg_buf, Data + 2, msg_len);
            msg_buf[msg_len] = '\0';
            zErrMsg = msg_buf;
            
            rc = sqlite3_set_errmsg(db, errcode, zErrMsg);
            (void)rc;
            
            free(msg_buf);
        }
    } else {
        zErrMsg = NULL;
        rc = sqlite3_set_errmsg(db, errcode, zErrMsg);
        (void)rc;
    }

    /* Test sqlite3_get_table with various SQL queries */
    /* Construct SQL from input data */
    if (Size > 10) {
        size_t sql_len = Size - 10;
        if (sql_len > 500) sql_len = 500;
        
        char *sql_buf = (char *)malloc(sql_len + 1);
        if (sql_buf) {
            memcpy(sql_buf, Data + 10, sql_len);
            sql_buf[sql_len] = '\0';
            
            /* Ensure SQL is somewhat valid by prefixing with SELECT if needed */
            if (strlen(sql_buf) > 0 && 
                (strncmp(sql_buf, "SELECT", 6) != 0 && 
                 strncmp(sql_buf, "INSERT", 6) != 0 &&
                 strncmp(sql_buf, "UPDATE", 6) != 0 &&
                 strncmp(sql_buf, "DELETE", 6) != 0 &&
                 strncmp(sql_buf, "CREATE", 6) != 0 &&
                 strncmp(sql_buf, "DROP", 4) != 0)) {
                /* Prepend SELECT to make it more likely to be valid */
                char *safe_sql = (char *)malloc(sql_len + 10);
                if (safe_sql) {
                    snprintf(safe_sql, sql_len + 10, "SELECT * FROM test_table WHERE id = %d", Data[10] & 0xFF);
                    rc = sqlite3_get_table(db, safe_sql, &pazResult, &nRow, &nColumn, &pzErrmsg);
                    if (rc == SQLITE_OK && pazResult) {
                        sqlite3_free_table(pazResult);
                        pazResult = NULL;
                    }
                    free(safe_sql);
                }
            } else {
                rc = sqlite3_get_table(db, sql_buf, &pazResult, &nRow, &nColumn, &pzErrmsg);
                if (rc == SQLITE_OK && pazResult) {
                    sqlite3_free_table(pazResult);
                    pazResult = NULL;
                }
            }
            free(sql_buf);
        }
    }

    /* Test error retrieval functions */
    rc = sqlite3_errcode(db);
    (void)rc;
    
    rc = sqlite3_extended_errcode(db);
    (void)rc;
    
    const char *err_str = sqlite3_errstr(errcode);
    (void)err_str;

    /* Clean up */
    sqlite3_close(db);
    db = NULL;

    /* Test with NULL database handle */
test_null_db:
    /* Note: Many SQLite APIs do not safely handle NULL db handles internally 
       without specific compile-time flags (like SQLITE_ENABLE_API_ARMOR). 
       To prevent crashes in standard builds, we skip calling APIs that 
       dereference the db pointer immediately. 
       sqlite3_errstr is safe as it takes an int, not a db pointer.
       The others (set_errmsg, extended_result_codes, errcode, extended_errcode)
       are skipped for NULL db to avoid SEGV in this specific build.
    */
    
    /* Test sqlite3_errstr with various codes (Safe, does not use db pointer) */
    const char *safe_err_str = sqlite3_errstr(errcode);
    (void)safe_err_str;

    return 0;
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    /* Handle empty input */
    if (Size == 0) {
        return 0;
    }

    /* Run the fuzzing logic */
    fuzz_sqlite3(Data, Size);

    return 0;
}
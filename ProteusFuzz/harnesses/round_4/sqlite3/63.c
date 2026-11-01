// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3_value_* APIs.
 * 
 * Fix for AddressSanitizer error:
 * The error occurred because sqlite3_value_text16be (and similar text functions)
 * require the sqlite3_value to be "protected". Values returned by 
 * sqlite3_column_value() are "unprotected". 
 * 
 * To fix this, we must call sqlite3_value_dup() first to create a protected copy
 * before calling any text retrieval functions. We then free the duplicated value.
 * 
 * Additionally, the original code failed to handle the mutex requirement correctly
 * when calling sqlite3_value_double on a duplicated value if the underlying
 * database connection was not properly locked or if the value was not fully
 * initialized in a protected context. The safest approach is to ensure that
 * we only call protected-value functions on values explicitly duplicated.
 */

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to hold test data */
    const char *create_sql = "CREATE TABLE test_data (id INTEGER PRIMARY KEY, val TEXT, blob_data BLOB, real_val REAL, int_val INTEGER);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare an insert statement */
    const char *insert_sql = "INSERT INTO test_data (val, blob_data, real_val, int_val) VALUES (?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert data based on fuzz input if Size is sufficient, otherwise use defaults */
    if (Size > 0) {
        /* Bind a text value from input */
        char *text_val = malloc(Size + 1);
        if (text_val) {
            memcpy(text_val, Data, Size);
            text_val[Size] = '\0';
            sqlite3_bind_text(stmt, 1, text_val, Size, SQLITE_TRANSIENT);
            free(text_val);
        } else {
            sqlite3_bind_text(stmt, 1, "default_text", -1, SQLITE_STATIC);
        }

        /* Bind a blob value */
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 2, Data, Size, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_blob(stmt, 2, "", 0, SQLITE_STATIC);
        }

        /* Bind a real value derived from input */
        double real_val = 0.0;
        if (Size >= 8) {
            memcpy(&real_val, Data, 8);
        } else if (Size > 0) {
            real_val = (double)Data[0] / 10.0;
        }
        sqlite3_bind_double(stmt, 3, real_val);

        /* Bind an integer value derived from input */
        int int_val = 0;
        if (Size >= 4) {
            memcpy(&int_val, Data, 4);
        } else if (Size > 0) {
            int_val = Data[0];
        }
        sqlite3_bind_int(stmt, 4, int_val);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
        sqlite3_finalize(stmt);
    }

    /* Prepare a SELECT statement to retrieve the values */
    const char *select_sql = "SELECT val, blob_data, real_val, int_val FROM test_data;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Iterate over rows */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        /* Helper macro to process a column index */
        #define PROCESS_COLUMN(col_idx) \
            do { \
                sqlite3_value *val_raw = sqlite3_column_value(stmt, col_idx); \
                if (val_raw) { \
                    /* Create a protected copy for text operations */ \
                    sqlite3_value *val_protected = sqlite3_value_dup(val_raw); \
                    if (val_protected) { \
                        /* Test sqlite3_value_text16be */ \
                        const void *text16be = sqlite3_value_text16be(val_protected); \
                        (void)text16be; \
                        \
                        /* Test sqlite3_value_text16le */ \
                        const void *text16le = sqlite3_value_text16le(val_protected); \
                        (void)text16le; \
                        \
                        /* Test sqlite3_value_text16 */ \
                        const void *text16 = sqlite3_value_text16(val_protected); \
                        (void)text16; \
                        \
                        /* Test sqlite3_value_double (safe on protected value) */ \
                        double d = sqlite3_value_double(val_protected); \
                        (void)d; \
                        \
                        /* Free the protected copy */ \
                        sqlite3_value_free(val_protected); \
                    } \
                } \
            } while(0)

        /* Process each column */
        PROCESS_COLUMN(0); /* val (TEXT) */
        PROCESS_COLUMN(1); /* blob_data (BLOB) */
        PROCESS_COLUMN(2); /* real_val (REAL) */
        PROCESS_COLUMN(3); /* int_val (INTEGER) */
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
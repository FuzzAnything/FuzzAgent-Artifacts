// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:92610:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:93342:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:93342:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3_value_encoding, sqlite3_vtab_in_first,
 * sqlite3_column_value, sqlite3_value_frombind, sqlite3_bind_value,
 * and sqlite3_value_bytes16.
 *
 * Fix: The previous version crashed because sqlite3_column_value returns an
 * "unprotected" sqlite3_value. Functions like sqlite3_value_bytes16 and
 * sqlite3_value_encoding require a "protected" value (one where the mutex
 * is held). To fix this, we use sqlite3_value_dup() to create a protected
 * copy of the value before calling these functions, then free the copy.
 */

static int sqlite3_open_mem(sqlite3 **ppDb) {
    return sqlite3_open(":memory:", ppDb);
}

static void cleanup_stmt(sqlite3_stmt *pStmt) {
    if (pStmt) {
        sqlite3_finalize(pStmt);
    }
}

static void cleanup_db(sqlite3 *pDb) {
    if (pDb) {
        sqlite3_close(pDb);
    }
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Initialize database */
    rc = sqlite3_open_mem(&db);
    if (rc != SQLITE_OK) return 0;

    /* Create a simple table to work with */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    /* Prepare a statement with parameters to test bind and column_value */
    const char *insert_sql = "INSERT INTO test_table (name, data) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    if (Size >= 2) {
        /* Bind first parameter as TEXT */
        int len1 = (Data[0] % 100) + 1;
        if (len1 > Size - 1) len1 = Size - 1;
        
        rc = sqlite3_bind_text(stmt, 1, (const char*)&Data[1], len1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            cleanup_stmt(stmt);
            cleanup_db(db);
            return 0;
        }

        /* Bind second parameter as BLOB */
        int len2 = (Data[1] % 100) + 1;
        if (len2 > Size - 1) len2 = Size - 1;
        rc = sqlite3_bind_blob(stmt, 2, &Data[1], len2, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            cleanup_stmt(stmt);
            cleanup_db(db);
            return 0;
        }

        /* Execute the insert */
        rc = sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        /* Prepare a SELECT statement to test column_value and subsequent value functions */
        sqlite3_finalize(stmt);
        stmt = NULL;
        
        const char *select_sql = "SELECT name, data FROM test_table LIMIT 1;";
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            cleanup_db(db);
            return 0;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Test sqlite3_column_value */
            sqlite3_value *val1 = sqlite3_column_value(stmt, 0); /* name */
            sqlite3_value *val2 = sqlite3_column_value(stmt, 1); /* data */

            /* 
             * CRITICAL FIX: sqlite3_column_value returns an unprotected value.
             * Functions like sqlite3_value_bytes16, sqlite3_value_encoding, 
             * and sqlite3_value_frombind require a protected value.
             * We must duplicate the value to make it protected.
             */
            if (val1) {
                sqlite3_value *prot_val1 = sqlite3_value_dup(val1);
                if (prot_val1) {
                    /* Test sqlite3_value_encoding (only valid for TEXT) */
                    int enc = sqlite3_value_encoding(prot_val1);
                    (void)enc;

                    /* Test sqlite3_value_bytes16 */
                    int bytes16 = sqlite3_value_bytes16(prot_val1);
                    (void)bytes16;

                    /* Test sqlite3_value_frombind */
                    int frombind = sqlite3_value_frombind(prot_val1);
                    (void)frombind;

                    sqlite3_value_free(prot_val1);
                }
            }

            if (val2) {
                sqlite3_value *prot_val2 = sqlite3_value_dup(val2);
                if (prot_val2) {
                    /* Test sqlite3_value_encoding on BLOB */
                    int enc = sqlite3_value_encoding(prot_val2);
                    (void)enc;

                    /* Test sqlite3_value_bytes16 on BLOB */
                    int bytes16 = sqlite3_value_bytes16(prot_val2);
                    (void)bytes16;

                    sqlite3_value_free(prot_val2);
                }
            }
        }
        
        /* Test sqlite3_bind_value with a value from the result */
        sqlite3_reset(stmt);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_value *src_val = sqlite3_column_value(stmt, 0);
            if (src_val) {
                /* Prepare another statement to bind into */
                sqlite3_stmt *bind_stmt = NULL;
                const char *bind_sql = "INSERT INTO test_table (name) VALUES (?);";
                rc = sqlite3_prepare_v2(db, bind_sql, -1, &bind_stmt, NULL);
                if (rc == SQLITE_OK) {
                    /* 
                     * sqlite3_bind_value accepts unprotected values, so we can 
                     * pass src_val directly without duplication.
                     */
                    rc = sqlite3_bind_value(bind_stmt, 1, src_val);
                    if (rc == SQLITE_OK) {
                        sqlite3_step(bind_stmt);
                    }
                    sqlite3_finalize(bind_stmt);
                }
            }
        }
    }

    /* 
     * Test sqlite3_vtab_in_first 
     * This function is designed to be called from within xFilter.
     * Calling it outside that context should return SQLITE_ERROR.
     */
    sqlite3_value *dummy_val = NULL;
    sqlite3_value *out_val = NULL;
    
    /* Case 1: NULL input */
    rc = sqlite3_vtab_in_first(NULL, &out_val);
    (void)rc;

    /* Case 2: Valid pointer but not from xFilter context */
    if (stmt && sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_value *col_val = sqlite3_column_value(stmt, 0);
        if (col_val) {
            /* 
             * This should return SQLITE_ERROR because col_val is not a filter param.
             * Note: sqlite3_vtab_in_first expects a protected value from xFilter.
             * Passing an unprotected value here might trigger internal checks or
             * undefined behavior, but typically returns SQLITE_ERROR.
             * To be safe, we duplicate it to ensure it's protected, though the
             * context is still wrong.
             */
            sqlite3_value *prot_col_val = sqlite3_value_dup(col_val);
            if (prot_col_val) {
                rc = sqlite3_vtab_in_first(prot_col_val, &out_val);
                (void)rc;
                sqlite3_value_free(prot_col_val);
            }
        }
    }

    cleanup_stmt(stmt);
    cleanup_db(db);

    return 0;
}
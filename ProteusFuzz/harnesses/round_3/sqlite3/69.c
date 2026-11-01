// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:93342:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:93350:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:93342:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static sqlite3 *g_db = NULL;

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize database connection if not already done */
    if (g_db == NULL) {
        if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
            return 0;
        }
    }

    /* Prepare a statement to get a valid sqlite3_value for testing */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?1, ?2, ?3, ?4, ?5";
    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return 0;
    }

    /* Bind various types of data from the fuzz input */
    int idx = 0;
    
    /* Bind 1: Integer */
    if (idx + sizeof(int) <= Size) {
        int val_int = *(const int*)&Data[idx];
        idx += sizeof(int);
        sqlite3_bind_int(stmt, 1, val_int);
    } else {
        sqlite3_bind_int(stmt, 1, 0);
    }

    /* Bind 2: Text (ensure null termination) */
    if (idx < Size) {
        size_t len = Size - idx;
        if (len > 1000) len = 1000; /* Limit size */
        sqlite3_bind_text(stmt, 2, (const char*)&Data[idx], len, SQLITE_TRANSIENT);
        idx += len;
    } else {
        sqlite3_bind_text(stmt, 2, "", 0, SQLITE_STATIC);
    }

    /* Bind 3: Blob */
    if (idx < Size) {
        size_t len = Size - idx;
        if (len > 1000) len = 1000;
        sqlite3_bind_blob(stmt, 3, (const void*)&Data[idx], len, SQLITE_TRANSIENT);
        idx += len;
    } else {
        sqlite3_bind_blob(stmt, 3, "", 0, SQLITE_STATIC);
    }

    /* Bind 4: Float */
    if (idx + sizeof(double) <= Size) {
        double val_double = *(const double*)&Data[idx];
        idx += sizeof(double);
        sqlite3_bind_double(stmt, 4, val_double);
    } else {
        sqlite3_bind_double(stmt, 4, 0.0);
    }

    /* Bind 5: NULL */
    sqlite3_bind_null(stmt, 5);

    /* Step to make the statement valid and values accessible */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(g_db);
        g_db = NULL;
        return 0;
    }

    /* 
     * Get values from the statement.
     * Note: sqlite3_column_value returns an UNPROTECTED value.
     * The documentation states that sqlite3_value_int, sqlite3_value_blob, etc.
     * require PROTECTED values. To avoid the assertion failure in ASAN builds
     * (which checks for mutex ownership), we must duplicate the values first
     * to make them protected.
     */
    sqlite3_value *val1 = sqlite3_column_value(stmt, 0);
    sqlite3_value *val2 = sqlite3_column_value(stmt, 1);
    sqlite3_value *val3 = sqlite3_column_value(stmt, 2);
    sqlite3_value *val4 = sqlite3_column_value(stmt, 3);
    sqlite3_value *val5 = sqlite3_column_value(stmt, 4);

    /* Create protected copies for safe access */
    sqlite3_value *p_val1 = NULL;
    sqlite3_value *p_val2 = NULL;
    sqlite3_value *p_val3 = NULL;
    sqlite3_value *p_val4 = NULL;
    sqlite3_value *p_val5 = NULL;

    if (val1) p_val1 = sqlite3_value_dup(val1);
    if (val2) p_val2 = sqlite3_value_dup(val2);
    if (val3) p_val3 = sqlite3_value_dup(val3);
    if (val4) p_val4 = sqlite3_value_dup(val4);
    if (val5) p_val5 = sqlite3_value_dup(val5);

    /* Test sqlite3_value_int on protected value */
    if (p_val1) {
        int i1 = sqlite3_value_int(p_val1);
        (void)i1;
    }
    
    /* Test sqlite3_value_blob on protected value */
    if (p_val3) {
        const void *blob = sqlite3_value_blob(p_val3);
        if (blob) {
            /* Just access to ensure no crash */
            (void)blob;
        }
    }

    /* Test sqlite3_value_nochange on protected value */
    if (p_val1) {
        int nc = sqlite3_value_nochange(p_val1);
        (void)nc;
    }
    if (p_val5) {
        int nc = sqlite3_value_nochange(p_val5);
        (void)nc;
    }

    /* Test sqlite3_vtab_in_first and sqlite3_vtab_in_next */
    /* These are specifically for virtual table xFilter. 
     * Passing a regular value here is misuse, but we test for robustness.
     * We use the protected copies.
     */
    sqlite3_value *out_val = NULL;
    
    if (p_val1) {
        rc = sqlite3_vtab_in_first(p_val1, &out_val);
        (void)rc;
        if (out_val) {
            /* Try to iterate */
            rc = sqlite3_vtab_in_next(p_val1, &out_val);
            (void)rc;
        }
    }
    
    if (p_val2) {
        rc = sqlite3_vtab_in_first(p_val2, &out_val);
        (void)rc;
    }

    /* Cleanup duplicated values */
    if (p_val1) sqlite3_value_free(p_val1);
    if (p_val2) sqlite3_value_free(p_val2);
    if (p_val3) sqlite3_value_free(p_val3);
    if (p_val4) sqlite3_value_free(p_val4);
    if (p_val5) sqlite3_value_free(p_val5);

    /* Finalize statement */
    sqlite3_finalize(stmt);

    return 0;
}
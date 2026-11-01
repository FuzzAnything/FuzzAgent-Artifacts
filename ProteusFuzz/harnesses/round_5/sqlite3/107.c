// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:93350:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:95598:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Helper function to create a protected sqlite3_value from a statement column.
 * This is necessary because the target APIs require protected values.
 * Note: sqlite3_column_value returns an UNPROTECTED value.
 * We must use sqlite3_value_dup to create a PROTECTED copy for accessors.
 */
static sqlite3_value *get_protected_value(sqlite3_stmt *stmt, int col) {
    if (stmt == NULL) return NULL;
    sqlite3_value *raw_val = sqlite3_column_value(stmt, col);
    if (raw_val == NULL) return NULL;
    // Create a protected copy. The caller is responsible for freeing this with sqlite3_value_free.
    return sqlite3_value_dup(raw_val);
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to hold test data */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement to insert data */
    const char *insert_sql = "INSERT INTO test_table (data) VALUES (?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzzing input as a BLOB */
    sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);

    /* Execute the insert */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    sqlite3_reset(stmt);

    /* Prepare a statement to retrieve the data */
    const char *select_sql = "SELECT data FROM test_table LIMIT 1";
    sqlite3_finalize(stmt);
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step to get the row */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Get a protected value from the column */
    /* get_protected_value now returns a DUPLICATED (protected) value */
    sqlite3_value *pVal = get_protected_value(stmt, 0);
    if (pVal == NULL) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* 1. sqlite3_value_numeric_type */
    int numeric_type = sqlite3_value_numeric_type(pVal);
    (void)numeric_type;

    /* 2. sqlite3_value_bytes */
    int bytes_len = sqlite3_value_bytes(pVal);
    (void)bytes_len;

    /* 3. sqlite3_value_blob */
    const void *blob_data = sqlite3_value_blob(pVal);
    if (blob_data != NULL) {
        volatile const void *ptr = blob_data;
        (void)ptr;
    }

    /* 4. sqlite3_value_text */
    /* Now safe to call because pVal is protected (mutex held via dup) */
    const unsigned char *text_data = sqlite3_value_text(pVal);
    if (text_data != NULL) {
        volatile const unsigned char *ptr = text_data;
        (void)ptr;
    }

    /* 5. sqlite3_value_nochange */
    int is_nochange = sqlite3_value_nochange(pVal);
    (void)is_nochange;

    /*
     * 6. sqlite3_vtab_in_next
     * This function is specific to virtual table IN-list iterators.
     * Calling it with a standard BLOB value is likely to fail logically or return an error,
     * but it should not crash the mutex assertion.
     */
    sqlite3_value *out_val = NULL;
    int vtab_rc = sqlite3_vtab_in_next(pVal, &out_val);
    (void)vtab_rc;
    (void)out_val;

    /* Cleanup */
    /* Must free the duplicated value */
    sqlite3_value_free(pVal);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
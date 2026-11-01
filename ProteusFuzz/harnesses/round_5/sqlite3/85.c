// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_value_* functions.
 * Target APIs:
 *   sqlite3_value_text16be
 *   sqlite3_value_text16
 *   sqlite3_value_double
 *   sqlite3_value_free
 *   sqlite3_value_dup
 *   sqlite3_value_text16le
 *
 * Fix: sqlite3_column_value() returns an "unprotected" sqlite3_value.
 * Accessing its content (e.g., via sqlite3_value_double) without holding
 * the database mutex triggers an assertion in debug/asan builds.
 * We must use sqlite3_value_dup() to create a "protected" copy before
 * accessing the value's content.
 */

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    if (Size < 4) {
        return 0;
    }

    /* Initialize database connection in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a simple table and insert some test data */
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, val TEXT, num REAL, blob_data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare an insert statement to fuzz data injection */
    const char *insert_sql = "INSERT INTO test (val, num, blob_data) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Use input data to bind values */
    size_t text_len = (Size / 3);
    if (text_len == 0) text_len = 1;
    if (text_len > Size) text_len = Size;

    rc = sqlite3_bind_text(stmt, 1, (const char *)(Data + 0), (int)text_len, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Bind second chunk as double */
    double test_double = 0.0;
    /* Fix: Ensure we have enough bytes remaining before reading 8 bytes */
    if (Size >= text_len + 8) {
        memcpy(&test_double, Data + text_len, 8);
    }
    rc = sqlite3_bind_double(stmt, 2, test_double);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Bind third chunk as blob */
    size_t blob_start = text_len + 8;
    size_t blob_len = 0;
    if (Size > blob_start) {
        blob_len = Size - blob_start;
        rc = sqlite3_bind_blob(stmt, 3, (const void *)(Data + blob_start), (int)blob_len, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    } else {
        /* Bind empty blob if no data left */
        rc = sqlite3_bind_blob(stmt, 3, NULL, 0, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    }

    /* Execute insert */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);

    /* Now prepare a SELECT to retrieve values and test the target APIs */
    const char *select_sql = "SELECT val, num, blob_data FROM test LIMIT 1;";
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

    /* Retrieve sqlite3_value objects from columns */
    /* Note: sqlite3_column_value returns UNPROTECTED values. 
       We must duplicate them to make them PROTECTED before calling value_* accessors. */
    sqlite3_value *raw_val_text = sqlite3_column_value(stmt, 0);
    sqlite3_value *raw_val_num = sqlite3_column_value(stmt, 1);
    sqlite3_value *raw_val_blob = sqlite3_column_value(stmt, 2);

    /* Create protected copies */
    sqlite3_value *val_text = sqlite3_value_dup(raw_val_text);
    sqlite3_value *val_num = sqlite3_value_dup(raw_val_num);
    sqlite3_value *val_blob = sqlite3_value_dup(raw_val_blob);

    /* Test sqlite3_value_double on protected value */
    if (val_num) {
        double d = sqlite3_value_double(val_num);
        (void)d;
    }

    /* Test sqlite3_value_text16be */
    if (val_text) {
        const void *t16be = sqlite3_value_text16be(val_text);
        (void)t16be;
    }

    /* Test sqlite3_value_text16le */
    if (val_text) {
        const void *t16le = sqlite3_value_text16le(val_text);
        (void)t16le;
    }

    /* Test sqlite3_value_text16 */
    if (val_text) {
        const void *t16 = sqlite3_value_text16(val_text);
        (void)t16;
    }

    /* Test sqlite3_value_dup again (nested duplication) and sqlite3_value_free */
    sqlite3_value *dup_text = NULL;
    sqlite3_value *dup_num = NULL;
    sqlite3_value *dup_blob = NULL;

    if (val_text) {
        dup_text = sqlite3_value_dup(val_text);
    }
    if (val_num) {
        dup_num = sqlite3_value_dup(val_num);
    }
    if (val_blob) {
        dup_blob = sqlite3_value_dup(val_blob);
    }

    /* Perform operations on duplicated values if successful */
    if (dup_text) {
        const void *t16be_dup = sqlite3_value_text16be(dup_text);
        const void *t16le_dup = sqlite3_value_text16le(dup_text);
        const void *t16_dup = sqlite3_value_text16(dup_text);
        (void)t16be_dup;
        (void)t16le_dup;
        (void)t16_dup;
    }
    if (dup_num) {
        double d_dup = sqlite3_value_double(dup_num);
        (void)d_dup;
    }
    if (dup_blob) {
        /* Try to read blob as text, should return NULL */
        const void *t16be_blob = sqlite3_value_text16be(dup_blob);
        const void *t16le_blob = sqlite3_value_text16le(dup_blob);
        const void *t16_blob = sqlite3_value_text16(dup_blob);
        (void)t16be_blob;
        (void)t16le_blob;
        (void)t16_blob;
    }

    /* Free duplicated values (the second level of duplication) */
    if (dup_text) {
        sqlite3_value_free(dup_text);
    }
    if (dup_num) {
        sqlite3_value_free(dup_num);
    }
    if (dup_blob) {
        sqlite3_value_free(dup_blob);
    }

    /* Free the first level of duplication (the protected copies from column_value) */
    if (val_text) {
        sqlite3_value_free(val_text);
    }
    if (val_num) {
        sqlite3_value_free(val_num);
    }
    if (val_blob) {
        sqlite3_value_free(val_blob);
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
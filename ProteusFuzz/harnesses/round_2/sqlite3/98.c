// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:92485:18 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Helper to create a simple test database in memory */
static int setup_database(sqlite3 **db_out) {
    int rc = sqlite3_open(":memory:", db_out);
    if (rc != SQLITE_OK) {
        if (*db_out) {
            sqlite3_close(*db_out);
            *db_out = NULL;
        }
        return -1;
    }

    const char *sql = 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "data BLOB, "
        "value REAL "
        "); "
        "INSERT INTO test_table VALUES (1, 'hello', X'010203', 3.14); "
        "INSERT INTO test_table VALUES (2, 'world', X'ABCD', 2.71);";
    
    char *err_msg = NULL;
    rc = sqlite3_exec(*db_out, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(*db_out);
        *db_out = NULL;
        return -1;
    }
    return 0;
}

/* Helper to cleanup */
static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    /* 1. Prepare the environment */
    if (setup_database(&db) != 0) {
        return 0;
    }

    /* Prepare a statement to fetch values */
    const char *sql = "SELECT name, data, value, id FROM test_table WHERE id <= ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cleanup_database(db);
        return 0;
    }

    /* Use input data to influence the query parameter (id limit) */
    int limit = (Size > 0) ? (Data[0] % 10) + 1 : 1;
    if (limit < 1) limit = 1;
    sqlite3_bind_int(stmt, 1, limit);

    /* 3. Explore program states by invoking target functions diversely */
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        /* Fetch values from different columns */
        sqlite3_value *v_text = sqlite3_column_value(stmt, 0); /* name */
        sqlite3_value *v_blob = sqlite3_column_value(stmt, 1); /* data */
        sqlite3_value *v_real = sqlite3_column_value(stmt, 2); /* value */
        sqlite3_value *v_int = sqlite3_column_value(stmt, 3);  /* id */

        /* 
         * CRITICAL FIX: sqlite3_column_value returns an UNPROTECTED value.
         * Functions like sqlite3_value_text16be require a PROTECTED value.
         * We must use sqlite3_value_dup to create a protected copy before calling
         * text retrieval functions.
         */

        /* Test sqlite3_value_dup on various types to get protected copies */
        sqlite3_value *dup_text = sqlite3_value_dup(v_text);
        sqlite3_value *dup_blob = sqlite3_value_dup(v_blob);
        sqlite3_value *dup_real = sqlite3_value_dup(v_real);
        sqlite3_value *dup_int = sqlite3_value_dup(v_int);

        /* Test text retrieval functions on text value (using protected copy) */
        if (dup_text) {
            const void *t16be = sqlite3_value_text16be(dup_text);
            (void)t16be; /* Suppress unused warning */
            
            const void *t16 = sqlite3_value_text16(dup_text);
            (void)t16;

            const void *t16le = sqlite3_value_text16le(dup_text);
            (void)t16le;
        }

        /* Test text retrieval functions on non-text values (should return NULL) */
        if (dup_blob) {
            const void *t16be_blob = sqlite3_value_text16be(dup_blob);
            (void)t16be_blob;
            const void *t16_blob = sqlite3_value_text16(dup_blob);
            (void)t16_blob;
            const void *t16le_blob = sqlite3_value_text16le(dup_blob);
            (void)t16le_blob;
        }

        /* Test pointer retrieval (expect NULL as we didn't bind pointers) */
        if (dup_int) {
            void *ptr = sqlite3_value_pointer(dup_int, "my_subtype");
            (void)ptr;
        }

        /* Test sqlite3_value_dup on NULL input (should return NULL) */
        sqlite3_value *dup_null = sqlite3_value_dup(NULL);
        (void)dup_null;

        /* 2. Cleanup: Free duplicated values */
        if (dup_text) sqlite3_value_free(dup_text);
        if (dup_blob) sqlite3_value_free(dup_blob);
        if (dup_real) sqlite3_value_free(dup_real);
        if (dup_int) sqlite3_value_free(dup_int);
        if (dup_null) sqlite3_value_free(dup_null); /* Should be harmless no-op */

        /* Test freeing NULL explicitly */
        sqlite3_value_free(NULL);
    }

    /* Cleanup statement and database */
    sqlite3_finalize(stmt);
    cleanup_database(db);

    return 0;
}
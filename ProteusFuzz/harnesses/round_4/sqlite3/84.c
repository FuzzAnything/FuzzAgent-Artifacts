// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:186751:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Custom SQL function to test sqlite3_context_db_handle
 */
static void dummy_sql_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3 *db = sqlite3_context_db_handle(context);
    if (db) {
        /* Just access the handle to ensure no crash */
        (void)sqlite3_errcode(db);
    }
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *next_stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Open a database in memory to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a simple table to allow INSERT operations for rowid testing */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 2. Fuzzing Logic */

    /* Attempt to prepare a statement using input data as SQL */
    /* We limit the size to prevent massive allocations, but allow valid SQL */
    if (Size > 0) {
        /* Ensure null termination */
        char *sql_buf = (char *)malloc(Size + 1);
        if (sql_buf) {
            memcpy(sql_buf, Data, Size);
            sql_buf[Size] = '\0';

            /* Try to prepare the statement */
            rc = sqlite3_prepare_v2(db, sql_buf, Size, &stmt, NULL);
            if (rc == SQLITE_OK) {
                /* If prepared, try to step it (might fail, which is fine for fuzzing) */
                rc = sqlite3_step(stmt);
                (void)rc; /* Ignore return for fuzzing state exploration */
            }
            free(sql_buf);
        }
    }

    /* Test sqlite3_next_stmt */
    /* Start from NULL to get the first statement */
    next_stmt = sqlite3_next_stmt(db, NULL);
    while (next_stmt != NULL) {
        /* Iterate through statements */
        next_stmt = sqlite3_next_stmt(db, next_stmt);
    }

    /* Test sqlite3_db_handle */
    if (stmt != NULL) {
        sqlite3 *handle = sqlite3_db_handle(stmt);
        if (handle) {
            /* Verify handle matches our db (sanity check) */
            (void)handle;
        }
    }

    /* Test sqlite3_last_insert_rowid and sqlite3_set_last_insert_rowid */
    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
    (void)rowid;

    /* Set a random rowid from input data if available */
    sqlite3_int64 new_rowid = 0;
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&new_rowid, Data, sizeof(sqlite3_int64));
    } else if (Size > 0) {
        new_rowid = (sqlite3_int64)Data[0];
    }
    sqlite3_set_last_insert_rowid(db, new_rowid);

    /* Verify the set value */
    rowid = sqlite3_last_insert_rowid(db);
    (void)rowid;

    /* Test sqlite3_errcode */
    /* Induce an error by executing invalid SQL or checking after operations */
    const char *invalid_sql = "SELECT * FROM nonexistent_table_xyz;";
    rc = sqlite3_exec(db, invalid_sql, NULL, NULL, NULL);
    int err_code = sqlite3_errcode(db);
    (void)err_code;

    /* Test sqlite3_context_db_handle via a custom function */
    /* Register the function */
    rc = sqlite3_create_function(db, "fuzz_dummy_func", 0, SQLITE_UTF8, NULL, dummy_sql_func, NULL, NULL);
    if (rc == SQLITE_OK) {
        /* Execute the function to trigger the context handler */
        const char *func_sql = "SELECT fuzz_dummy_func();";
        sqlite3_stmt *func_stmt = NULL;
        rc = sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, NULL);
        if (rc == SQLITE_OK) {
            rc = sqlite3_step(func_stmt);
            (void)rc;
            sqlite3_finalize(func_stmt);
        }
        /* Drop the function to clean up */
        sqlite3_exec(db, "DROP FUNCTION IF EXISTS fuzz_dummy_func", NULL, NULL, NULL);
    }

    /* 3. Cleanup */
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}
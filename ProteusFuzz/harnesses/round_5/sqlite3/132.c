// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:186751:17 in sqlite3.h
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
 * Fuzz driver for sqlite3_last_insert_rowid, sqlite3_set_last_insert_rowid,
 * sqlite3_next_stmt, sqlite3_errcode, sqlite3_db_handle, sqlite3_context_db_handle.
 *
 * This driver creates a database connection, prepares statements, performs inserts,
 * and exercises the target APIs with diverse inputs derived from the fuzzing data.
 */

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_stmt *pStmt = sqlite3_next_stmt(db, NULL);
        while (pStmt) {
            sqlite3_stmt *pNext = sqlite3_next_stmt(db, pStmt);
            sqlite3_finalize(pStmt);
            pStmt = pNext;
        }
        sqlite3_close(db);
    }
}

/* Custom SQL function to exercise sqlite3_context_db_handle */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* Exercise sqlite3_context_db_handle */
    sqlite3 *db = sqlite3_context_db_handle(ctx);
    if (db) {
        /* Just ensure the handle is valid by checking errcode */
        int rc = sqlite3_errcode(db);
        (void)rc;
    }
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a simple table for testing inserts */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    /* Register a custom function to test sqlite3_context_db_handle */
    rc = sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    /* Prepare a statement for inserting data */
    sqlite3_stmt *insert_stmt = NULL;
    const char *insert_sql = "INSERT INTO test_table (data) VALUES (?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL);
    if (rc != SQLITE_OK || insert_stmt == NULL) {
        cleanup_db(db);
        return 0;
    }

    /* Prepare a statement for selecting data to exercise next_stmt */
    sqlite3_stmt *select_stmt = NULL;
    const char *select_sql = "SELECT * FROM test_table";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL);
    if (rc != SQLITE_OK || select_stmt == NULL) {
        sqlite3_finalize(insert_stmt);
        cleanup_db(db);
        return 0;
    }

    /* Extract data from fuzz input for testing */
    int64_t rowid_val = 0;
    if (Size >= sizeof(int64_t)) {
        memcpy(&rowid_val, Data, sizeof(int64_t));
    }

    /* Exercise sqlite3_set_last_insert_rowid */
    sqlite3_set_last_insert_rowid(db, rowid_val);

    /* Exercise sqlite3_last_insert_rowid before any insert */
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    (void)last_rowid;

    /* Perform an insert if we have enough data */
    if (Size > sizeof(int64_t)) {
        const char *data_str = (const char *)(Data + sizeof(int64_t));
        size_t data_len = Size - sizeof(int64_t);
        /* Ensure null termination for safety, though bind_blob handles length */
        sqlite3_bind_text(insert_stmt, 1, data_str, data_len, SQLITE_TRANSIENT);
        rc = sqlite3_step(insert_stmt);
        if (rc == SQLITE_DONE) {
            /* Exercise sqlite3_last_insert_rowid after successful insert */
            last_rowid = sqlite3_last_insert_rowid(db);
            (void)last_rowid;
        } else {
            /* Exercise sqlite3_errcode on failure */
            int err = sqlite3_errcode(db);
            (void)err;
        }
        sqlite3_reset(insert_stmt);
    }

    /* Exercise sqlite3_next_stmt */
    sqlite3_stmt *pStmt = sqlite3_next_stmt(db, NULL);
    while (pStmt != NULL) {
        /* Exercise sqlite3_db_handle */
        sqlite3 *stmt_db = sqlite3_db_handle(pStmt);
        if (stmt_db) {
            int err = sqlite3_errcode(stmt_db);
            (void)err;
        }
        pStmt = sqlite3_next_stmt(db, pStmt);
    }

    /* Test sqlite3_errcode with various states */
    int err_code = sqlite3_errcode(db);
    (void)err_code;

    /* Execute a custom function to trigger sqlite3_context_db_handle */
    const char *func_sql = "SELECT dummy_func()";
    sqlite3_stmt *func_stmt = NULL;
    rc = sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, NULL);
    if (rc == SQLITE_OK && func_stmt != NULL) {
        rc = sqlite3_step(func_stmt);
        sqlite3_finalize(func_stmt);
    }

    /* Cleanup */
    sqlite3_finalize(insert_stmt);
    sqlite3_finalize(select_stmt);
    cleanup_db(db);

    return 0;
}
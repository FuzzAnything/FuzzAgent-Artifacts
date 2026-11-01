// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:186751:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* Helper to cleanup statements */
static void cleanup_stmt(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

/* Dummy function for testing sqlite3_context_db_handle */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* Retrieve db handle from context */
    sqlite3 *ctx_db = sqlite3_context_db_handle(ctx);
    (void)ctx_db;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *next_stmt = NULL;
    int rc;
    sqlite3_int64 rowid;
    int err_code;
    sqlite3 *stmt_db;

    /* 1. Prepare the environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a simple table to allow INSERTs for testing rowid logic */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 2. Parse input data to drive diverse states */
    if (Size > 0) {
        /* Use input data to generate a rowid value */
        rowid = (sqlite3_int64)Data[0];
        if (Size > 1) {
            rowid = (rowid << 8) | Data[1];
        }
        if (Size > 2) {
            rowid = (rowid << 8) | Data[2];
        }
        if (Size > 3) {
            rowid = (rowid << 8) | Data[3];
        }
    } else {
        rowid = 0;
    }

    /* 3. Invoke target functions diversely */

    /* Test sqlite3_set_last_insert_rowid */
    sqlite3_set_last_insert_rowid(db, rowid);

    /* Test sqlite3_last_insert_rowid */
    rowid = sqlite3_last_insert_rowid(db);

    /* Test sqlite3_errcode (should be 0 or previous error) */
    err_code = sqlite3_errcode(db);

    /* Prepare a statement to test sqlite3_next_stmt and sqlite3_db_handle */
    const char *select_sql = "SELECT * FROM test_table LIMIT 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        /* Test sqlite3_db_handle */
        stmt_db = sqlite3_db_handle(stmt);
        (void)stmt_db; /* Use variable to avoid unused warning */

        /* Test sqlite3_next_stmt starting from NULL */
        next_stmt = sqlite3_next_stmt(db, NULL);
        
        /* Test sqlite3_next_stmt starting from current stmt */
        if (next_stmt) {
            sqlite3_stmt *next2 = sqlite3_next_stmt(db, next_stmt);
            (void)next2;
        }

        /* Test sqlite3_next_stmt with invalid statement (NULL) on valid db */
        next_stmt = sqlite3_next_stmt(db, NULL);
    }

    /* Create an error condition to test sqlite3_errcode */
    const char *invalid_sql = "SELECT * FROM non_existent_table_xyz;";
    sqlite3_stmt *err_stmt = NULL;
    rc = sqlite3_prepare_v2(db, invalid_sql, -1, &err_stmt, NULL);
    if (rc != SQLITE_OK) {
        /* Error occurred, check errcode */
        err_code = sqlite3_errcode(db);
        (void)err_code;
    }
    if (err_stmt) {
        cleanup_stmt(err_stmt);
    }

    /* Test sqlite3_context_db_handle requires a registered function.
     * We register a dummy function to get a valid context.
     */
    static int dummy_func_count = 0;
    
    /* Register the function if not already registered (idempotent-ish for fuzzing) */
    if (dummy_func_count == 0) {
        rc = sqlite3_create_function(db, "fuzz_dummy", 0, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);
        if (rc == SQLITE_OK) {
            /* Execute the function to trigger context creation */
            const char *call_sql = "SELECT fuzz_dummy();";
            sqlite3_stmt *func_stmt = NULL;
            rc = sqlite3_prepare_v2(db, call_sql, -1, &func_stmt, NULL);
            if (rc == SQLITE_OK && func_stmt) {
                rc = sqlite3_step(func_stmt);
                cleanup_stmt(func_stmt);
            }
        }
        dummy_func_count++;
    }

    /* 4. Cleanup */
    cleanup_stmt(stmt);
    sqlite3_close(db);

    return 0;
}
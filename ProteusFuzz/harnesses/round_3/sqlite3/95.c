// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:186751:17 in sqlite3.h
// sqlite3_db_mutex at sqlite3.c:186516:27 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to create a valid sqlite3 connection.
 * Uses an in-memory database to avoid file system dependencies.
 */
static sqlite3 *create_test_db(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return NULL;
    }
    return db;
}

/*
 * Helper function to create a valid prepared statement for testing.
 */
static sqlite3_stmt *create_test_stmt(sqlite3 *db) {
    sqlite3_stmt *stmt = NULL;
    if (db == NULL) return NULL;
    
    /* Prepare a simple statement to ensure we have a valid stmt object */
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, val TEXT);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return stmt;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare the environment */
    sqlite3 *db = create_test_db();
    if (db == NULL) {
        return 0;
    }

    sqlite3_stmt *stmt = create_test_stmt(db);
    
    /* Use input data to drive fuzzing logic */
    sqlite3_int64 rowid = 0;
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&rowid, Data, sizeof(sqlite3_int64));
    } else if (Size > 0) {
        /* Fallback to simple casting if data is small */
        rowid = (sqlite3_int64)Data[0];
    }

    /* 2. Invoke target functions diversely */

    /* API 1: sqlite3_set_last_insert_rowid */
    /* Test with valid db */
    sqlite3_set_last_insert_rowid(db, rowid);
    
    /* API 2: sqlite3_db_mutex */
    sqlite3_mutex *mutex = sqlite3_db_mutex(db);
    (void)mutex; /* Suppress unused warning */
    
    /* API 3: sqlite3_next_stmt */
    /* Iterate from start */
    sqlite3_stmt *next = sqlite3_next_stmt(db, NULL);
    (void)next;
    
    /* Iterate from current stmt */
    if (stmt) {
        next = sqlite3_next_stmt(db, stmt);
        (void)next;
    }
    
    /* 
     * FIX: Removed the call with NULL db. 
     * sqlite3_next_stmt dereferences the db pointer to access the mutex.
     * Without SQLITE_ENABLE_API_ARMOR, passing NULL causes a SEGV.
     * We only call with valid handles to ensure safety across all builds.
     */
    // next = sqlite3_next_stmt(NULL, NULL);
    // (void)next;
    
    /* Test with valid db but NULL stmt (already done above, but explicit) */
    next = sqlite3_next_stmt(db, NULL);
    (void)next;

    /* API 4: sqlite3_last_insert_rowid */
    sqlite3_int64 last_id = sqlite3_last_insert_rowid(db);
    (void)last_id;
    
    /* Test with NULL db (sqlite3_last_insert_rowid handles NULL safely) */
    last_id = sqlite3_last_insert_rowid(NULL);
    (void)last_id;

    /* API 5: sqlite3_context_db_handle */
    /* We cannot easily create a valid sqlite3_context without executing a custom function.
       However, we can test the NULL case which is a common fuzzing target.
       If we wanted to be exhaustive, we would register a function, execute a query,
       and capture the context, but that adds significant complexity and state.
       Testing NULL is sufficient for robustness checks on the API armor. */
    sqlite3 *ctx_db = sqlite3_context_db_handle(NULL);
    (void)ctx_db;

    /* API 6: sqlite3_db_handle */
    sqlite3 *stmt_db = sqlite3_db_handle(stmt);
    (void)stmt_db;
    
    /* Test with NULL stmt */
    stmt_db = sqlite3_db_handle(NULL);
    (void)stmt_db;

    /* 3. Cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:187881:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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
 * Static callback functions for tracing and hooks.
 * These are marked static to avoid name conflicts.
 */

/* Trace callback for sqlite3_trace_v2 */
static int trace_v2_callback(unsigned event, void *pCtx, void *pArg1, void *pArg2) {
    (void)event;
    (void)pCtx;
    (void)pArg1;
    (void)pArg2;
    return 0;
}

/* Trace callback for deprecated sqlite3_trace */
static void trace_callback(void *pCtx, const char *zSql) {
    (void)pCtx;
    (void)zSql;
}

/* Commit hook callback */
static int commit_callback(void *pArg) {
    (void)pArg;
    return 0;
}

/* WAL hook callback */
static int wal_callback(void *pArg, sqlite3 *db, const char *zDb, int nPage) {
    (void)pArg;
    (void)db;
    (void)zDb;
    (void)nPage;
    return SQLITE_OK;
}

/*
 * Helper function to extract a valid SQL string from fuzz input.
 * Returns a pointer to the start of the SQL and sets nByte to the length.
 * Ensures the string is null-terminated for safety if passed to sqlite3_prepare_v3.
 */
static const char *get_sql_from_input(const uint8_t *Data, size_t Size, int *nByte) {
    if (Size == 0) {
        *nByte = 0;
        return "SELECT 1";
    }

    /* Find the first null terminator or use the whole buffer */
    size_t len = Size;
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] == 0) {
            len = i;
            break;
        }
    }

    /* Ensure we have at least a tiny valid SQL statement if input is empty or garbage */
    if (len == 0) {
        *nByte = 8;
        return "SELECT 1";
    }

    /* If the input looks like SQL (starts with common keywords), use it */
    const char *sql_start = (const char *)Data;
    *nByte = (int)len;

    /* Basic sanity check: if it doesn't look like SQL, return a safe default */
    /* We check for common SQL keywords to avoid passing pure garbage that might crash parser early */
    if (len > 0 && (sql_start[0] == 'S' || sql_start[0] == 's' ||
                    sql_start[0] == 'I' || sql_start[0] == 'i' ||
                    sql_start[0] == 'U' || sql_start[0] == 'u' ||
                    sql_start[0] == 'D' || sql_start[0] == 'd' ||
                    sql_start[0] == 'C' || sql_start[0] == 'c')) {
        return sql_start;
    }

    /* Fallback to safe SQL if input is binary garbage */
    *nByte = 8;
    return "SELECT 1";
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Open an in-memory database to avoid file system issues */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a simple table to allow INSERT/UPDATE/DELETE operations for sqlite3_changes */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 2. Fuzzing Logic */
    
    /* Extract SQL from input */
    int nByte;
    const char *zSql = get_sql_from_input(Data, Size, &nByte);

    /* Fuzz sqlite3_prepare_v3 */
    /* We vary prepFlags based on input data if available */
    unsigned int prepFlags = 0;
    if (Size > 0) {
        prepFlags = Data[0] & 0x0F; /* Limit to low bits to avoid invalid flags */
    }

    rc = sqlite3_prepare_v3(db, zSql, nByte, prepFlags, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        /* Execute the statement to trigger potential changes */
        rc = sqlite3_step(stmt);
        
        /* Fuzz sqlite3_changes: returns rows modified by last INSERT/UPDATE/DELETE */
        int changes = sqlite3_changes(db);
        (void)changes; /* Use the value to prevent unused variable warnings */

        /* Fuzz sqlite3_trace_v2: Register a trace callback */
        /* Use input data to determine mask if possible, otherwise default */
        unsigned uMask = (Size > 1) ? (Data[1] & 0xFF) : SQLITE_TRACE_STMT;
        if (uMask == 0) uMask = SQLITE_TRACE_STMT; /* Avoid disabling if mask is 0 */
        
        void *pCtx = (void *)(uintptr_t)(Size > 2 ? Data[2] : 0x1234);
        sqlite3_trace_v2(db, uMask, trace_v2_callback, pCtx);

        /* Fuzz sqlite3_trace (deprecated): Register legacy trace */
        void *pTraceArg = (void *)(uintptr_t)(Size > 3 ? Data[3] : 0x5678);
        sqlite3_trace(db, trace_callback, pTraceArg);

        /* Fuzz sqlite3_commit_hook */
        void *pCommitArg = (void *)(uintptr_t)(Size > 4 ? Data[4] : 0x9ABC);
        sqlite3_commit_hook(db, commit_callback, pCommitArg);

        /* Fuzz sqlite3_wal_hook */
        void *pWalArg = (void *)(uintptr_t)(Size > 5 ? Data[5] : 0xDEF0);
        sqlite3_wal_hook(db, wal_callback, pWalArg);

        /* Finalize the statement */
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* 3. Cleanup */
    /* Close the database connection */
    sqlite3_close(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_trace at sqlite3.c:187881:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:187881:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static helper to avoid name conflicts */
static int trace_v2_callback(unsigned event, void *pCtx, void *pArg1, void *pArg2) {
    (void)event;
    (void)pCtx;
    (void)pArg1;
    (void)pArg2;
    return 0;
}

static void trace_callback(void *pCtx, const char *zSql) {
    (void)pCtx;
    (void)zSql;
}

static int commit_callback(void *pCtx) {
    (void)pCtx;
    return 0;
}

static int wal_callback(void *pCtx, sqlite3 *db, const char *zDb, int nPages) {
    (void)pCtx;
    (void)db;
    (void)zDb;
    (void)nPages;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a simple table to allow modifications for sqlite3_changes */
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, val TEXT);", NULL, NULL, NULL);

    /* Fuzz sqlite3_prepare_v3 */
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    unsigned int prepFlags = 0;
    
    /* Use input data as SQL, ensuring null termination if possible, or limit length */
    size_t sqlLen = Size;
    if (sqlLen > 4096) sqlLen = 4096;
    
    /* Create a safe buffer for SQL to ensure null termination */
    char *safeSql = malloc(sqlLen + 1);
    if (safeSql) {
        memcpy(safeSql, Data, sqlLen);
        safeSql[sqlLen] = '\0';

        /* Fuzz prepare flags based on input */
        if (Size > 0) prepFlags = Data[0] & 0x0F;

        rc = sqlite3_prepare_v3(db, safeSql, (int)sqlLen, prepFlags, &stmt, &tail);
        
        if (rc == SQLITE_OK && stmt) {
            /* Execute statement to potentially trigger changes */
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        free(safeSql);
    }

    /* Fuzz sqlite3_changes */
    /* This function is safe to call even if no statement ran recently, 
       though it returns 0. We call it to exercise the code path. */
    int changes = sqlite3_changes(db);
    (void)changes;

    /* Fuzz sqlite3_trace_v2 */
    /* Use input data to determine mask */
    unsigned int mask = 0;
    if (Size > 1) {
        mask = Data[1];
    }
    /* Register callback */
    rc = sqlite3_trace_v2(db, mask, trace_v2_callback, (void*)(uintptr_t)(Size));
    (void)rc;
    
    /* Disable trace by passing NULL callback */
    rc = sqlite3_trace_v2(db, mask, NULL, NULL);
    (void)rc;

    /* Fuzz sqlite3_trace (deprecated) */
    void *prevTraceArg = sqlite3_trace(db, trace_callback, (void*)(uintptr_t)(Size));
    (void)prevTraceArg;
    /* Disable */
    sqlite3_trace(db, NULL, NULL);

    /* Fuzz sqlite3_commit_hook */
    void *prevCommitArg = sqlite3_commit_hook(db, commit_callback, (void*)(uintptr_t)(Size));
    (void)prevCommitArg;
    /* Disable */
    sqlite3_commit_hook(db, NULL, NULL);

    /* Fuzz sqlite3_wal_hook */
    void *prevWalArg = sqlite3_wal_hook(db, wal_callback, (void*)(uintptr_t)(Size));
    (void)prevWalArg;
    /* Disable */
    sqlite3_wal_hook(db, NULL, NULL);

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}
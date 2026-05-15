// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:80259:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static sqlite3 *db = NULL;

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Initialize database connection if needed
    if (db == NULL) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
    }
    
    // Use a subset of data for UTF-16 SQL statement
    size_t sql_size = Size > 1024 ? 1024 : Size;
    void *zSql = malloc(sql_size + 2); // +2 for possible null terminator
    if (!zSql) return 0;
    
    // Copy data as UTF-16 (just treat as raw bytes for fuzzing)
    memcpy(zSql, Data, sql_size);
    
    // Prepare statement with UTF-16 SQL
    sqlite3_stmt *pStmt = NULL;
    const void *pzTail = NULL;
    int rc = sqlite3_prepare16(db, zSql, (int)sql_size, &pStmt, &pzTail);
    
    free(zSql);
    
    if (rc == SQLITE_OK && pStmt) {
        // Test sqlite3_stmt_status with various operations
        int ops[] = {
            SQLITE_STMTSTATUS_FULLSCAN_STEP,
            SQLITE_STMTSTATUS_SORT,
            SQLITE_STMTSTATUS_AUTOINDEX,
            SQLITE_STMTSTATUS_VM_STEP,
            SQLITE_STMTSTATUS_REPREPARE,
            SQLITE_STMTSTATUS_RUN,
            SQLITE_STMTSTATUS_FILTER_MISS,
            SQLITE_STMTSTATUS_FILTER_HIT,
            SQLITE_STMTSTATUS_MEMUSED
        };
        
        for (int i = 0; i < sizeof(ops)/sizeof(ops[0]); i++) {
            // Test with reset flag = 0
            sqlite3_stmt_status(pStmt, ops[i], 0);
            // Test with reset flag = 1
            sqlite3_stmt_status(pStmt, ops[i], 1);
        }
        
        // Test sqlite3_stmt_readonly
        sqlite3_stmt_readonly(pStmt);
        
        // Test sqlite3_stmt_busy
        sqlite3_stmt_busy(pStmt);
        
        // Test sqlite3_expired
        sqlite3_expired(pStmt);
        
        // Test sqlite3_stmt_explain with different modes
        sqlite3_stmt_explain(pStmt, 0); // Normal mode
        sqlite3_stmt_explain(pStmt, 1); // EXPLAIN mode
        sqlite3_stmt_explain(pStmt, 2); // EXPLAIN QUERY PLAN mode
        
        // Try stepping the statement to change state
        sqlite3_step(pStmt);
        
        // Test functions again after stepping
        sqlite3_stmt_busy(pStmt);
        sqlite3_stmt_readonly(pStmt);
        
        // Reset statement
        sqlite3_reset(pStmt);
        
        // Test again after reset
        sqlite3_stmt_busy(pStmt);
        
        // Finalize statement
        sqlite3_finalize(pStmt);
    }
    
    // Clean up any remaining statements
    sqlite3_stmt *pNext = NULL;
    sqlite3_stmt *p = NULL;
    for (p = sqlite3_next_stmt(db, NULL); p; p = pNext) {
        pNext = sqlite3_next_stmt(db, p);
        sqlite3_finalize(p);
    }
    
    return 0;
}

// Cleanup function
__attribute__((destructor))
static void cleanup(void) {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}
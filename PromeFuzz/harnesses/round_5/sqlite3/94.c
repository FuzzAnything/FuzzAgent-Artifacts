// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:173158:16 in sqlite3.h
// sqlite3_profile at sqlite3.c:173188:18 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:173218:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:172007:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int trace_callback(unsigned int traceEvent, void *context, void *p, void *x) {
    (void)traceEvent;
    (void)context;
    (void)p;
    (void)x;
    return 0;
}

static void profile_callback(void *context, const char *sql, sqlite3_uint64 time) {
    (void)context;
    (void)sql;
    (void)time;
}

static int dummy_progress(void *arg) {
    (void)arg;
    return 0;
}

static void dummy_update_callback(void *arg, int op, const char *dbname,
                                  const char *tablename, sqlite3_int64 rowid) {
    (void)arg;
    (void)op;
    (void)dbname;
    (void)tablename;
    (void)rowid;
}

static void rollback_callback(void *arg) {
    (void)arg;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *tail;
    char *err_msg = NULL;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Set up some callbacks to explore more states
    sqlite3_trace_v2(db, SQLITE_TRACE_STMT, trace_callback, NULL);
    sqlite3_profile(db, profile_callback, NULL);
    sqlite3_progress_handler(db, 100, dummy_progress, NULL);
    sqlite3_update_hook(db, dummy_update_callback, NULL);
    sqlite3_commit_hook(db, dummy_progress, NULL);
    sqlite3_rollback_hook(db, rollback_callback, NULL);
    
    // Create a table to work with
    rc = sqlite3_exec(db, 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data BLOB);"
        "CREATE TABLE IF NOT EXISTS test2(x INTEGER, y TEXT);"
        "CREATE VIRTUAL TABLE IF NOT EXISTS virt USING fts4(content);",
        NULL, NULL, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Use fuzzer data to set last insert rowid
    if (Size >= sizeof(sqlite3_int64)) {
        sqlite3_int64 rowid;
        memcpy(&rowid, Data, sizeof(sqlite3_int64));
        sqlite3_set_last_insert_rowid(db, rowid);
    }
    
    // Execute some SQL statements to generate changes
    const char *sqls[] = {
        "INSERT INTO test(data) VALUES(randomblob(10));",
        "UPDATE test SET data = randomblob(5) WHERE id = 1;",
        "DELETE FROM test WHERE id = 1;",
        "INSERT INTO test2 VALUES(1, 'hello');",
        "UPDATE test2 SET y = 'world' WHERE x = 1;",
        "DELETE FROM test2 WHERE x = 1;",
        "INSERT INTO virt VALUES('sample text');",
        NULL
    };
    
    for (int i = 0; sqls[i] != NULL; i++) {
        rc = sqlite3_prepare_v2(db, sqls[i], -1, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Call all target functions with the database connection
    int changes = sqlite3_changes(db);
    int total_changes = sqlite3_total_changes(db);
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    sqlite3_int64 total_changes64 = sqlite3_total_changes64(db);
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    
    // Use the values to avoid compiler optimizations
    volatile int dummy1 = changes;
    volatile int dummy2 = total_changes;
    volatile sqlite3_int64 dummy3 = last_rowid;
    volatile sqlite3_int64 dummy4 = total_changes64;
    volatile sqlite3_int64 dummy5 = changes64;
    (void)dummy1;
    (void)dummy2;
    (void)dummy3;
    (void)dummy4;
    (void)dummy5;
    
    // Try setting last insert rowid again with different values
    if (Size >= 2 * sizeof(sqlite3_int64)) {
        sqlite3_int64 rowid2;
        memcpy(&rowid2, Data + sizeof(sqlite3_int64), sizeof(sqlite3_int64));
        sqlite3_set_last_insert_rowid(db, rowid2);
        
        // Call functions again after setting
        sqlite3_last_insert_rowid(db);
        sqlite3_changes(db);
        sqlite3_changes64(db);
    }
    
    // Execute more statements to generate additional state changes
    rc = sqlite3_exec(db, 
        "BEGIN;"
        "INSERT INTO test(data) VALUES(randomblob(20));"
        "INSERT INTO test(data) VALUES(randomblob(30));"
        "COMMIT;",
        NULL, NULL, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Call functions again after batch operations
    sqlite3_total_changes(db);
    sqlite3_total_changes64(db);
    sqlite3_changes(db);
    sqlite3_changes64(db);
    
    // Create a savepoint and rollback to test changes behavior
    rc = sqlite3_exec(db, "SAVEPOINT sp1;", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_exec(db, "INSERT INTO test(data) VALUES(randomblob(40));", 
                         NULL, NULL, NULL);
        sqlite3_changes(db);
        sqlite3_changes64(db);
        sqlite3_exec(db, "ROLLBACK TO sp1;", NULL, NULL, NULL);
        sqlite3_changes(db);
        sqlite3_changes64(db);
        sqlite3_exec(db, "RELEASE sp1;", NULL, NULL, NULL);
    }
    
    // Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    
    return 0;
}
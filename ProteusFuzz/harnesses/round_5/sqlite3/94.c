// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:186766:26 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:186782:26 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 change tracking and status APIs.
 * Target APIs:
 *   sqlite3_total_changes
 *   sqlite3_changes
 *   sqlite3_db_status64
 *   sqlite3_changes64
 *   sqlite3_total_changes64
 *   sqlite3_errcode
 */

static int fuzz_sqlite3_changes(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int changes;
    int total_changes;
    sqlite3_int64 changes64;
    sqlite3_int64 total_changes64;
    sqlite3_int64 cur_val, high_val;
    int err_code;

    if (Size == 0) {
        return 0;
    }

    /* Open database in memory to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a simple table to perform modifications */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement for modifications */
    const char *insert_sql = "INSERT INTO test_table (data) VALUES (X'00');";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Perform some modifications based on input data to trigger different states */
    int iterations = (Size > 100) ? 100 : (int)Size;
    for (int i = 0; i < iterations; i++) {
        rc = sqlite3_reset(stmt);
        if (rc != SQLITE_OK) break;
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE && rc != SQLITE_ROW) break;
    }

    /* Call target APIs to fuzz their behavior */
    changes = sqlite3_changes(db);
    total_changes = sqlite3_total_changes(db);
    changes64 = sqlite3_changes64(db);
    total_changes64 = sqlite3_total_changes64(db);

    /* Fuzz sqlite3_db_status64 with various operation codes */
    /* Note: Removed SQLITE_DBSTATUS_LOOKASIDE_MISS_ALLOC as it may not be defined in all builds */
    int op_codes[] = {
        SQLITE_DBSTATUS_LOOKASIDE_USED,
        SQLITE_DBSTATUS_LOOKASIDE_HIT,
        SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
        SQLITE_DBSTATUS_CACHE_USED,
        SQLITE_DBSTATUS_CACHE_HIT,
        SQLITE_DBSTATUS_CACHE_MISS,
        SQLITE_DBSTATUS_CACHE_WRITE,
        SQLITE_DBSTATUS_SCHEMA_USED,
        SQLITE_DBSTATUS_STMT_USED,
        SQLITE_DBSTATUS_CACHE_USED_SHARED,
        SQLITE_DBSTATUS_DEFERRED_FKS
    };
    int num_ops = sizeof(op_codes) / sizeof(op_codes[0]);
    for (int i = 0; i < num_ops; i++) {
        rc = sqlite3_db_status64(db, op_codes[i], &cur_val, &high_val, 0);
        (void)rc;
        (void)cur_val;
        (void)high_val;
    }

    /* Try resetting high-water marks */
    for (int i = 0; i < num_ops; i++) {
        rc = sqlite3_db_status64(db, op_codes[i], &cur_val, &high_val, 1);
        (void)rc;
        (void)cur_val;
        (void)high_val;
    }

    /* Trigger an error to test sqlite3_errcode */
    rc = sqlite3_exec(db, "INVALID SQL STATEMENT;", NULL, NULL, NULL);
    err_code = sqlite3_errcode(db);
    (void)err_code;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    /* Call the fuzzing logic */
    fuzz_sqlite3_changes(Data, Size);
    return 0;
}
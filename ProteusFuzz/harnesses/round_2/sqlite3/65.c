// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:94349:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:94349:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
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

static int create_db_and_prepare(sqlite3 **db_out, sqlite3_stmt **stmt_out, const char *sql) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return -1;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }

    *db_out = db;
    *stmt_out = stmt;
    return 0;
}

static void cleanup_db(sqlite3 *db, sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Use the input data to generate a simple SQL query
    // We limit the size to prevent huge allocations and ensure null termination
    size_t sql_len = Size;
    if (sql_len > 1000) sql_len = 1000;

    char *sql = (char *)malloc(sql_len + 1);
    if (!sql) return 0;
    memcpy(sql, Data, sql_len);
    sql[sql_len] = '\0';

    // Try to prepare a statement
    if (create_db_and_prepare(&db, &stmt, sql) != 0) {
        // If preparation fails, we still want to test the NULL pointer handling
        // of the target functions.
        free(sql);
        
        // Test with NULL pointer
        (void)sqlite3_stmt_status(NULL, 0, 0);
        (void)sqlite3_stmt_busy(NULL);
        (void)sqlite3_expired(NULL);
        (void)sqlite3_stmt_readonly(NULL);
        (void)sqlite3_stmt_isexplain(NULL);
        (void)sqlite3_reset(NULL);
        
        return 0;
    }

    // Execute the statement to potentially change its state (busy, expired, etc.)
    // We step a few times to explore different states
    int step_count = 0;
    while (step_count < 10) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
            step_count++;
        } else {
            break;
        }
    }

    // Fuzz sqlite3_stmt_status
    // op codes: 0 to 20 (common ones are 0-15, but we try a range)
    int op = Data[0] % 30; 
    int reset_flag = (Size > 1) ? (Data[1] % 2) : 0;
    (void)sqlite3_stmt_status(stmt, op, reset_flag);

    // Fuzz sqlite3_stmt_busy
    (void)sqlite3_stmt_busy(stmt);

    // Fuzz sqlite3_expired
    (void)sqlite3_expired(stmt);

    // Fuzz sqlite3_stmt_readonly
    (void)sqlite3_stmt_readonly(stmt);

    // Fuzz sqlite3_stmt_isexplain
    (void)sqlite3_stmt_isexplain(stmt);

    // Fuzz sqlite3_reset
    (void)sqlite3_reset(stmt);

    // Try to step again after reset to ensure state changes are covered
    (void)sqlite3_step(stmt);

    // Cleanup
    cleanup_db(db, stmt);
    free(sql);

    return 0;
}
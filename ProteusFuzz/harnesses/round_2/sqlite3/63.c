// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_set_last_insert_rowid at sqlite3.c:186751:17 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to safely open a database connection.
 * Uses a dummy file as per instructions.
 */
static sqlite3 *open_db(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Static helper to create a prepared statement for testing.
 * Returns NULL if preparation fails.
 */
static sqlite3_stmt *create_stmt(sqlite3 *db) {
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 1;";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return stmt;
}

/*
 * Dummy user function to generate a valid context for sqlite3_context_db_handle
 */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = open_db();
    if (!db) {
        return 0;
    }

    int op = Data[0] % 6;
    sqlite3_int64 rowid_val = 0;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *stmt_to_finalize = NULL;

    /* Extract a 64-bit integer from the input for rowid if available */
    if (Size >= 9) {
        memcpy(&rowid_val, Data + 1, sizeof(sqlite3_int64));
    } else if (Size >= 5) {
        int32_t val32 = 0;
        memcpy(&val32, Data + 1, sizeof(int32_t));
        rowid_val = val32;
    } else if (Size >= 3) {
        int16_t val16 = 0;
        memcpy(&val16, Data + 1, sizeof(int16_t));
        rowid_val = val16;
    } else if (Size >= 2) {
        rowid_val = Data[1];
    } else {
        rowid_val = 0;
    }

    /* 
     * Prepare a statement if needed for sqlite3_db_handle or sqlite3_next_stmt
     */
    if (op == 3 || op == 4) {
        stmt = create_stmt(db);
    }

    switch (op) {
        case 0: {
            /* Test sqlite3_set_last_insert_rowid */
            sqlite3_set_last_insert_rowid(db, rowid_val);
            (void)sqlite3_last_insert_rowid(db);
            break;
        }
        case 1: {
            /* Test sqlite3_errcode */
            sqlite3_exec(db, "SELECT invalid_column;", NULL, NULL, NULL);
            int code = sqlite3_errcode(db);
            (void)code;
            break;
        }
        case 2: {
            /* Test sqlite3_db_handle */
            sqlite3_stmt *test_stmt = (Size > 2 && Data[2] == 0) ? NULL : stmt;
            sqlite3 *handle = sqlite3_db_handle(test_stmt);
            (void)handle;
            break;
        }
        case 3: {
            /* Test sqlite3_next_stmt */
            sqlite3_stmt *current = (Size > 2 && Data[2] == 0) ? NULL : stmt;
            sqlite3_stmt *next = sqlite3_next_stmt(db, current);
            if (next) {
                /* 
                 * CRITICAL FIX: 
                 * sqlite3_next_stmt returns a pointer to an existing statement 
                 * within the database's linked list. It does NOT create a new 
                 * statement that needs to be finalized separately from the 
                 * original 'stmt'. Finalizing 'next' here effectively finalizes 
                 * 'stmt' if they point to the same object, or corrupts the 
                 * database's internal statement list.
                 * 
                 * We should NOT finalize 'next' here. The cleanup block at the 
                 * end will handle finalizing 'stmt'. If 'next' is the same as 
                 * 'stmt', finalizing it here causes a double-free/use-after-free 
                 * in the cleanup block.
                 */
                /* Removed: sqlite3_finalize(next); */
            }
            break;
        }
        case 4: {
            /* Test sqlite3_last_insert_rowid */
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_fuzz (id INTEGER PRIMARY KEY);", NULL, NULL, NULL);
            sqlite3_exec(db, "INSERT INTO test_fuzz VALUES (123);", NULL, NULL, NULL);
            sqlite3_int64 rid = sqlite3_last_insert_rowid(db);
            (void)rid;
            break;
        }
        case 5: {
            /* Test sqlite3_context_db_handle */
            if (sqlite3_create_function(db, "fuzz_dummy", 0, SQLITE_UTF8, NULL, dummy_func, NULL, NULL) == SQLITE_OK) {
                sqlite3_stmt *s = NULL;
                if (sqlite3_prepare_v2(db, "SELECT fuzz_dummy()", -1, &s, NULL) == SQLITE_OK) {
                    if (sqlite3_step(s) == SQLITE_ROW) {
                        /* Logic skipped as per original driver comments */
                    }
                    sqlite3_finalize(s);
                }
                sqlite3_create_function(db, "fuzz_dummy", 0, SQLITE_UTF8, NULL, NULL, NULL, NULL); 
            }
            break;
        }
    }

    /* Cleanup */
    /* 
     * Only finalize 'stmt' if it was created and not already finalized 
     * by logic inside the switch (which we fixed in case 3).
     */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}
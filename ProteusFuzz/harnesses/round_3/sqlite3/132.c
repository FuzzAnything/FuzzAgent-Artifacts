// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:186782:26 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:186766:26 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fuzz driver for sqlite3 error reporting and change tracking APIs.
 * Target APIs:
 * 1. sqlite3_errcode
 * 2. sqlite3_total_changes64
 * 3. sqlite3_changes64
 * 4. sqlite3_db_status64
 * 5. sqlite3_total_changes
 * 6. sqlite3_changes
 */

/* Helper to create a temporary database file for fuzzing */
static int create_dummy_db(sqlite3 **db_out) {
    const char *filename = "./dummy_file";
    int rc = sqlite3_open(filename, db_out);
    if (rc != SQLITE_OK) {
        return -1;
    }
    return 0;
}

/* Helper to close and cleanup database */
static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/* Helper to execute a random SQL statement to trigger state changes */
static void execute_random_sql(sqlite3 *db, const uint8_t *data, size_t size) {
    if (!db || !data || size == 0) return;

    /* Limit SQL size to prevent excessive memory usage */
    size_t sql_len = (size > 1000) ? 1000 : size;
    char *sql = malloc(sql_len + 1);
    if (!sql) return;

    memcpy(sql, data, sql_len);
    sql[sql_len] = '\0';

    /* Ensure the SQL is valid UTF-8 by replacing non-printable chars */
    for (size_t i = 0; i < sql_len; i++) {
        if (sql[i] < 32 || sql[i] > 126) {
            sql[i] = ' ';
        }
    }

    sqlite3_exec(db, sql, NULL, NULL, NULL);
    free(sql);
}

/* Fuzzing entry point */
int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    sqlite3_int64 val64;
    int val32;
    sqlite3_int64 current_val, high_water_mark;

    /* 1. Prepare environment: Open database connection */
    if (create_dummy_db(&db) != 0) {
        return 0; /* Skip if we cannot open database */
    }

    /* Execute some SQL to potentially generate errors or changes */
    if (Size > 0) {
        execute_random_sql(db, Data, Size);
    }

    /* 2. Invoke target functions to explore program states */

    /* Test sqlite3_errcode */
    rc = sqlite3_errcode(db);
    (void)rc; /* Suppress unused variable warning */

    /* Test sqlite3_total_changes64 */
    val64 = sqlite3_total_changes64(db);
    (void)val64;

    /* Test sqlite3_changes64 */
    val64 = sqlite3_changes64(db);
    (void)val64;

    /* Test sqlite3_db_status64 with various opcodes */
    /* SQLITE_DBSTATUS_LOOKASIDE_USED is a common safe opcode */
    rc = sqlite3_db_status64(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &current_val, &high_water_mark, 0);
    (void)rc;

    /* Try resetting the high water mark */
    rc = sqlite3_db_status64(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &current_val, &high_water_mark, 1);
    (void)rc;

    /* Try with an invalid opcode to test error handling */
    rc = sqlite3_db_status64(db, -1, &current_val, &high_water_mark, 0);
    (void)rc;

    /* Test sqlite3_total_changes */
    val32 = sqlite3_total_changes(db);
    (void)val32;

    /* Test sqlite3_changes */
    val32 = sqlite3_changes(db);
    (void)val32;

    /* 3. Cleanup */
    cleanup_db(db);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API sequence:
 * sqlite3_open -> sqlite3_busy_timeout -> sqlite3_step -> sqlite3_column_int -> sqlite3_finalize -> sqlite3_sleep -> sqlite3_close
 */

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int timeout_ms = 0;
    int sleep_ms = 0;
    int col_idx = 0;
    int col_val = 0;

    /* 1. Prepare the environment */
    /* Ensure we have enough data to avoid out-of-bounds reads */
    if (Size < 3) {
        return 0;
    }

    /* Use fuzzing data to configure parameters safely */
    timeout_ms = (int)Data[0];
    sleep_ms = (int)Data[1] % 100; /* Keep sleep short to avoid TLE */
    col_idx = (int)Data[2] % 10;   /* Column index 0-9 */

    /* Use a memory database to avoid file system issues */
    const char *db_filename = ":memory:";

    /* 2. Invoke sqlite3_open */
    rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* If open fails, we cannot proceed with the sequence safely. */
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }

    /* 3. Invoke sqlite3_busy_timeout */
    sqlite3_busy_timeout(db, timeout_ms);

    /* 4. Prepare a statement for sqlite3_step */
    /* We need a valid statement to step through. */
    const char *sql = "SELECT 12345 AS val;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* 5. Invoke sqlite3_step */
    rc = sqlite3_step(stmt);

    /* 6. Invoke sqlite3_column_int */
    /* Call regardless of step result to exercise code paths, 
       though spec recommends calling only on SQLITE_ROW. */
    col_val = sqlite3_column_int(stmt, col_idx);
    (void)col_val; /* Suppress unused variable warning */

    /* 7. Invoke sqlite3_finalize */
    sqlite3_finalize(stmt);
    stmt = NULL;

    /* 8. Invoke sqlite3_sleep */
    sqlite3_sleep(sleep_ms);

    /* 9. Invoke sqlite3_close */
    sqlite3_close(db);
    db = NULL;

    return 0;
}
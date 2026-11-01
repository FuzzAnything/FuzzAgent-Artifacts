// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target sequence:
 * 1. sqlite3_open
 * 2. sqlite3_busy_timeout
 * 3. sqlite3_step (requires prepare, so we add prepare/reset logic)
 * 4. sqlite3_column_int
 * 5. sqlite3_finalize
 * 6. sqlite3_sleep
 * 7. sqlite3_close
 */

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = NULL;
    int rc;
    int timeout_ms = 0;
    int sleep_ms = 0;
    int col_idx = 0;

    if (Size == 0) {
        return 0;
    }

    /* Use input data to derive parameters */
    if (Size >= 1) {
        timeout_ms = (int)Data[0];
    }
    if (Size >= 2) {
        sleep_ms = (int)Data[1];
    }
    if (Size >= 3) {
        col_idx = (int)Data[2];
    }

    /* Create a dummy SQL statement if input is large enough */
    const char *sql_template = "SELECT 1;";
    if (Size >= 10) {
        /* Construct a potentially malformed SQL to trigger various states */
        char *sql_buf = (char *)malloc(Size + 1);
        if (sql_buf) {
            memcpy(sql_buf, Data, Size);
            sql_buf[Size] = '\0';
            sql = sql_buf;
        } else {
            sql = sql_template;
        }
    } else {
        sql = sql_template;
    }

    /* 1. sqlite3_open */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If open fails, we can't proceed with other DB operations */
        if (sql != sql_template) free((void *)sql);
        return 0;
    }

    /* 2. sqlite3_busy_timeout */
    sqlite3_busy_timeout(db, timeout_ms);

    /* Prepare a statement to enable sqlite3_step */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        /* If prepare fails, we skip step/column_int/finalize */
        sqlite3_close(db);
        if (sql != sql_template) free((void *)sql);
        return 0;
    }

    /* 3. sqlite3_step */
    rc = sqlite3_step(stmt);

    /* 4. sqlite3_column_int */
    /* Only call if step returned SQLITE_ROW */
    if (rc == SQLITE_ROW) {
        (void)sqlite3_column_int(stmt, col_idx);
    }

    /* 5. sqlite3_finalize */
    sqlite3_finalize(stmt);

    /* 6. sqlite3_sleep */
    sqlite3_sleep(sleep_ms);

    /* 7. sqlite3_close */
    sqlite3_close(db);

    if (sql != sql_template) {
        free((void *)sql);
    }

    return 0;
}
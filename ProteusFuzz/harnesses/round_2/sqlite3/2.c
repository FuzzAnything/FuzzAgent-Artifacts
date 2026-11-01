// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
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
 * Fuzz driver for SQLite3 API functions.
 * Target sequence:
 * sqlite3_prepare_v2 -> sqlite3_errmsg -> sqlite3_step -> sqlite3_step ->
 * sqlite3_column_count -> sqlite3_column_type -> sqlite3_column_name ->
 * sqlite3_column_text -> sqlite3_column_text -> sqlite3_column_text ->
 * sqlite3_column_bytes -> sqlite3_finalize
 */

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;

    /* Ensure we have enough data to work with */
    if (Size == 0) {
        return 0;
    }

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a dummy table to ensure SELECT statements have a target */
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, init_sql, NULL, NULL, NULL);

    /* Use input data as SQL query, ensuring it is null-terminated */
    char *sql = (char *)malloc(Size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';

    /* 1. sqlite3_prepare_v2 */
    rc = sqlite3_prepare_v2(db, sql, Size, &stmt, &tail);

    /* 2. sqlite3_errmsg (check for errors from prepare) */
    if (rc != SQLITE_OK) {
        const char *err_msg = sqlite3_errmsg(db);
        (void)err_msg; /* Suppress unused variable warning */
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free(sql);
        return 0;
    }

    /* 3. sqlite3_step (first step) */
    rc = sqlite3_step(stmt);

    /* 4. sqlite3_step (second step) */
    rc = sqlite3_step(stmt);

    /* 5. sqlite3_column_count */
    int col_count = sqlite3_column_count(stmt);
    if (col_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free(sql);
        return 0;
    }

    /* 6. sqlite3_column_type */
    int col_type = sqlite3_column_type(stmt, 0);
    (void)col_type;

    /* 7. sqlite3_column_name */
    const char *col_name = sqlite3_column_name(stmt, 0);
    (void)col_name;

    /* 8, 9, 10. sqlite3_column_text (three times) */
    const unsigned char *text1 = sqlite3_column_text(stmt, 0);
    (void)text1;

    const unsigned char *text2 = sqlite3_column_text(stmt, 1);
    (void)text2;

    const unsigned char *text3 = sqlite3_column_text(stmt, 2);
    (void)text3;

    /* 11. sqlite3_column_bytes */
    int bytes = sqlite3_column_bytes(stmt, 0);
    (void)bytes;

    /* 12. sqlite3_finalize */
    sqlite3_finalize(stmt);

    /* Cleanup */
    sqlite3_close(db);
    free(sql);

    return 0;
}
// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target sequence:
 * sqlite3_prepare_v2 -> sqlite3_errmsg -> sqlite3_step -> sqlite3_step
 * -> sqlite3_column_count -> sqlite3_column_type -> sqlite3_column_name
 * -> sqlite3_column_text -> sqlite3_column_text -> sqlite3_column_text
 * -> sqlite3_column_bytes -> sqlite3_finalize
 */

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *tail = NULL;
    const char *sql;

    /* 1. Prepare the environment */
    if (Size == 0) {
        return 0;
    }

    /* Use the input data as the SQL string, ensuring it is null-terminated */
    char *sql_buf = (char *)malloc(Size + 1);
    if (!sql_buf) {
        return 0;
    }
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';
    sql = sql_buf;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        free(sql_buf);
        return 0;
    }

    /* 2. Invoke sqlite3_prepare_v2 */
    rc = sqlite3_prepare_v2(db, sql, (int)Size, &stmt, &tail);

    /* 3. Invoke sqlite3_errmsg (always safe to call) */
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    /* If preparation failed, stmt might be NULL. Finalize is safe on NULL, but we must not use stmt. */
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free(sql_buf);
        return 0;
    }

    /* At this point, stmt is valid (non-NULL) because prepare_v2 succeeded. */

    /* 4. Invoke sqlite3_step (first time) */
    rc = sqlite3_step(stmt);

    /* 5. Invoke sqlite3_step (second time) */
    rc = sqlite3_step(stmt);

    /* 6. Invoke sqlite3_column_count */
    int col_count = sqlite3_column_count(stmt);
    (void)col_count;

    /* 7. Invoke sqlite3_column_type */
    /* Check if there are columns before accessing specific column index to avoid undefined behavior 
       though sqlite3_column_type usually handles out-of-bounds gracefully, being safe is better. */
    if (col_count > 0) {
        int col_type = sqlite3_column_type(stmt, 0);
        (void)col_type;
    }

    /* 8. Invoke sqlite3_column_name */
    /* Only call if there are columns to avoid potential issues with invalid index, 
       though the API is robust, checking count is safer for fuzzing stability. */
    if (col_count > 0) {
        const char *col_name = sqlite3_column_name(stmt, 0);
        (void)col_name;
    }

    /* 9. Invoke sqlite3_column_text (three times) */
    /* We try column 0, 1, and 2. Check bounds to ensure valid access. */
    if (col_count > 0) {
        const unsigned char *text1 = sqlite3_column_text(stmt, 0);
        (void)text1;
    }
    if (col_count > 1) {
        const unsigned char *text2 = sqlite3_column_text(stmt, 1);
        (void)text2;
    }
    if (col_count > 2) {
        const unsigned char *text3 = sqlite3_column_text(stmt, 2);
        (void)text3;
    }

    /* 10. Invoke sqlite3_column_bytes */
    if (col_count > 0) {
        int bytes = sqlite3_column_bytes(stmt, 0);
        (void)bytes;
    }

    /* 11. Cleanup: sqlite3_finalize */
    sqlite3_finalize(stmt);

    /* 12. Cleanup: Close database */
    sqlite3_close(db);

    free(sql_buf);

    return 0;
}
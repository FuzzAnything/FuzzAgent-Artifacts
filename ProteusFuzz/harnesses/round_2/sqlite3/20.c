// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    char *sql_text = NULL;

    /* Minimum size check to avoid trivial inputs */
    if (Size < 1) {
        return 0;
    }

    /* Initialize database connection in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a new dynamic string object */
    str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Step 1: sqlite3_str_appendf - append formatted text */
    if (Size >= 2) {
        /* Create a safe buffer for format string, ensuring it's null-terminated */
        char *fmt_buf = malloc(Size + 1);
        if (fmt_buf) {
            memcpy(fmt_buf, Data, Size);
            fmt_buf[Size] = '\0';
            
            /* Try to append formatted string */
            sqlite3_str_appendf(str, "%s", fmt_buf);
            free(fmt_buf);
        }
    }

    /* Step 2: sqlite3_str_append - append N bytes from input */
    size_t append_len = (Size > 100) ? 100 : Size;
    if (append_len > 0) {
        sqlite3_str_append(str, (const char *)Data, (int)append_len);
    }

    /* Step 3: sqlite3_str_append - append more data (second call as required) */
    size_t append_len2 = (Size > 50) ? 50 : Size;
    if (append_len2 > 0) {
        size_t offset = (Size > append_len2) ? append_len2 : 0;
        if (offset < Size) {
            sqlite3_str_append(str, (const char *)(Data + offset), (int)append_len2);
        }
    }

    /* Step 4: sqlite3_prepare_v2 - compile SQL string */
    /* Get the constructed string value to use as SQL */
    sql_text = sqlite3_str_value(str);
    
    if (sql_text != NULL) {
        int sql_len = (int)strlen(sql_text);
        rc = sqlite3_prepare_v2(db, sql_text, sql_len, &stmt, &tail);
        
        /* If preparation failed, stmt might be NULL, but finalize handles NULL safely */
        if (rc != SQLITE_OK && stmt != NULL) {
            /* stmt is valid but preparation had an error, we still finalize */
        }
    } else {
        /* If string is empty or invalid, try a default SQL statement */
        const char *default_sql = "SELECT 1;";
        rc = sqlite3_prepare_v2(db, default_sql, (int)strlen(default_sql), &stmt, &tail);
    }

    /* Step 5: sqlite3_str_value - retrieve string value (already called above) */
    /* The string is finalized internally by sqlite3_str_value, but we must clean up the object */

    /* Step 6: sqlite3_finalize - destroy prepared statement */
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }

    /* Cleanup: destroy the string object and free its memory */
    if (str != NULL) {
        /* sqlite3_str_finish frees the string buffer and the object itself */
        sqlite3_str_finish(str);
    }

    /* Close database connection */
    sqlite3_close(db);

    return 0;
}
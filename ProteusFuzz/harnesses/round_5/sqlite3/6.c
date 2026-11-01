// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *str = NULL;
    const char *sql = "SELECT 1 as col1, 'test' as col2";
    int rc;
    
    /* Allocate buffers on heap to avoid stack-use-after-scope */
    char pattern_buf[256] = "a%";
    char text_buf[256] = "abc";
    unsigned int escapeChar = 0;
    const char *pattern = pattern_buf;
    const char *text = text_buf;

    /* Use input data if available to vary pattern/text slightly, ensuring safety */
    if (Size >= 4) {
        size_t len = (Size < 255) ? Size : 255;
        memcpy(pattern_buf, Data, len);
        pattern_buf[len] = '\0';
        memcpy(text_buf, Data, len);
        text_buf[len] = '\0';
        pattern = pattern_buf;
        text = text_buf;
    }

    /* 1. Prepare the environment */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* 2. Invoke target functions in the specified order */

    /* sqlite3_strlike */
    (void)sqlite3_strlike(pattern, text, escapeChar);

    /* sqlite3_column_count */
    int colCount = sqlite3_column_count(stmt);

    /* sqlite3_column_name (first call) */
    const char *name1 = NULL;
    if (colCount > 0) {
        name1 = sqlite3_column_name(stmt, 0);
    }

    /* sqlite3_column_name (second call) */
    const char *name2 = NULL;
    if (colCount > 1) {
        name2 = sqlite3_column_name(stmt, 1);
    }

    /* sqlite3_stricmp */
    if (name1 && name2) {
        (void)sqlite3_stricmp(name1, name2);
    } else if (name1) {
        (void)sqlite3_stricmp(name1, "dummy");
    } else if (name2) {
        (void)sqlite3_stricmp("dummy", name2);
    }

    /* sqlite3_column_type */
    if (colCount > 0) {
        (void)sqlite3_column_type(stmt, 0);
    }

    /* sqlite3_str_appendf (first call) */
    if (name1) {
        sqlite3_str_appendf(str, "Column 1: %s", name1);
    } else {
        sqlite3_str_appendf(str, "Column 1: NULL");
    }

    /* sqlite3_str_appendf (second call) */
    if (name2) {
        sqlite3_str_appendf(str, " | Column 2: %s", name2);
    } else {
        sqlite3_str_appendf(str, " | Column 2: NULL");
    }

    /* sqlite3_finalize */
    rc = sqlite3_finalize(stmt);
    /* stmt is now invalid */

    /* sqlite3_str_appendf (third call) */
    sqlite3_str_appendf(str, " | Finalized: %s", (rc == SQLITE_OK) ? "OK" : "ERR");

    /* sqlite3_str_finish */
    char *result = sqlite3_str_finish(str);
    if (result) {
        sqlite3_free(result);
    }

    /* 3. Cleanup */
    sqlite3_close(db);

    return 0;
}
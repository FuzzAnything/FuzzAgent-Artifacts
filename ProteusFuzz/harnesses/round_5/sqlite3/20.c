// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *tail = NULL;
    char *result_str = NULL;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a dynamic string object */
    str = sqlite3_str_new(db);
    if (!str) {
        sqlite3_close(db);
        return 0;
    }

    /* 1. sqlite3_str_appendf */
    if (Size > 0 && Data[0] == 0) {
        sqlite3_str_appendf(str, "%.*s", (int)Size, (const char *)Data);
    } else {
        sqlite3_str_appendf(str, "test");
    }

    /* 2. sqlite3_str_append (first call) */
    int append_len1 = (Size > 100) ? 100 : (int)Size;
    if (append_len1 < 0) append_len1 = 0;
    sqlite3_str_append(str, (const char *)Data, append_len1);

    /* 3. sqlite3_str_append (second call) */
    int append_len2 = (Size > 50) ? 50 : (int)Size;
    if (append_len2 < 0) append_len2 = 0;
    sqlite3_str_append(str, (const char *)Data, append_len2);

    /* 4. sqlite3_prepare_v2 */
    result_str = sqlite3_str_value(str);
    if (result_str) {
        int sql_len = (int)strlen(result_str);
        if (sql_len > 0) {
            rc = sqlite3_prepare_v2(db, result_str, sql_len, &stmt, &tail);
        } else {
            rc = sqlite3_prepare_v2(db, "", 0, &stmt, &tail);
        }
    } else {
        rc = sqlite3_prepare_v2(db, "SELECT 1;", 9, &stmt, &tail);
    }

    /* 6. sqlite3_finalize */
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Cleanup: MUST call sqlite3_str_finish to free the internal buffer */
    if (str) {
        sqlite3_str_finish(str);
        str = NULL;
    }

    sqlite3_close(db);

    return 0;
}
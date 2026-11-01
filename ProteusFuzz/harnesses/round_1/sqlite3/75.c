// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free_table at sqlite3.c:156640:17 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:94395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:156571:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:156640:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

static char *test_vmprintf(const char *fmt, int val, ...) {
    va_list args;
    va_start(args, val);
    char *res = sqlite3_vmprintf(fmt, args);
    va_end(args);
    return res;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *input_str = (const char *)Data;
    size_t input_len = Size;

    // 1. Test sqlite3_mprintf
    char safe_format[256];
    size_t fmt_len = (input_len < 255) ? input_len : 255;
    memcpy(safe_format, input_str, fmt_len);
    safe_format[fmt_len] = '\0';

    char *result_mprintf = sqlite3_mprintf(safe_format, 12345);
    if (result_mprintf != NULL) {
        sqlite3_free(result_mprintf);
    }

    // 2. Test sqlite3_vmprintf
    char *result_vmprintf = test_vmprintf(safe_format, 12345);
    if (result_vmprintf != NULL) {
        sqlite3_free(result_vmprintf);
    }

    // 3. Test sqlite3_realloc
    void *ptr = sqlite3_realloc(NULL, 100);
    if (ptr != NULL) {
        void *ptr_new = sqlite3_realloc(ptr, (int)(input_len % 200));
        if (ptr_new != NULL) {
            void *ptr_neg = sqlite3_realloc(ptr_new, -1);
            if (ptr_neg != NULL) {
                sqlite3_free(ptr_neg);
            }
        } else {
            sqlite3_free(ptr);
        }
    }

    // 4. Test sqlite3_expanded_sql and sqlite3_free_table
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char **table_result = NULL;

    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT);";
        sqlite3_exec(db, create_sql, NULL, NULL, NULL);

        size_t sql_len = (input_len < 1000) ? input_len : 1000;
        char sql[1001];
        memcpy(sql, input_str, sql_len);
        sql[sql_len] = '\0';

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            char *expanded = sqlite3_expanded_sql(stmt);
            if (expanded != NULL) {
                sqlite3_free(expanded);
            }

            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }

        const char *select_sql = "SELECT * FROM test;";
        char *errmsg = NULL;
        int pnRow = 0;
        int pnColumn = 0;
        int rc = sqlite3_get_table(db, select_sql, &table_result, &pnRow, &pnColumn, &errmsg);

        if (rc == SQLITE_OK && table_result != NULL) {
            sqlite3_free_table(table_result);
            table_result = NULL;
        } else if (rc != SQLITE_OK && errmsg != NULL) {
            sqlite3_free(errmsg);
        }

        sqlite3_close(db);
    }

    // 5. Test sqlite3_free with NULL
    sqlite3_free(NULL);

    // 6. Test sqlite3_free_table with NULL
    sqlite3_free_table(NULL);

    return 0;
}
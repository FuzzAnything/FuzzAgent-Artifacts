// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Limit size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }

    // Open an in-memory database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }

    // Create a dynamic string object
    sqlite3_str *str = sqlite3_str_new(db);
    if (!str) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a dummy format string and data for appendf
    // Use a small portion of fuzzer data for format string
    size_t fmt_len = Size > 0 ? (Size % 100) + 1 : 1;
    char *fmt = (char*)malloc(fmt_len + 1);
    if (!fmt) {
        sqlite3_str_finish(str);
        sqlite3_close(db);
        return 0;
    }
    for (size_t i = 0; i < fmt_len && i < Size; i++) {
        fmt[i] = Data[i] ? Data[i] : 'A'; // Replace null bytes
    }
    fmt[fmt_len] = '\0';

    // 1. Call sqlite3_str_appendf
    // Use a simple format string with string argument
    char *append_data = (char*)malloc(Size + 1);
    if (!append_data) {
        free(fmt);
        sqlite3_str_finish(str);
        sqlite3_close(db);
        return 0;
    }
    for (size_t i = 0; i < Size; i++) {
        append_data[i] = Data[i] ? Data[i] : 'B'; // Replace null bytes
    }
    append_data[Size] = '\0';
    
    sqlite3_str_appendf(str, "%s: %s", fmt, append_data);
    
    // 2. First call to sqlite3_str_append
    // Use first half of data
    size_t first_half = Size / 2;
    if (first_half > 0) {
        char *first_part = (char*)malloc(first_half);
        if (first_part) {
            for (size_t i = 0; i < first_half; i++) {
                first_part[i] = Data[i] ? Data[i] : 'C';
            }
            sqlite3_str_append(str, first_part, (int)first_half);
            free(first_part);
        }
    }

    // 3. Second call to sqlite3_str_append
    // Use second half of data
    size_t second_half = Size - first_half;
    if (second_half > 0) {
        char *second_part = (char*)malloc(second_half);
        if (second_part) {
            for (size_t i = 0; i < second_half; i++) {
                second_part[i] = Data[first_half + i] ? Data[first_half + i] : 'D';
            }
            sqlite3_str_append(str, second_part, (int)second_half);
            free(second_part);
        }
    }

    // Create SQL statement from string builder content
    char *sql = sqlite3_str_value(str);
    if (!sql) {
        sql = "SELECT 1"; // Fallback SQL
    }

    // 4. Call sqlite3_prepare_v2
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    
    // 5. Call sqlite3_str_value again
    char *str_value = sqlite3_str_value(str);
    (void)str_value; // Use value to avoid unused variable warning
    
    // 6. Call sqlite3_finalize
    if (stmt) {
        sqlite3_finalize(stmt);
    }

    // Cleanup
    free(fmt);
    free(append_data);
    sqlite3_str_finish(str);
    sqlite3_close(db);
    
    return 0;
}
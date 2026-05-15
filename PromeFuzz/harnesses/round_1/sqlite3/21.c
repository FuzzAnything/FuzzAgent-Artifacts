// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Create a temporary file for database operations
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize database connection
    sqlite3* db = NULL;
    int rc = sqlite3_open_v2(dummy_file, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        remove(dummy_file);
        return 0;
    }

    // Create a dynamic string object
    sqlite3_str* str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        remove(dummy_file);
        return 0;
    }

    // 1. Call sqlite3_str_appendf
    const char* format = "%.*s";
    sqlite3_str_appendf(str, format, (int)(Size > 0 ? Size : 0), Data);

    // 2. First call to sqlite3_str_append
    int n1 = (int)(Size > 0 ? Size : 0);
    sqlite3_str_append(str, (const char*)Data, n1);

    // 3. Second call to sqlite3_str_append
    int n2 = (int)(Size / 2);
    sqlite3_str_append(str, (const char*)Data, n2);

    // 4. Call sqlite3_prepare_v2
    const char* sql = sqlite3_str_value(str);
    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
    if (sql != NULL) {
        int sql_len = (int)strlen(sql);
        sqlite3_prepare_v2(db, sql, sql_len, &stmt, &tail);
    }

    // 5. Call sqlite3_str_value again
    char* str_value = sqlite3_str_value(str);

    // 6. Call sqlite3_finalize
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }

    // Cleanup
    sqlite3_str_finish(str);
    sqlite3_close(db);
    remove(dummy_file);

    return 0;
}
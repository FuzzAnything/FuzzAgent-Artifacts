// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
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

static int fuzz_sqlite3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Prepare a simple SQL statement if data is provided
    if (Size > 0) {
        // Ensure the data is null-terminated for sqlite3_prepare_v2
        char *sql = (char *)malloc(Size + 1);
        if (sql == NULL) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql, Data, Size);
        sql[Size] = '\0';

        rc = sqlite3_prepare_v2(db, sql, (int)Size, &stmt, NULL);
        free(sql);

        if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW) {
            sqlite3_close(db);
            return 0;
        }
    }

    // If no statement was prepared, try a default one
    if (stmt == NULL) {
        rc = sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
    }

    // Invoke the target API functions in the specified order

    // 1. sqlite3_db_handle
    sqlite3 *handle = sqlite3_db_handle(stmt);
    (void)handle; // Use the handle to avoid unused variable warning

    // 2. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    (void)col_count;

    // 3. sqlite3_bind_parameter_count
    int bind_count = sqlite3_bind_parameter_count(stmt);
    (void)bind_count;

    // 4. sqlite3_sql
    const char *sql_text = sqlite3_sql(stmt);
    (void)sql_text;

    // 5. sqlite3_str_new
    sqlite3_str *str = sqlite3_str_new(db);
    if (str != NULL) {
        // Optionally append some data to the string
        sqlite3_str_appendf(str, "test");
        // Finish and free the string
        char *result = sqlite3_str_finish(str);
        if (result != NULL) {
            sqlite3_free(result);
        }
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    return fuzz_sqlite3(Data, Size);
}
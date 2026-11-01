// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:93832:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:93832:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:93832:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to ensure we have columns to query
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a SELECT statement based on input data
    // We ensure the SQL string is null-terminated and valid
    char *sql = (char *)malloc(Size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';

    // Try to prepare the statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    free(sql);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Execute the statement
    rc = sqlite3_step(stmt);

    // If we have a row, test the column functions
    if (rc == SQLITE_ROW) {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++) {
            // Test sqlite3_column_int
            (void)sqlite3_column_int(stmt, i);

            // Test sqlite3_column_text16
            const void *text16 = sqlite3_column_text16(stmt, i);
            (void)text16;

            // Test sqlite3_column_type
            int type = sqlite3_column_type(stmt, i);
            (void)type;

            // Test sqlite3_column_double
            double d = sqlite3_column_double(stmt, i);
            (void)d;

            // Test sqlite3_column_decltype16
            const void *decltype16 = sqlite3_column_decltype16(stmt, i);
            (void)decltype16;
        }
    }

    // Test with invalid column indices to trigger edge cases
    int col_count = sqlite3_column_count(stmt);
    if (col_count > 0) {
        (void)sqlite3_column_int(stmt, col_count);
        (void)sqlite3_column_int(stmt, -1);
        (void)sqlite3_column_text16(stmt, col_count);
        (void)sqlite3_column_text16(stmt, -1);
        (void)sqlite3_column_type(stmt, col_count);
        (void)sqlite3_column_type(stmt, -1);
        (void)sqlite3_column_double(stmt, col_count);
        (void)sqlite3_column_double(stmt, -1);
        (void)sqlite3_column_decltype16(stmt, col_count);
        (void)sqlite3_column_decltype16(stmt, -1);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return 0;
}
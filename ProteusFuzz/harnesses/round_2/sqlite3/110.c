// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:93828:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = (const char *)Data;
    int rc;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to ensure we have columns to query if the input is a SELECT */
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);

    /* Prepare the statement using the fuzzed input */
    const char *tail = NULL;
    rc = sqlite3_prepare_v2(db, sql, (int)Size, &stmt, &tail);

    if (rc == SQLITE_OK && stmt != NULL) {
        /* Exercise sqlite3_sql */
        const char *stmt_sql = sqlite3_sql(stmt);
        (void)stmt_sql;

        /* Exercise sqlite3_column_count */
        int col_count = sqlite3_column_count(stmt);

        /* Iterate over columns to exercise name, decltype */
        for (int i = 0; i < col_count; i++) {
            /* Exercise sqlite3_column_name */
            const char *col_name = sqlite3_column_name(stmt, i);
            (void)col_name;

            /* Exercise sqlite3_column_decltype */
            const char *col_decltype = sqlite3_column_decltype(stmt, i);
            (void)col_decltype;
        }

        /* Attempt to step the statement to see if we get rows */
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int col_count_step = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count_step; i++) {
                /* Exercise sqlite3_column_int */
                int val = sqlite3_column_int(stmt, i);
                (void)val;
            }
        }
    }

    /* Cleanup */
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}